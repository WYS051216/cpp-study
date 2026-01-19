# [STL] Map：键值对容器

## 1. 核心简介

* **①基本概念**：
* `map` 中所有元素都是 `pair`。
* `pair` 中第一个元素为 **key (键值)**，起索引作用；第二个元素为 **value (实值)**。
* 所有元素都会根据 **key** 自动排序。


* **②本质**：
* 属于**关联式容器**。
* 底层结构是用 **二叉树 (红黑树)** 实现的。


* **优点**：
* 可以根据 key 值快速找到 value 值。



### 🔎 Map vs Multimap

* **`map`**：不允许容器中有重复的 key 值元素。
* **`multimap`**：允许容器中有重复的 key 值元素。

---

## 2. 构造和赋值

### 函数原型

* **构造**：
* `map<T1, T2> mp;` // 默认构造
* `map(const map &mp);` // 拷贝构造


* **赋值**：
* `map& operator=(const map &mp);` // 重载等号操作符



### 💻 代码示例

```cpp
#include<iostream>
#include<map>
using namespace std;

void printMap(map<int, int> &m)
{
    for(auto it = m.begin(); it != m.end(); it++)
    {
        cout << "key值: " << it->first << " value: " << it->second << endl;
    }
    cout << endl;
}

void test01()
{
    map<int, int> m;
    // 插入数据 (C++11 列表初始化)
    m.insert({1, 10});
    m.insert({2, 20});
    m.insert({3, 30});
    m.insert({4, 40});

    cout << "--- 原始 m ---" << endl;
    printMap(m); 

    // 拷贝构造
    map<int, int> m2(m);
    cout << "--- 拷贝构造 m2 ---" << endl;
    printMap(m2);

    // 赋值
    map<int, int> m3;
    m3 = m2;
    cout << "--- 赋值 m3 ---" << endl;
    printMap(m3);
}

int main()
{
    test01();
    return 0;
}

```

---

## 3. 大小和交换

### 函数原型

* `size()`：返回容器中元素的数目。
* `empty()`：判断容器是否为空。
* `swap(st)`：交换两个集合容器。

### 💻 代码示例

```cpp
#include<iostream>
#include<map>
using namespace std;

void printMap(map<int, int> &m)
{
    for(auto it = m.begin(); it != m.end(); it++)
    {
        cout << "key值: " << it->first << " value: " << it->second << endl;
    }
    cout << endl;
}

// 1. 大小测试
void test_size()
{
    map<int, int> m;
    m.insert({1, 10});
    m.insert({2, 20});
    m.insert({3, 30});
    
    if(m.empty())
    {
        cout << "m为空" << endl;
    }
    else 
    {
        cout << "m不为空" << endl;
        cout << "m的大小为：" << m.size() << endl;
    }
}

// 2. 交换测试
void test_swap()
{
    map<int, int> m1;
    m1.insert({1, 10});
    m1.insert({2, 20});

    map<int, int> m2;
    m2.insert({10, 100});
    m2.insert({20, 200});
    m2.insert({30, 300});

    cout << "交换前：" << endl;
    printMap(m1);
    printMap(m2);

    cout << "交换后：" << endl;
    m1.swap(m2); // 核心操作
    printMap(m1);
    printMap(m2);
}

int main()
{
    test_size();
    test_swap();
    return 0;
}

```

---

## 4. 插入和删除

### 函数原型

* `insert(elem)`：在容器中插入元素。
* `clear()`：清除所有元素。
* `erase(pos)`：删除 `pos` 迭代器所指的元素，返回下一个元素的迭代器。
* `erase(beg, end)`：删除区间 `[beg, end)` 的所有元素。
* `erase(key)`：删除容器中键值为 `key` 的元素（常用）。

### 💻 代码示例

```cpp
#include<iostream>
#include<map>
using namespace std;

void printMap(map<int, int> &m)
{
    for(auto it = m.begin(); it != m.end(); it++)
    {
        cout << "key值: " << it->first << " value: " << it->second << endl;
    }
    cout << endl;
}

void test01()
{
    map<int, int> m;
    
    // --- 四种插入方式 ---
    // 1. 列表初始化 (推荐)
    m.insert({1, 10}); 
    // 2. make_pair (常用)
    m.insert(make_pair(2, 20));
    // 3. value_type
    m.insert(map<int, int>::value_type(3, 30));
    // 4. pair构造
    m.insert(pair<int, int>(4, 40));
    
    // 5. []操作符 (不建议用于纯插入，适合用于访问或修改)
    // 注意：如果key不存在，它会创建一个默认值0
    m[5] = 50; 
    
    cout << "插入后：" << endl;
    printMap(m);

    // --- 删除操作 ---
    
    // 1. 删除迭代器 (删除第一个)
    m.erase(m.begin());
    printMap(m);
    
    // 2. 按 Key 删除 (常用)
    m.erase(3); // 删掉 key 为 3 的元素
    printMap(m);
    
    // 3. 清空 / 区间删除
    // m.erase(m.begin(), m.end()); 等同于 m.clear()
    m.clear();
    printMap(m);
}

int main()
{
    test01();
    return 0;
}

```

---

## 5. 查找和统计

### 函数原型

* **`find(key)`**：查找 `key` 是否存在。若存在，返回该键的元素的迭代器；若不存在，返回 `map.end()`。
* **`count(key)`**：统计 `key` 的元素个数（对于 map，结果只有 0 或 1）。

### 💻 代码示例

```cpp
#include<iostream>
#include<map>
using namespace std;

void test01()
{
    map<int, int> m;
    m.insert({1, 10});
    m.insert({2, 20});
    m.insert({3, 30});

    // --- 查找 ---
    auto pos = m.find(3); // 返回的是迭代器
    
    if(pos != m.end())
    {
        cout << "查到了元素 key = " << pos->first << " value = " << pos->second << endl; 
    }
    else 
    {
        cout << "未找到" << endl;
    }

    // --- 统计 ---
    // Map 不允许重复 key，所以结果要么是 0 要么是 1
    int num = m.count(3);
    cout << "num = " << num << endl;
}

int main()
{
    test01();
    return 0;
}

```

---

## 6. Map 容器排序 (自定义规则)

* **默认规则**：Map 默认按照 Key 值**从小到大**排序。
* **自定义规则**：利用**仿函数**改变排序规则（如从大到小）。

### 💻 代码示例

```cpp
#include<iostream>
#include<map>
using namespace std;

// 仿函数：指定排序规则
class MyCompare
{
public:
    // const 修饰符是良好的编程习惯
    bool operator()(int v1, int v2) const
    {
        // 降序：让大的排前面
        return v1 > v2;
    }
};

void printMap(map<int, int, MyCompare> &m)
{
    for(auto it = m.begin(); it != m.end(); it++)
    {
        cout << "key值: " << it->first << " value: " << it->second << endl;
    }
    cout << endl;
}

void test01()
{
    // 在定义容器时指定排序规则
    map<int, int, MyCompare> m;
    
    m.insert({1, 10});
    m.insert({2, 20});
    m.insert({3, 30});
    m.insert({4, 40});
    m.insert({5, 50}); 
    
    cout << "按 Key 降序排列：" << endl;
    printMap(m);
}

int main()
{
    test01();
    return 0;
}

```