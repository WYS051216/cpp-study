# 🐳 Docker Compose 容器集群编排速查笔记

---

## 1. Docker Compose 核心概念与安装

### 1.1 基本概念
* **定位**：负责实现对 Docker 容器集群的**快速编排**。它是 Docker 官方推出的一个开源项目。
* **作用**：在实际微服务项目中，一个应用往往依赖多个容器（比如 Web应用 + MySQL + Redis）。Compose 允许我们通过定义一个 YAML 格式的配置文件（`docker-compose.yml`），写好多个容器之间的调用关系和启动参数，然后**仅通过一条命令就可以同时启动或关闭这些容器**，极大地解放了生产力。

### 1.2 安装步骤与权限赋予 (Linux 环境)
由于最新的 Docker Desktop 通常已经自带了 Compose，但如果在纯净的 Linux 服务器上，通常需要手动下载二进制文件。

**Step 1: 下载 Docker Compose**
从 GitHub 官方源下载（版本号可按需更改），并将其放入系统的 `/usr/local/bin` 目录下：
```bash
sudo curl -L "[https://github.com/docker/compose/releases/download/v2.20.2/docker-compose-$](https://github.com/docker/compose/releases/download/v2.20.2/docker-compose-$)(uname -s)-$(uname -m)" -o /usr/local/bin/docker-compose
```

**Step 2: 赋予可执行权限 (⚠️ 核心步骤)**
刚下载的文件是没有执行权限的，必须给它加上可执行权限（`+x`），系统才能运行它：
```bash
sudo chmod +x /usr/local/bin/docker-compose
```

**Step 3: 验证安装是否成功**
```bash
docker-compose --version
```

---

## 2. 使用 Docker Compose 的“三步曲”

任何复杂的微服务集群，用 Compose 部署都只分为标准的三步：

1. **定制镜像 (Dockerfile)**：编写 `Dockerfile` 定义各个微服务应用，并构建出对应的专属镜像文件。
2. **编写剧本 (docker-compose.yml)**：定义一个完整的业务单元，在 YAML 文件里安排好整体应用中的各个容器服务（网络、端口、依赖关系、数据卷等）。
3. **一键部署 (up)**：最后在 `docker-compose.yml` 所在的同级目录下，执行 `docker-compose up` 命令来启动并运行整个应用程序，完成一键部署上线。

---

## 3. 常规命令速查表

> **💡 注意：** 以下所有的 `docker-compose` 命令，**必须**在 `docker-compose.yml` 文件所在的目录下面执行，否则它会找不到“剧本”而报错。

| 命令 | 核心作用与说明 |
| :--- | :--- |
| `docker-compose -h` | **查看帮助**：列出所有可用的命令及参数说明。 |
| `docker-compose up` | **一键启动**：根据 yml 文件创建并启动所有的服务，但会在前台打印日志（按 Ctrl+C 会全部停止）。 |
| `docker-compose up -d` | **一键后台启动**：创建并启动所有服务，并在**后台静默运行**（企业最常用）。 |
| `docker-compose down` | **一键销毁**：停止并**彻底删除**由 up 命令启动的容器、默认网络。（⚠️ 注意：不会删除映射的数据卷）。 |
| `docker-compose ps` | **查看状态**：展示当前 compose 编排过的所有容器的运行状态。 |
| `docker-compose start` | **启动服务**：仅仅启动已经存在但处于停止状态的容器（不重新创建）。 |
| `docker-compose stop` | **停止服务**：仅仅停止正在运行的容器，但**不删除**容器。 |
| `docker-compose restart`| **重启服务**：重启所有容器。 |
| `docker-compose exec 服务ID /bin/bash` | **进入容器**：进入 yml 里面定义的某个具体服务（容器）内部。*(例如：`docker-compose exec mysql /bin/bash`)* |
| `docker-compose top` | **查看进程**：展示各个容器内当前正在运行的进程信息。 |
| `docker-compose logs` | **查看日志**：持续追踪输出所有容器的控制台日志。*(常用 `logs -f 服务ID` 查看特定服务的动态日志)* |
| `docker-compose config` | **检查配置**：检查当前的 `docker-compose.yml` 文件格式和语法是否正确，并打印解析后的结果。 |
| `docker-compose config -q`| **静默检查**：安静模式验证 yml 文件格式。如果没问题就没有任何输出，如果有错才会打印错误信息。 |