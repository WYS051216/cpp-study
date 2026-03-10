# 数据库基础笔记

## 一、数据库相关概念

### 1. 数据库（DB）
数据库（Database）：存储数据的仓库，数据是有组织地进行存储。

### 2. 数据库管理系统（DBMS）
数据库管理系统（Database Management System）：操纵和管理数据库的大型软件。

### 3. SQL
SQL（Structured Query Language）：操作关系型数据库的编程语言，定义了一套操作关系型数据库的统一标准。

---

# 二、关系型数据库

关系型数据库：建立在关系模型基础上，由多张相互连接的二维表组成的数据库。

## 特点

- 使用表存储数据，格式统一，便于维护
- 使用 SQL 语言操作，标准统一，使用方便

---

# 三、数据模型

常见数据模型：

- 层次模型
- 网状模型
- 关系模型

关系型数据库使用 **关系模型（二维表）**。

---

# 四、SQL

## 1. 通用语法

1. SQL 语句可以单行或多行书写，以分号结尾
2. 可以使用空格 / 缩进增强语句的可读性
3. SQL 不区分大小写，关键字建议使用大写

示例：

```sql
SELECT * FROM student;
```

---

# 五、SQL分类

SQL 主要分为四类：

| 分类 | 全称 | 作用 |
|----|----|----|
| DDL | Data Definition Language | 数据定义语言 |
| DML | Data Manipulation Language | 数据操作语言 |
| DQL | Data Query Language | 数据查询语言 |
| DCL | Data Control Language | 数据控制语言 |

---

# 六、DDL（数据定义语言）

DDL 用来定义数据库对象，例如：

- 数据库
- 表
- 字段

---

# 七、数据库操作

## 查询

### 查询所有数据库

```sql
SHOW DATABASES;
```

### 查询当前数据库

```sql
SELECT DATABASE();
```

---

## 创建数据库

```sql
CREATE DATABASE [IF NOT EXISTS] 数据库名
[DEFAULT CHARSET 字符集]
[COLLATE 排序规则];
```

示例：

```sql
CREATE DATABASE IF NOT EXISTS test
DEFAULT CHARSET utf8mb4;
```

---

## 删除数据库

```sql
DROP DATABASE 数据库名;
```

---

## 使用数据库

```sql
USE 数据库名;
```

---

# 八、表操作

## 查询所有表

```sql
SHOW TABLES;
```

---

## 查询表结构

```sql
DESC 表名;
```

---

## 查询建表语句

```sql
SHOW CREATE TABLE 表名;
```

---

# 九、创建表

```sql
CREATE TABLE 表名(
    字段1 字段类型 [COMMENT 字段1注释],
    字段2 字段类型 [COMMENT 字段2注释],
    ...
    字段n 字段类型 [COMMENT 字段n注释]
) [COMMENT 表注释];
```

示例：

```sql
CREATE TABLE student(
    id INT COMMENT '学生ID',
    name VARCHAR(20) COMMENT '学生姓名',
    age INT COMMENT '学生年龄'
) COMMENT '学生表';
```

---

# 十、数据类型

## 1. 数值类型

| 类型 | 大小 |
|----|----|
| TINYINT | 1 byte |
| SMALLINT | 2 byte |
| MEDIUMINT | 3 byte |
| INT / INTEGER | 4 byte |
| BIGINT | 8 byte |
| FLOAT | 4 byte |
| DOUBLE | 8 byte |
| DECIMAL | 高精度小数 |

---

## 2. 字符串类型

- CHAR
- VARCHAR
- TINYTEXT
- TEXT
- MEDIUMTEXT
- LONGTEXT
- TINYBLOB
- BLOB
- MEDIUMBLOB
- LONGBLOB

---

## 3. 日期时间类型

- DATE
- TIME
- YEAR
- DATETIME
- TIMESTAMP

---

# 十一、修改表结构

## 添加字段

```sql
ALTER TABLE 表名
ADD 字段名 类型(长度);
```

示例：

```sql
ALTER TABLE student
ADD gender CHAR(1);
```

---

## 修改字段类型

```sql
ALTER TABLE 表名
MODIFY 字段名 新数据类型(长度);
```

示例：

```sql
ALTER TABLE student
MODIFY name VARCHAR(50);
```

---

## 修改字段名和字段类型

```sql
ALTER TABLE 表名
CHANGE 旧字段名 新字段名 类型(长度);
```

示例：

```sql
ALTER TABLE student
CHANGE name student_name VARCHAR(50);
```

---

## 删除字段

```sql
ALTER TABLE 表名
DROP 字段名;
```

示例：

```sql
ALTER TABLE student
DROP age;
```

---

## 修改表名

```sql
ALTER TABLE 表名
RENAME TO 新表名;
```

示例：

```sql
ALTER TABLE student
RENAME TO student_info;
```

---

## 删除表

```sql
DROP TABLE 表名;
```