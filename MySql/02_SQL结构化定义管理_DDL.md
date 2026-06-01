# SQL 结构定义化管理 (DDL) 核心笔记

## 1. 数据库级操作 (DDL)
* **创建数据库**:  
  `CREATE DATABASE [IF NOT EXISTS] 数据库名 [DEFAULT CHARSET 字符集 COLLATE 排序规则];`
* **修改数据库**:  
  `ALTER DATABASE 数据库名 CHARACTER SET 字符集;`
* **删除数据库**:  
  `DROP DATABASE [IF EXISTS] 数据库名;`
* **查看与切换**:
    * `SHOW DATABASES;` (查询所有数据库)
    * `USE 数据库名;` (切换当前数据库)

---

## 2. 数据类型 (Data Types)
### 2.1 数值类型
| 类型 | 描述 | 范围 (有符号) |
| :--- | :--- | :--- |
| **tinyint** | 微小整数 | -128 ~ 127 |
| **smallint** | 小整数 | -32,768 ~ 32,767 |
| **mediumint** | 中整数 | ±838万级别 |
| **int** | 标准整数 | ±21亿级别 |
| **bigint** | 大整数 | $2^{63}-1$ 级别 |
| **float / double** | 浮点数 | 单精度 / 双精度 |
| **decimal** | 定点数 | 财务计算首选，以字符串形式存储，精度不丢失 |

### 2.2 日期与时间类型
* **date**: `YYYY-MM-DD` (日期)
* **time**: `HH:mm:ss` (时间)
* **datetime**: `YYYY-MM-DD HH:mm:ss` (日期+时间)
* **timestamp**: 时间戳，随行修改自动更新
* **year**: `YYYY` (年份)

### 2.3 字符串类型
* **char(n)**: 定长字符串。空间固定，处理速度快，不足部分补空格。
* **varchar(n)**: 变长字符串。空间按需分配，节省存储空间，常用。
* **text**: 长文本串。

---

## 3. 表结构与约束 (Table & Constraints)
### 3.1 列的修饰属性
* **NOT NULL**: 必填项，不能为空。
* **DEFAULT**: 设置默认值。
* **AUTO_INCREMENT**: (MySQL) 自动增长，常用于主键。
* **IDENTITY(1,1)**: (SQL Server) 标识列，起始为1，增量为1。
* **unsigned**: 无符号数，仅存储正数。
* **zerofill**: 零填充，位数不足时前面补0。

### 3.2 表操作 (DDL)
* **建表**: `CREATE TABLE 表名 (字段名 类型 [约束], ...);`
* **改名**: `ALTER TABLE 旧名 RENAME AS 新名;`
* **增列**: `ALTER TABLE 表名 ADD 字段名 类型;`
* **改列**: 
    * `MODIFY`: 只改类型/约束 `ALTER TABLE 表名 MODIFY 字段名 新类型;`
    * `CHANGE`: 改列名+类型 `ALTER TABLE 表名 CHANGE 旧列名 新列名 类型;`
* **删表**: `DROP TABLE [IF EXISTS] 表名;`

---

## 4. 约束的修改 (ALTER TABLE 进阶)
通过 `ALTER TABLE` 可以在建表后管理各种约束：

### 4.1 主键约束 (Primary Key)
* **添加**: `ALTER TABLE 表名 ADD PRIMARY KEY (字段名...);`
* **删除**: `ALTER TABLE 表名 DROP PRIMARY KEY;`

### 4.2 外键约束 (Foreign Key)
外键用于建立两张表（主表和从表）之间的关联，保证数据的一致性。
* **添加**: `ALTER TABLE 从表名 ADD CONSTRAINT [外键名] FOREIGN KEY (从表字段) REFERENCES 主表名(主表字段);`
* **删除**: `ALTER TABLE 表名 DROP FOREIGN KEY 外键名称;`

### 4.3 唯一约束 (Unique)
* **添加**: `ALTER TABLE 表名 ADD CONSTRAINT [约束名] UNIQUE (字段名...);`
* **删除**: `ALTER TABLE 表名 DROP INDEX 约束名称;` (MySQL中唯一约束通过索引实现)

### 4.4 默认值约束 (Default)
* **添加**: `ALTER TABLE 表名 ALTER 字段名 SET DEFAULT 默认值;`
* **删除**: `ALTER TABLE 表名 ALTER 字段名 DROP DEFAULT;`

### 4.5 非空约束 (Not Null)
* **修改**: `ALTER TABLE 表名 MODIFY 字段名 类型 NOT NULL;`

---

## 5. 索引 (Index)
### 5.1 索引的作用
索引是一种单独的、物理的对数据库表中一列或多列的值进行排序的存储结构（类似书的目录）。
* **优点**: 极大地提高数据检索的速度。
* **缺点**: 会占用额外的磁盘空间，并且在插入、更新、删除数据时会降低速度（因为要同步维护索引）。

### 5.2 索引的分类
1. **普通索引 (INDEX)**: 最基本的索引，无特殊限制。
2. **唯一索引 (UNIQUE)**: 索引列的值必须唯一，但允许有空值。
3. **主键索引 (PRIMARY KEY)**: 一种特殊的唯一索引，不允许有空值（建主键时自动创建）。
4. **全文索引 (FULLTEXT)**: 针对大文本数据提取关键词，用于搜索引擎级别的匹配。
5. **组合索引 (复合索引)**: 在多个字段上创建一个索引，需遵循“最左前缀原则”。

### 5.3 索引的操作
* **创建索引**: `ALTER TABLE 表名 ADD INDEX 索引名 (字段名1, 字段名2...);`
* **查看索引**: `SHOW INDEX FROM 表名;`
* **删除索引**: `ALTER TABLE 表名 DROP INDEX 索引名;`