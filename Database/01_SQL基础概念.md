# SQL 基础与底层概念核心笔记

## 1. SQL 语言分类 (SQL Classification)
* **DDL (Data Definition Language)**: 数据定义语言。用于定义或修改数据库的结构（Schema），如：`CREATE`, `ALTER`, `DROP`。
* **DML (Data Manipulation Language)**: 数据操作语言。用于对表中的数据进行增删改，如：`INSERT`, `UPDATE`, `DELETE`。
* **DQL (Data Query Language)**: 数据查询语言。用于检索数据，核心指令为 `SELECT`。
* **DCL (Data Control Language)**: 数据控制语言。用于定义数据库的访问权限和安全级别，如：`GRANT`, `COMMIT`, `ROLLBACK`。

---

## 2. 数据存储引擎对比 (MySQL)

| 特性 | MyISAM | InnoDB |
| :--- | :--- | :--- |
| **事务支持** | 不支持 | **支持 (事务性读写)** |
| **外键支持** | 不支持 | **支持 (数据一致性更高)** |
| **锁级别** | 表级锁 (并发低) | 行级锁 (并发高) |
| **全文索引** | 支持 | 支持 (5.6版本后) |
| **适用场景** | 读操作极多、更新少的简单业务 | **主流业务**、涉及多表关联、安全性要求高 |

---

## 3. 事务 (Transaction) 概念
* **定义**: 事务是将一系列 SQL 操作看作一个整体。
* **原则**: 逻辑上这组操作要么**全部执行成功**，要么**全部失败回滚**。
* **应用场景**: 银行转账、涉及多表同时更新的业务逻辑。