# SQL 查询与函数 (DQL) 核心笔记

## 1. 数据查询进阶 (DQL)
### 1.1 基础查询语法
* **标准语法**: 
  `SELECT [ALL | DISTINCT] * | 字段名1 [AS 别名1][, 字段名2...] FROM 表名 [WHERE 查询条件] [GROUP BY 字段名...] [HAVING 过滤条件] [ORDER BY 排序字段 ASC|DESC];`
* **去重查询**: `SELECT DISTINCT 字段名 FROM 表名;` (去除查询结果中重复的记录)。

### 1.2 比较操作符 (WHERE 子句)
| 操作符 / 关键字 | 说明 | 示例 |
| :--- | :--- | :--- |
| `>`, `<`, `=`, `!=` | 基础关系比较 | `WHERE age > 18` |
| `IS NULL` / `IS NOT NULL` | 判断是否为空值 | `WHERE email IS NULL` |
| `BETWEEN ... AND ...` | 在某个范围之内 (包含边界) | `WHERE age BETWEEN 18 AND 25` |
| `IN (值1, 值2...)` | 匹配列表中的任意一个值 | `WHERE dept_id IN (1, 2, 3)` |
| `LIKE` | 模糊查询 (`%`代表任意字符，`_`代表单个字符) | `WHERE name LIKE '张%'` |

### 1.3 聚合函数
用于对一组数据进行汇总计算（常配合 `GROUP BY` 使用）：
1. **`COUNT()`**: 统计满足条件的行数。`COUNT(*)` 统计总行数；`COUNT(字段)` 统计该字段非空的行数。
2. **`SUM()`**: 计算该列数值的总和。
3. **`AVG()`**: 计算该列数值的平均值。
4. **`MAX()`**: 求最大值。
5. **`MIN()`**: 求最小值。

### 1.4 排序与分组
* **分组 (`GROUP BY`)**: 将查询结果按一个或多个字段分组。
* **分组后过滤 (`HAVING`)**: 对 `GROUP BY` 分好的组进行条件筛选。
* **排序 (`ORDER BY`)**: 
  * `ASC`: 升序（默认）。
  * `DESC`: 降序。
  * *注意：`ORDER BY` 必须放在 `WHERE` 和 `GROUP BY` 之后。*

---

## 2. 常用内置函数
### 2.1 字符串函数
| 函数 | 描述 |
| :--- | :--- |
| `CONCAT(s1, s2...)` | 字符串拼接 |
| `LENGTH(str)` / `CHAR_LENGTH(str)`| 获取字节长度 / 获取字符个数 |
| `LOWER(str)` / `UPPER(str)` | 转换为小写 / 转换为大写 |
| `LEFT(str, n)` / `RIGHT(str, n)` | 从左侧 / 右侧截取 n 个字符 |
| `LTRIM(str)` / `RTRIM(str)` / `TRIM(str)`| 去除左侧 / 右侧 / 两侧的空格 |
| `REPLACE(str, s1, s2)` | 用 s2 替换 str 中所有的 s1 |
| `SUBSTRING(str, n, len)` | 从第 n 个位置开始截取长度为 len 的字符串 |

### 2.2 数学函数
| 函数 | 描述 |
| :--- | :--- |
| `ABS(x)` | 返回 x 的绝对值 |
| `FLOOR(x)` / `CEIL(x)` | 向下取整 / 向上取整 |
| `TRUNCATE(x, d)` | 返回数字 x 保留到小数点后 d 位的值（直接截断，不四舍五入） |
| `ROUND(x, d)` | 返回数字 x 保留到小数点后 d 位的值（四舍五入） |
| `RAND()` | 返回 0~1 之间的随机浮点数 |
| `MOD(n, m)` | 返回 n 除以 m 的余数 |

### 2.3 日期和时间函数
* `CURDATE()` / `CURRENT_DATE()`: 返回当前日期 (YYYY-MM-DD)。
* `CURTIME()` / `CURRENT_TIME()`: 返回当前时间 (HH:mm:ss)。
* `NOW()` / `CURRENT_TIMESTAMP()`: 返回当前日期和时间。
* `YEAR(d)` / `MONTH(d)` / `DAYOFMONTH(d)`: 提取年份 / 月份 / 日。
* `HOUR(t)` / `MINUTE(t)` / `SECOND(t)`: 提取小时 / 分钟 / 秒。
* `ADDDATE(d, n)`: 计算起始日期 d 加上 n 天后的日期。
* `TIMESTAMPDIFF(type, d1, d2)`: 计算 d1 和 d2 之间的时间差 (type 可选 day, month, year 等)。
* `DATE_FORMAT(d, f)`: 按格式 f 格式化日期 (如 `%Y-%m-%d`)。

### 2.4 条件判断函数
1. **`IF(条件, 表达式1, 表达式2)`**: 如果条件满足返回表达式1，否则返回表达式2。
2. **`IFNULL(表达式1, 表达式2)`**: 如果表达式1不为空返回表达式1，否则返回表达式2。
3. **`CASE WHEN` (分支判断)**:
   ```sql
   CASE 
       WHEN 条件1 THEN 表达式1
       WHEN 条件2 THEN 表达式2
       ...
       [ELSE 表达式n]
   END
   ```

### 2.5 系统信息函数
* `VERSION()`: 获取数据库版本号。
* `CONNECTION_ID()`: 获取服务器连接 ID。
* `DATABASE()` / `SCHEMA()`: 获取当前数据库名。
* `USER()` / `CURRENT_USER()`: 获取当前登录用户。