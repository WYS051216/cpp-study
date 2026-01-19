# [STL] Set：自动排序与去重的红黑树

## 1. 核心本质 (Backend View)
- **是什么**：`set` 是一个关联式容器。
- **底层结构**：**红黑树 (Red-Black Tree)** —— 一种自动平衡的二叉树。
    - *为什么重要*：因为是平衡树，所以查找、插入、删除的时间复杂度都是 **$O(\log N)$**（非常快且稳定），适合存储海量需要去重的数据。
- **特性**：
    1.  **自动排序**：插入时自动按从小到大排列。
    2.  **自动去重**：容器内不允许有重复元素。

### 🔎 Set vs Multiset
- **`std::set`**：严禁重复（插入重复值会失败）。
- **`std::multiset`**：允许重复（主要用于排序）。

---

## 2. API 操作速查表

| 操作类别 | 函数名 | 描述 / 后端关键点 |
| :--- | :--- | :--- |
| **构造与赋值** | `set<T> st;` | 默认构造。 |
| | `set(const set &st);` | 拷贝构造。 |
| | `operator=` | 重载等号赋值。 |
| **容量与大小** | `size()` | 返回容器中元素的数目。 |
| | `empty()` | 判断容器是否为空（true 为空）。 |
| | `swap(st)` | 交换两个集合的所有数据（高效指针交换）。 |
| **插入 (增)** | `insert(elem)` | **面试考点**：返回 `pair<iterator, bool>`。通过 `.second` 判断是否插入成功。 |
| **删除 (删)** | `erase(key)` | **按值删除**。比如 `s.erase(30)`，删掉 30。 |
| | `erase(pos)` | **按迭代器删除**。比如 `s.erase(s.begin())`。 |
| | `clear()` | **清空**。删掉整棵树。 |
| **查找 (查)** | **`find(key)`** | **核心！** 返回迭代器。若没找到返回 `end()`。 |
| | `count(key)` | 对于 set，结果只能是 0 或 1（用于快速判断是否存在）。 |

---

## 3. 常用操作代码模板

```cpp
#include <iostream>
#include <set>
using namespace std;

// 通用打印函数
void printSet(const set<int> &s)
{
    for (auto it = s.begin(); it != s.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
}

// --- 模块1: 构造与基本特性 ---
void test_basics()
{
    cout << "=== 1. 基本特性 (自动排序+去重) ===" << endl;
    set<int> s1;
    s1.insert(10);
    s1.insert(40); // 乱序插入
    s1.insert(20);
    s1.insert(30);
    s1.insert(30); // 重复插入，会被忽略

    cout << "s1 内容: ";
    printSet(s1); // 输出应该是 10 20 30 40

    if (!s1.empty()) {
        cout << "s1 大小: " << s1.size() << endl;
    }
}

// --- 模块2: 查找与统计 (核心) ---
void test_find_count()
{
    cout << "\n=== 2. 查找与统计 ===" << endl;
    set<int> s1;
    s1.insert(10);
    s1.insert(30);
    s1.insert(50);

    // find 测试 (必须和 end() 比较)
    auto pos = s1.find(30);
    if (pos != s1.end()) {
        cout << "find: 找到了 30" << endl;
    } else {
        cout << "find: 没找到" << endl;
    }

    // count 测试 (0 或 1)
    int num = s1.count(30);
    cout << "count: 30 出现的次数: " << num << endl;
}

// --- 模块3: 插入结果检测与删除 ---
void test_crud()
{
    cout << "\n=== 3. 插入检测与删除 ===" << endl;
    set<int> s1;
    s1.insert(10);
    s1.insert(20);

    // 1. 检测插入是否成功
    pair<set<int>::iterator, bool> ret = s1.insert(10); // 故意插入重复的
    if (ret.second) {
        cout << "插入 10 成功" << endl;
    } else {
        cout << "插入 10 失败 (原因: 元素已存在)" << endl;
    }

    // 2. 删除操作
    s1.erase(10); // 按值删除
    cout << "删除 10 后: ";
    printSet(s1);
    
    s1.clear(); // 清空
    cout << "清空后大小: " << s1.size() << endl;
}

int main()
{
    test_basics();
    test_find_count();
    test_crud();
    return 0;
}
```
## set容器排序
**主要技术点**
- 利用仿函数，可以改变排序规则

**什么是仿函数**
- **定义**：一个重载`operator()`的类
- **作用**：让类对象可以像函数一样被调用
## 代码模板
```cpp
// 1. 定义仿函数类
class MyCompare
{
public:
    // const 是好习惯，表示这个比较过程不会修改类本身
    bool operator()(int v1, int v2) const
    {
        return v1 > v2; // 降序：让大的排前面
    }
};

// 2. 使用时在尖括号里指定
void test()
{
    // set<类型, 排序规则>
    set<int, MyCompare> s2;
    
    s2.insert(10);
    s2.insert(50);
    s2.insert(30);

    // 输出: 50 30 10
    for(auto it = s2.begin(); it != s2.end(); it++) {
        cout << *it << " ";
    }
}
```
## 自定义数据类型的排序规则
```
#include<iostream>
using namespace std;
#include<set>
#include<string>

//set容器排序，存放自定义数据类型

class Person
{
public:
    Person(string name, int age)
    {
        this->m_Name = name;
        this->m_Age = age;
    }
    string m_Name;
    int m_Age;
};

class ComparePerson
{
public:
    bool operator()(const Person &p1, const Person &p2) const
    {
        //按照年龄 降序
        return p1.m_Age > p2.m_Age;
    }
};

void test01()
{
    //自定义的数据类型都会指定排序规则
    set<Person, ComparePerson>s1;
    Person p1("刘备", 24);
    Person p2("关羽", 28);
    Person p3("张飞", 25);
    Person p4("赵云", 21);
    s1.insert(p1);
    s1.insert(p2);
    s1.insert(p3);
    s1.insert(p4);
    for(auto it = s1.begin(); it != s1.end(); it++)
    {
        cout << "姓名：" << it->m_Name << " " << "年龄：" << it->m_Age << endl;
    }
    cout << endl;
}

int main()
{
    test01();
    return 0;
}
```