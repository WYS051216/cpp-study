# SQL 高级编程对象 (变量、存储过程、函数、触发器、视图)

## 1. 变量分类
### 1.1 全局变量
* 当服务启动时，将所有全局变量初始化为默认值。要更改必须具有管理员权限，作用域为整个服务器的整个生命周期。
* **显示**: `SHOW GLOBAL VARIABLES;` / `SHOW GLOBAL VARIABLES LIKE '...';`
* **设置**: `SET GLOBAL 变量名 = 值;` 或 `SET @@global.变量名 = 值;`
* **查询**: `SELECT @@global.变量名;`

### 1.2 会话变量
* 服务器为每个连接的客户端维护的一系列变量。其作用域仅限于当前连接，会话变量是独立的。
* **显示**: `SHOW SESSION VARIABLES;` / `SHOW SESSION VARIABLES LIKE '...';`
* **设置**: `SET SESSION 变量名 = 值;` 或 `SET @@session.变量名 = 值;` 或 `SET @@变量名 = 值;`
* **查询**: `SELECT @@变量名;` 或 `SELECT @@SESSION.变量名;`

### 1.3 用户变量
* MySQL中用户变量不用提前申明，在使用的时候直接用 `@变量名` 就可以了，其作用域为当前连接。
* **赋值**: 
  ① `SET @age = 18;` 或 `SET @age := 18;`
  ② `SELECT @age := 22;` (使用 SELECT 时必须使用 `:=` 赋值符)
  ③ `SELECT 字段 INTO @变量名 FROM 表名;`

### 1.4 局部变量
* 只作用在 `BEGIN` / `END` 语句块中。
* **定义**: `DECLARE age INT(3) DEFAULT 0;`
* **赋值**: `SET age = 10;` 或 `SELECT 10 INTO age;`

---

## 2. 存储过程 (Stored Procedure)
* **概念**: 是一组为了完成特定功能的存储在数据库中的SQL语句集，一次编译后永久生效。
* **优点**: 
  1. 运行速度快：执行一次后，直接从内存中调出。
  2. 减少网络传输：所有数据访问都在数据库服务器内部进行。
  3. 增强安全性。
* **使用语法**:
  ```sql
  -- ① 声明分隔符
  DELIMITER $$
  
  -- ② 创建存储过程
  CREATE PROCEDURE 存储过程名称 (
      [IN | OUT | INOUT] 参数名1 数据类型,
      [IN | OUT | INOUT] 参数名2 数据类型...
  )
  -- ③ 语句块开始
  BEGIN
      -- SQL 语句集
  END $$
  
  -- ④ 还原分隔符
  DELIMITER ;
  
  -- ⑤ 调用存储过程
  CALL 存储过程名称(参数1, 参数2...);
  ```

---

## 3. 事务 (Transaction)
* **概念**: 是访问并可能更新各种数据项的一个数据库操作序列，这些操作要么全部执行，要么全部不执行，是一个不可分割的工作单位。
* **事务的特性 (ACID)**:
  1. **原子性 (Atomicity)**。
  2. **一致性 (Consistency)**: 完成后，必须保证所有数据保持一致状态。
  3. **隔离性 (Isolation)**: 对数据操作的多个并发事务彼此独立，互不影响。
  4. **持久性 (Durability)**: 保证该事务对数据库的改变不被丢失。

---

## 4. 自定义函数与控制结构
### 4.1 自定义函数
* **语法**:
  ```sql
  CREATE FUNCTION 函数名(参数名1 数据类型...) 
  RETURNS 数据类型
  -- 函数特征说明：
  -- DETERMINISTIC (确定性)
  -- NO SQL (没有SQL语句，不修改数据)
  -- READS SQL DATA (只读取数据，不修改)
  -- MODIFIES SQL DATA (要修改数据)
  -- CONTAINS SQL (包含SQL语句)
  BEGIN
      -- SQL 逻辑
      RETURN 结果;
  END;
  ```
* **调用**: `SELECT 函数名();`

### 4.2 循环结构
1. **WHILE 循环**:
   ```sql
   WHILE 循环条件 DO
       -- SQL 语句集
   END WHILE;
   ```
2. **REPEAT 循环**:
   ```sql
   REPEAT
       -- SQL 语句集
   UNTIL 循环终止条件 END REPEAT;
   ```
3. **LOOP 循环**:
   ```sql
   标签: LOOP
       -- SQL 语句
       IF 循环终止条件 THEN 
           LEAVE 标签;
       END IF;
   END LOOP;
   ```

---

## 5. 触发器 (Trigger)
* **概念**: 用来保证数据完整性的一种方式，由事件来触发（如对表进行增删改操作时会被激活执行）。
* **创建语法**:
  ```sql
  CREATE TRIGGER 触发器名称 
  {BEFORE | AFTER} {INSERT | UPDATE | DELETE} 
  ON 表名 FOR EACH ROW
  BEGIN
      -- 执行的SQL操作
  END;
  ```
* **触发器类型与内部变量**:
  1. **INSERT 触发器**: `NEW` 表示将要或者已经新增的数据。
  2. **UPDATE 触发器**: `OLD` 表示修改前的数据，`NEW` 表示修改后的数据。
  3. **DELETE 触发器**: `OLD` 表示将要或已经删除的数据。

---

## 6. 视图 (View)
* **概念**: 是一张虚拟表，本身不存储数据。
* **操作语法**:
  * **创建**: `CREATE VIEW 视图名称 AS SELECT 列1... FROM 表名 WHERE 条件;`
  * **更新**: `CREATE OR REPLACE VIEW 视图名称 AS SELECT ...`
  * **删除**: `DROP VIEW IF EXISTS 视图名称;`
* **优点**:
  1. 定制用户数据、聚焦特定的数据。
  2. 简化数据操作。
  3. 提高安全性。
* *PS: 视图并不能提升查询速度，只是为了方便开发，反而会增加数据库服务器的压力。*