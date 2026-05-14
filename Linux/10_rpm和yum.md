# Linux 软件包管理工具核心笔记

## 一、 RPM 软件包管理 (本地包管理)

RPM（RedHat Package Manager）是 Linux 系列操作系统（如 RedHat、CentOS 等）中广泛使用的软件包打包和安装工具。它处理以 `.rpm` 为扩展名的文件。

### 1. RPM 包的命名规则
以 `firefox-60.8.0-1.el7.centos.x86_64.rpm` 为例：
- `firefox`：软件包的名称。
- `60.8.0-1`：软件的版本号。
- `el7.centos`：包适配的操作系统（Enterprise Linux 7 的 CentOS 版本）。
- `x86_64`：适配的硬件平台（64位系统。如果是 `i686` 或 `i386` 则代表32位系统；`noarch` 表示通用）。

### 2. RPM 常用查询指令 (`-q` 组合)
- **查询系统中所有已安装的 rpm 包**：
  `rpm -qa`
  `rpm -qa | more` （分页查看）
- **查询当前系统是否安装了指定的软件**：
  `rpm -qa | grep firefox`
- **严格查询某个软件是否安装**（需输入准确的软件名）：
  `rpm -q firefox`
- **查询已安装软件包的详细信息**：
  `rpm -qi firefox` （`i` 代表 info）
- **查询软件安装后生成了哪些文件，以及所在路径**：
  `rpm -ql firefox` （`l` 代表 list）
- **反向查询：查看某个系统文件属于哪一个 rpm 包**：
  `rpm -qf /etc/firefox/pref` （`f` 代表 file）

### 3. RPM 卸载指令 (`-e`)
- **常规卸载**：
  `rpm -e firefox` （`e` 代表 erase）
- **强制卸载（忽略依赖关系）**：
  `rpm -e --nodeps firefox` （警告：强行卸载可能会导致其他依赖该包的软件无法运行）

### 4. RPM 安装指令 (`-i`)
- **基本安装语法**：
  `rpm -ivh 包的完整名称.rpm`
  - `i`：install，表示安装。
  - `v`：verbose，显示安装的详细提示信息。
  - `h`：hash，以 `#` 号形式打印安装的进度条。

---

## 二、 YUM 软件包管理 (在线包管理)

### 1. YUM 的核心功能与优势
YUM（Yellow dog Updater, Modified）是一个基于 RPM 的前端软件包管理器。
- **核心优势：自动解决依赖关系。** 使用 RPM 安装软件时，经常会报错缺少 A 文件，安装 A 文件又要求 B 文件，非常繁琐。而 YUM 可以自动从配置的软件源（服务器）下载所需的全部依赖包，并一次性安装完毕。
- **功能特性**：支持在线下载、安装、更新、卸载软件，以及集中管理所有的软件包和依赖。

### 2. YUM 常用指令
- **搜索服务器上的软件包**：
  `yum search 软件名` （在 YUM 源中搜索包含该关键字的包）
  `yum list | grep 软件名` （查询 YUM 服务器上是否有需要安装的软件）
- **安装指定软件**：
  `yum install 软件名` 
  `yum install -y 软件名` （`-y` 参数表示在安装过程中所有的提示默认回复 `yes`，实现全自动安装）
- **卸载指定软件**：
  `yum remove 软件名` 
  `yum remove -y 软件名` （自动确认卸载）
- **更新指定软件**：
  `yum update 软件名`
- **更新系统所有软件及内核**（高危操作，生产环境慎用）：
  `yum update`
- **查看软件包的详细信息**：
  `yum info 软件名`

### 3. YUM 缓存管理指令
YUM 会将下载的包和源数据存在本地缓存中以加快速度，偶尔需要清理或重建：
- **清理本地 YUM 缓存**：
  `yum clean all` （清除下载的 rpm 包和旧的头文件）
- **重建 YUM 缓存**：
  `yum makecache` （把服务器的包信息下载到本地缓存，提升以后下载的速度）