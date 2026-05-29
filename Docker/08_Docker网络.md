# 🐳 Docker 网络模式与通信机制核心笔记

## 1. Docker 网络基础概念与命令

### 1.1 Docker 网络的核心作用
Docker 网络主要用于解决两大问题：
1. **容器与宿主机之间**的通信。
2. **容器与容器之间**的互联与通信。

### 1.2 Docker 网络管理常用命令
通过 `docker network --help` 可以查看所有网络指令。日常最常用的命令如下：

| 终端指令 | 作用说明 |
| :--- | :--- |
| `docker network ls` | 查看当前 Docker 中存在的所有网络模式和网络列表。 |
| `docker network inspect 网络名`| 查看某个具体网络的底层详细信息（如网段、网关、连接的容器等）。 |
| `docker network create 网络名` | 创建一个自定义的网络。 |
| `docker network rm 网络名` | 删除一个指定的网络。 |
| `docker network prune` | 一键清理所有未被任何容器使用的孤立网络。 |

---

## 2. Docker 的四大基础网络模式详解

通过 `docker network ls` 默认可以看到 bridge、host、none 三种网络。

### 2.1 Bridge 模式 (桥接模式 - 默认)
Docker 服务默认会创建一个名为 `docker0` 的虚拟网桥。如果不指定网络，新创建的容器都会默认连接到这个网桥上。

* **底层逻辑 (veth pair)**：
  1. Docker 启动容器时，会根据 `docker0` 网桥的网段分配给容器一个内网 IP（Container-IP）。
  2. `docker0` 是所有容器的默认网关，处于同一宿主机内的容器可以通过它直接通信。
  3. **网卡配对**：Docker 会创建一对对等虚拟设备接口（veth pair）。一端（`eth0`）放在容器内部，另一端（类似 `vethxxx`）放在宿主机的 `docker0` 网桥上，就像一根虚拟网线把容器和宿主机连在了一起。

* **📌 实战案例**：
  ```bash
  # 不加任何参数，默认就是 bridge 模式
  docker run -d -p 8080:8080 --name tomcat_bridge tomcat
  ```
  *(注：此时在宿主机输入 `ip addr` 或者 `ifconfig`，你会看到多出了一个 `veth` 开头的虚拟网卡，这就是插在宿主机这头的“网线”。)*

### 2.2 Host 模式 (主机模式)
容器将**不会**获得一个独立的 Network Namespace，也不会虚拟出自己的网卡和 IP。相反，它直接与宿主机共用网络环境。

* **特点**：直接使用宿主机的 IP 地址与外界通信，不需要额外进行 NAT（网络地址转换）。性能最高，但容易发生端口冲突。

* **📌 实战案例**：
  ```bash
  # 使用 --network host 指定网络模式为 host
  docker run -d --network host --name tomcat_host tomcat
  ```
  *(⚠️ 警告：在 host 模式下，写 `-p 8080:8080` 进行端口映射是无效的。容器直接占用宿主机的 8080 端口。如果你在同一台机器上跑两个 host 模式的 Tomcat，第二个会因为端口被占用而启动失败。)*

### 2.3 None 模式
* **特点**：容器有自己独立的 Network Namespace，但是 Docker 引擎并不会为它进行任何网络配置（没有网卡，没有 IP，没有路由）。
* **应用场景**：极高安全性的断网环境，或者留给开发者完全自己手动配置网络。日常基本不用。

### 2.4 Container 模式
新建的容器不会创建自己的网卡和配置 IP，而是和一个**已经存在的指定容器**共享同一个 IP 和端口范围。

* **特点**：这两个容器在网络层面完全共享（相当于住在同一个房间，共用一个门牌号），但在文件系统、进程列表等方面依然是隔离的。

* **📌 实战案例**：
  ```bash
  # 1. 先启动一个基础容器 c1 (比如用 alpine 镜像)
  docker run -it --name c1 alpine /bin/sh
  
  # 2. 再启动容器 c2，指定网络模式共享 c1 的网络
  docker run -it --network container:c1 --name c2 alpine /bin/sh
  ```
  *(此时在 c1 和 c2 内部各自执行 `ip addr`，你会发现它们的 IP 地址、MAC 地址一模一样！如果 c1 停止了，c2 的网络也会随之断开。)*

---

## 3. 进阶：自定义网络模式 (Custom Network)

**为什么我们需要自定义网络？**
在默认的 `bridge` (`docker0`) 模式下，容器的 IP 在重启后可能会发生变化。如果我们在微服务代码里把数据库 IP 写死了，容器一重启，系统就崩了。
**自定义网络支持通过“容器名称”直接进行网络通信（自带 DNS 解析）**，即使 IP 变了，只要容器名字没变，依然能ping通！

* **📌 实战案例：使用自定义网络实现容器名通信**
  ```bash
  # 1. 创建一个名为 my_net 的自定义网络 (默认底层依然是 bridge)
  docker network create my_net
  
  # 2. 将容器 A 连入该网络
  docker run -d --name app_A --network my_net tomcat
  
  # 3. 将容器 B 连入该网络
  docker run -d --name app_B --network my_net tomcat
  
  # 4. 测试互通：进入 app_A 内部，直接 ping app_B 的名字！
  docker exec -it app_A ping app_B
  ```
  *(结果：在自定义网络下，`ping app_B` 会自动解析为 app_B 容器当前的动态 IP，实现完美互通！这在微服务集群部署中极其重要。)*