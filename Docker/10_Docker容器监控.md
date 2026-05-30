# 🐳 Docker 可视化管理与企业级监控 (Portainer & CIG)

---

## 1. 原生轻量级监控小工具 (`docker stats`)
在不安装任何第三方软件的情况下，Docker 自带了一个非常实用的性能监控命令。

* **命令**：`docker stats`
* **功能**：类似 Linux 系统的 `top` 命令，可以**实时查看**当前所有运行中容器的详细资源占用情况。
* **展示的核心数据**：CPU 使用率、内存使用量/限制值、网络 I/O（收发数据量）、磁盘 Block I/O 以及 PIDs（进程数）。

---

## 2. 轻量级可视化管理台：Portainer
对于单机环境或轻量级集群，Portainer 是目前最受欢迎的图形化管理界面，可以通过网页点点鼠标就能完成镜像拉取、容器启停和日志查看。

**一键安装与启动命令：**
```bash
docker run -d -p 8000:8000 -p 9000:9000 --name portainer \
    --restart=always \
    -v /var/run/docker.sock:/var/run/docker.sock \
    -v portainer_data:/data \
    portainer/portainer-ce
```
*(💡 安装完成后，在浏览器访问 `http://宿主机IP:9000` 即可进入中文管理后台。)*

---

## 3. 企业级重量级监控：CIG 平台架构
原生 `docker stats` 的缺点是只能看“实时数据”，没有历史记录，也没有报警功能。企业生产环境中，通常使用 **CIG 架构** 来搭建全方位的监控系统。

### 3.1 CIG 三剑客功能解析
CIG 是三个开源软件的首字母缩写，它们各司其职，形成了一条完整的数据处理流水线：

| 组件名称 | 核心角色 | 功能详解 |
| :--- | :--- | :--- |
| **C**advisor | **数据采集器** | 由 Google 开源的容器监控工具。它负责在后台默默收集所在宿主机上所有 Docker 容器的运行状况（CPU、内存、网络等指标数据）。 |
| **I**nfluxDB | **时序数据库** | 专门用来存储“带时间标签”的数据。cAdvisor 采集到的海量数据会源源不断地写入 InfluxDB 中进行持久化保存，以便后期查询历史记录。 |
| **G**rafana | **数据展示大屏** | 颜值极高的开源数据可视化看板。它负责从 InfluxDB 中读取数据，并把冷冰冰的数字渲染成各种酷炫的折线图、仪表盘，供运维人员查看，甚至可以配置钉钉/邮件报警。 |

### 3.2 搭建 CIG 平台的步骤 (基于 Docker Compose)
既然是启动三个相互依赖的容器，我们最完美的方案是使用上节课学过的 `docker-compose` 来一键编排！

**Step 1: 创建工作目录**
```bash
mkdir -p /mydata/cig
cd /mydata/cig
```

**Step 2: 编写剧本 (`docker-compose.yml`)**
在当前目录下创建一个名为 `docker-compose.yml` 的文件，填入以下标准配置：
```yaml
version: '3.1'

services:
  # 1. 时序数据库
  influxdb:
    image: tutum/influxdb:0.9
    restart: always
    environment:
      - PRE_CREATE_DB=cadvisor
    ports:
      - "8083:8083"
      - "8086:8086"
    volumes:
      - ./data/influxdb:/data

  # 2. 数据采集器
  cadvisor:
    image: google/cadvisor
    links:
      - influxdb:influxsrv
    command: -storage_driver=influxdb -storage_driver_db=cadvisor -storage_driver_host=influxsrv:8086
    restart: always
    ports:
      - "8080:8080"
    volumes:
      - /:/rootfs:ro
      - /var/run:/var/run:rw
      - /sys:/sys:ro
      - /var/lib/docker/:/var/lib/docker:ro

  # 3. 可视化面板
  grafana:
    image: grafana/grafana
    restart: always
    links:
      - influxdb:influxsrv
    ports:
      - "3000:3000"
    volumes:
      - ./data/grafana:/var/lib/grafana
    environment:
      - HTTP_USER=admin
      - HTTP_PASS=admin
```

**Step 3: 一键启动 CIG 集群**
```bash
docker-compose up -d
```

**Step 4: 配置 Grafana 大屏 (网页端操作)**
1. **登录大屏**：浏览器访问 `http://宿主机IP:3000`（账号密码默认为 admin/admin）。
2. **添加数据源 (Data Source)**：
   * 选择 `InfluxDB`。
   * HTTP URL 填入：`http://influxdb:8086`。
   * Database 名字填入：`cadvisor`，然后点击 Save & Test。
3. **导入仪表盘 (Dashboard)**：
   * 在 Grafana 侧边栏选择 `+` -> `Import`。
   * 可以输入 Grafana 官方图表模板 ID（比如常用的 Docker 监控模板 ID：`193` 或 `10566`），点击 Load 即可瞬间生成酷炫的监控大屏！