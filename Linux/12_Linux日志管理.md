# Linux 日志管理核心笔记

## 一、 Linux 日志的基本介绍

### 1. 日志的基本概念
- **定义**：日志文件是重要的系统信息文件，其中记录了很多重要的系统事件，包括用户的登录信息、系统的启动信息、系统的安全信息、邮件相关信息、各种服务相关信息等。
- **核心价值**：日志对于系统安全和日常运维至关重要。它记录了系统每天发生的各种事情，运维人员可以通过日志检查错误发生的原因，或者在系统受到攻击时分析攻击者留下的痕迹。**一句话理解日志：日志是用来记录重大事件的排障与审计工具。**
- **存储位置**：Linux 的标准日志信息统一保存在 `/var/log/` 目录中。

### 2. 常用日志文件及功能列举
在 `/var/log/` 目录下，最核心的常用日志文件包括：
- `/var/log/messages`：整个系统的核心常规日志，记录系统绝大多数重要信息、警告和错误（在 Ubuntu/Debian 中对应 `/var/log/syslog`）。
- `/var/log/secure`：系统的安全与认证日志，记录所有用户登录、`su` 切换、`sudo` 权限执行以及 SSH 远程连接的尝试（在 Ubuntu 中对应 `/var/log/auth.log`）。
- `/var/log/cron`：定时任务日志，记录 `crontab` 计划任务的触发和执行状态。
- `/var/log/maillog`：邮件系统日志，记录系统内部邮件（如 postfix、sendmail）的发送和接收状态。
- `/var/log/boot.log`：系统启动日志，记录 Linux 在开机引导过程中各个服务的启动状态与耗时。
- `/var/log/wtmp`：永久记录所有用户登录、注销及系统重启的历史信息。该文件为二进制文件，不能直接用 `cat` 查看，需使用 `last` 命令读取。
- `/var/log/btmp`：记录所有失败的登录尝试（即输入错误密码的记录）。同样为二进制文件，需使用 `lastb` 命令读取，常用于排查暴力破解攻击。

---

## 二、 Linux 日志管理服务 (rsyslogd)

现代 Linux 系统普遍使用 **`rsyslogd`**（Rocket-fast System for Log Processing）服务来统一管理和分发系统日志。

### 1. 管理 rsyslogd 服务
`rsyslogd` 是一个后台守护进程，其管理指令与普通服务一致：
- 查看日志服务状态：`systemctl status rsyslogd`
- 启动/停止/重启服务：`systemctl [start | stop | restart] rsyslogd`

### 2. 核心配置文件解析
`rsyslogd` 的核心配置文件位于 `/etc/rsyslog.conf`。该文件规定了**什么级别的日志**应该发送到**哪一个日志文件**中。其配置核心语法为：
```text
服务类型.日志级别    /path/to/log_file
```

#### (1) 服务类型 (Facility)
系统将日志来源划分为不同的设施：
- `auth` / `authpriv`：安全和认证相关。
- `cron`：定时任务相关。
- `mail`：邮件系统相关。
- `kern`：Linux 内核产生的信息。
- `user`：用户程序产生的常规信息。
- `local0` 到 `local7`：保留给用户自定义使用的设施。
- `*`：代表所有服务。

#### (2) 日志级别 (Level)
从低到高（信息重要程度递增），rsyslogd 划分了 8 个标准级别。**配置某一级别后，该级别及以上的所有高危日志都会被记录**：
1. `debug`：调试信息，内容最庞大。
2. `info`：常规提示信息（最常用）。
3. `notice`：注意信息，稍微偏离正常。
4. `warning` / `warn`：警告信息，可能存在潜在错误。
5. `err` / `error`：错误信息，某些功能已失效。
6. `crit`：严重错误，影响整个系统或大模块。
7. `alert`：极其严重，必须立即采取措施。
8. `emerg` / `panic`：崩溃级别，系统已不可用。
- `*` 代表所有级别；`none` 代表不记录任何信息。

---

## 三、 Linux 自定义日志服务

我们可以利用 `rsyslogd` 留给我们的 `local0` - `local7` 空闲设施，将我们自己编写的脚本或软件日志接入到系统的统一日志管理中。

### 1. 配置自定义日志步骤
假设我们需要把一个自定义项目的常规提示和错误日志，统一记录到 `/var/log/my_project.log` 中。

- **步骤 1**：编辑 rsyslog 配置文件：
  `sudo nano /etc/rsyslog.conf`
- **步骤 2**：在文件末尾追加一条自定义规则（使用 `local0` 设施）：
  ```text
  # 记录 local0 设施产生的所有 info 及以上级别的日志
  local0.info    /var/log/my_project.log
  ```
- **步骤 3**：重启日志服务使配置生效：
  `sudo systemctl restart rsyslogd`

### 2. 实战触发与测试：`logger` 指令
配置完成后，我们可以使用 Linux 自带的 `logger` 工具向该自定义设施发送测试信息。
- **指令语法**：`logger -p 设施名.级别 "日志内容"`
- **测试命令**：
  ```bash
  logger -p local0.info "My custom project started successfully."
  logger -p local0.err "Error: Failed to connect to local database!"
  ```
- **验证**：查看 `/var/log/my_project.log` 文件，即可看到带有时间戳和主机名的自定义日志条目。

---

## 四、 Linux 日志轮替 (logrotate)

### 1. 什么是日志轮替？
日志轮替（Log Rotation）是按照一定的规则（如按天、按周、或文件达到指定大小），自动将旧的日志文件重命名备份，并删除超过保留期限的超旧文件，同时创建新的空白日志文件继续记录。
- **核心目的**：**防止单个日志文件无限膨胀撑爆磁盘空间**，并保持日志的可读性。

### 2. 日志轮替的工作原理
Linux 依赖 **`logrotate`** 工具实现轮替。它不需要常驻后台，而是通过系统的定时任务（每天由 `cron` 触发 `/etc/cron.daily/logrotate` 脚本）来定时检查和执行。

### 3. 核心配置文件与自定义规则
`logrotate` 的主配置文件是 `/etc/logrotate.conf`。我们自定义的轮替规则通常存放在 `/etc/logrotate.d/` 目录中。

以下是一个为自定义日志 `/var/log/my_project.log` 编写的轮替规则示例：
在 `/etc/logrotate.d/` 目录下创建一个叫 `my_project` 的文件，内容如下：
```text
/var/log/my_project.log {
    daily                    # 每天轮替一次（可选 weekly, monthly）
    rotate 7                 # 保留最近的 7 个备份文件，多余的自动删除
    missingok                # 如果日志文件丢失，忽略报错继续下一步
    notifempty               # 如果日志文件是空的，则不进行轮替
    compress                 # 对轮替出的旧日志进行 gzip 压缩（生成 .log.1.gz）
    dateext                  # 轮替文件使用日期作为后缀（如 .log-20260516）
    sharedscripts            # 所有的文件轮替完毕后，只执行一次下面的脚本
    postrotate
        /bin/kill -HUP `cat /var/run/syslogd.pid 2> /dev/null` 2> /dev/null || true
    endscript                # 轮替后通知 rsyslogd 重新加载，否则它会继续往旧文件里写
}
```

---

## 五、 Linux 内存日志 (journald)

在现代 Linux（采用 Systemd 管理的系统）中，除了传统写入磁盘的 `rsyslogd`，还有一个专门负责**捕获系统早期启动、内核及各服务底层原生输出**的内存日志系统 —— **`systemd-journald`**。

### 1. 内存日志的特点
- **实时性高**：日志直接写入内存（虚拟文件系统），响应速度极快，随时发生变化。
- **临时性**：由于保存在内存中，**一旦 Linux 系统重启，内存日志就会被彻底清空**，不会永久保留。
- **作用**：常用于排查刚刚发生的系统服务启动失败、内核报错等即时故障。

### 2. 操作内存日志的常用指令：`journalctl`

`journalctl` 是查看和管理内存日志的核心工具。常用排查命令如下：

- **查看所有内存日志**（从开机到当前的所有底层日志）：
  `journalctl`
- **实时滚屏查看最新日志**（类似于 `tail -f`，排障最常用）：
  `journalctl -f`
- **查看本次开机以来的所有日志**：
  `journalctl -b`
- **查看指定服务（如 sshd）的底层内存日志**：
  `journalctl -u sshd.service`
- **查看特定级别及以上的严重错误日志**（如只看报错 `err`）：
  `journalctl -p err`
- **查看最近指定时间段内的日志**：
  `journalctl --since "2026-05-16 12:00:00"`
  `journalctl --since "1 hour ago"` （查看最近 1 小时内发生的事）
- **只显示最新的 N 行日志**：
  `journalctl -n 50` （显示最新 50 行）
```