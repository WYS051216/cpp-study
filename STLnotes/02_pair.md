# [STL] Pair：成对出现的数据结构

## 1. 核心本质
- **是什么**：`pair` 是一个标准库提供的**结构体模板**，用于将两个数据（类型可以不同）合成一个整体。
- **头文件**：`#include <utility>` (通常包含 `<iostream>` 或 `<string>` 时会被自动带进来，但标准写法是 `<utility>`)。
- **应用场景**：
    1.  **函数返回两个值**：比如 `set.insert()` 就返回了 `{迭代器, 是否成功}`。
    2.  **Map 容器**：`map` 中的每个元素都是 `key` 和 `value` 组成的 `pair`。

## 2. 常用操作速查

| 操作 | 语法示例 | 说明 |
| :--- | :--- | :--- |
| **创建方式 1** | `pair<string, int> p("Tom", 20);` | **构造函数法**。显式指定类型，清晰明了。 |
| **创建方式 2** | `make_pair("Jerry", 30);` | **辅助函数法**。自动推导类型，写起来快，不用写尖括号里的类型。 |
| **取第一个值** | `p.first` | 访问第一个数据（通常是 Key）。 |
| **取第二个值** | `p.second` | 访问第二个数据（通常是 Value）。 |

---

## 3. 标准实战代码

```cpp
#include <iostream>
#include <string>
#include <utility> // 标准头文件
using namespace std;

void test_pair()
{
    // --- 方式 1: 显式构造 ---
    // 适合：定义变量时，明确知道类型
    pair<string, int> p1("Tom", 20);
    
    cout << "方式1访问 -> 姓名: " << p1.first << ", 年龄: " << p1.second << endl;

    // --- 方式 2: make_pair ---
    // 适合：临时传参，或者不想写很长的类型声明时
    pair<string, int> p2 = make_pair("Jerry", 30);
    
    // 配合 auto 使用更加丝滑 (C++11)
    auto p3 = make_pair("Spike", 5); 

    cout << "方式2访问 -> 姓名: " << p2.first << ", 年龄: " << p2.second << endl;
    cout << "Auto推导  -> 姓名: " << p3.first << ", 年龄: " << p3.second << endl;
}

int main()
{
    test_pair();
    return 0;
}