# SQL 约束、索引与多表查询核心笔记

## 1. 表与表之间的关系
* **关系体现**: 数据库通过**主外键关联关系**来体现表与表之间的关联关系（例如：学生表 `属于` 班级表）。
* **主外键关联关系**: 物理上两个表之间本身并没有任何关系，然而实际上存在从属关系。可以在学生表中添加一个字段，表明学生所属的班级，该字段的值使用的是班级表中的主键，在学生表中称之为**外键**。这样就产生了关联关系。
* **定义外键语法**: 
  `FOREIGN KEY(本表中的字段名) REFERENCES 参考表(参考字段)`

## 2. 约束 (Constraints)
### 2.1 主键约束 (Primary Key)
* **作用**: 保证数据的唯一性。
* **添加**: `ALTER TABLE 表名 ADD PRIMARY KEY (字段名1, 字段名2...);`
* **删除**: `ALTER TABLE 表名 DROP PRIMARY KEY;`

### 2.2 外键约束 (Foreign Key)
* **添加**: 
  `ALTER TABLE 表名 ADD CONSTRAINT 外键名称 FOREIGN KEY(本表的字段名) REFERENCES 表名2(表名2的字段名);`
  *(注：外键名称由自己指定)*
* **删除**: `ALTER TABLE 表名 DROP FOREIGN KEY 外键名称;`

### 2.3 唯一约束 (Unique)
* **添加**: 
  `ALTER TABLE 表名 ADD CONSTRAINT 约束名称 UNIQUE (字段名1, ...);`
  *(注：约束名称由自己指定)*
* **删除**: `ALTER TABLE 表名 DROP KEY 约束名称;`

### 2.4 非空约束 (Not Null)
* **添加**: `ALTER TABLE 表名 MODIFY 字段名 列类型 NOT NULL;`
* **删除**: `ALTER TABLE 表名 MODIFY 字段名 列类型 NULL;`

### 2.5 默认值约束 (Default)
* **添加**: `ALTER TABLE 表名 ALTER 字段名 SET DEFAULT 默认值;`
* **删除**: `ALTER TABLE 表名 ALTER 字段名 DROP DEFAULT;`

### 2.6 自增约束 (Auto_Increment)
* **添加**: `ALTER TABLE 表名 MODIFY 字段名 列类型 AUTO_INCREMENT;`
  *(注：设置自增通常也包含了 NOT NULL)*
* **删除**: `ALTER TABLE 表名 MODIFY 字段名 列类型;` (去掉 AUTO_INCREMENT 即可)

---

## 3. 索引 (Index)
### 3.1 索引的概念与作用
* **概念**: 一种单独的、物理的对数据库表中一列或多列的值进行排序的存储结构。它是表中一列或多列值的集合和相应的指向表中物理标识这些值的数据页的逻辑指针清单。
* **作用**: 
  1. 保证数据的准确性。
  2. 提高检索速度。
  3. 提高系统性能。

### 3.2 索引的类型
* **唯一索引 (`UNIQUE`)**: 索引列的值必须唯一。
* **普通索引 (`INDEX`)**: 允许出现相同的索引内容。
* **主键索引 (`PRIMARY KEY`)**: 不允许出现相同的索引内容。
* **全文索引 (`FULLTEXT INDEX`)**: 针对值中的某个单词。
* **组合索引**: 实际上是将多个字段建到一个索引里，列值的组合必须唯一。

### 3.3 索引的创建、查看与删除
* **创建普通索引**: `ALTER TABLE 表名 ADD INDEX 索引名称 (字段名1, 字段名2...);`
* **创建全文索引**: `ALTER TABLE 表名 ADD FULLTEXT 索引名称 (字段名1, ...);`
* **查看索引**: `SHOW INDEX FROM 表名;`
* **删除索引**: `ALTER TABLE 表名 DROP INDEX 索引名称;`

---

## 4. 多表查询
### 4.1 连接查询分类
1. **笛卡尔积**: 两表数据的无条件组合。
2. **内连接 (Inner Join)**: 相当于在笛卡尔积的基础上加了连接条件（没有条件，上升为笛卡尔积）。
   * 语法1: `SELECT 字段名1, 字段名2... FROM 表1 [INNER] JOIN 表2 [ON 条件]`
   * 语法2: `SELECT ... FROM 表1, 表2 [WHERE 关联条件 AND 查询条件]`
3. **外连接 (Outer Join)**: 涉及主表和从表，要查询的信息主要来自于哪张表，哪张表就是主表。**结果 = 内连接的结果 + 主表中有的而内连接结果中没有的记录。**
   * **左外连接 (`LEFT JOIN`)**: 左边为主表。
     `SELECT 字段名1, ... FROM 主表 LEFT JOIN 从表 [ON 连接条件]`
   * **右外连接 (`RIGHT JOIN`)**: 右边为主表。
     `SELECT 字段名1, ... FROM 从表 RIGHT JOIN 主表 [ON 连接条件]`

### 4.2 子查询
* **概念**: 嵌套在其他查询中的查询。
* **常见位置**:
  1. `SELECT ... FROM (子查询);` (执行时机是在查询出来之后)
  2. `FROM ... WHERE (子查询) 之间;`
  3. `WHERE (子查询) 之后;`