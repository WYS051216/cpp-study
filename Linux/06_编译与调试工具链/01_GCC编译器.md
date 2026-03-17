# 01_GCC / G++ 编译器核心笔记

## 1. 编译的四个核心阶段 (底层原理)
平时我们敲 `g++ main.cpp -o main` 是一键完成的，但实际上编译器在后台做了 4 步。我们可以用特定的选项让它在某一步“停下来”：

* **预处理 (Preprocessing)**：展开头文件（把 `#include` 里的代码复制进来）、替换宏定义、删除注释。
  * **指令**：`-E` 选项指示编译器仅对输入文件进行预处理。
  * **示例**：`g++ -E test.cpp -o test.i` （生成 `.i` 文件，依然是 C++ 代码，但体积变大）
* **编译 (Compilation)**：检查语法错误，将 C++ 代码翻译成底层的汇编语言。
  * **指令**：`-S` 选项告诉 g++ 产生汇编语言文件后停止编译。
  * **示例**：`g++ -S test.i -o test.s` （生成 `.s` 汇编文件）
* **汇编 (Assembly)**：将汇编语言纯翻译成 CPU 能看懂的 `0` 和 `1` 二进制机器码。
  * **指令**：`-c` 选项告诉 g++ 仅把源代码编译为机器语言的目标代码。
  * **示例**：`g++ -c test.s -o test.o` （生成 `.o` 目标文件，用文本编辑器打开是一堆乱码）
* **链接 (Linking)**：将我们生成的 `.o` 文件，和系统自带的库文件（如 `iostream` 的实现）拼接打包在一起，变成最终可运行的程序。
  * **指令**：`-o` 选项用来指定最终产生的可执行文件的文件名。
  * **示例**：`g++ test.o -o test`

---

## 2. G++ 重要编译参数速查表

| 参数 | 作用说明 |
| :--- | :--- |
| `-g` | 编译带调试信息的可执行文件（**重要**：如果不加这个，后面用 GDB 无法调试） |
| `-O[n]` | 优化源代码。如 `-O2` 会让程序跑得更快，发布上线时常用。 |
| `-l` (小写L) | 指定程序要链接的**库名**（例如使用多线程库 `-lpthread`） |
| `-L` | 指定程序要链接的**库文件所在的目录路径** |
| `-I` (大写i) | 指定**头文件**（`.h` / `.hpp`）的搜索目录 |
| `-Wall` | 打印出所有警告信息（**强烈建议带上**，能避开很多潜在 Bug） |
| `-w` | 关闭所有警告信息（不推荐） |
| `-std=c++11` | 设置 C++ 编译标准为 C++11（也可换成 `c++14`, `c++17`） |
| `-o` | 指定输出文件的名字 |
| `-D` | 在编译时定义宏（例如 `g++ -DDEBUG main.cpp`，代码里就可以用到 `DEBUG` 宏） |

> **PS**: 在终端使用 `man g++` 命令可以查看 gcc/g++ 的英文使用手册。

---

## 3. 多文件编译示例与 `tree` 命令

当我们做项目时，代码通常分布在不同的文件夹里。可以通过终端输入 `tree .` 来查看目录结构：

```text
.
├── include        # 存放头文件
│   └── Swap.h
├── main.cpp       # 主程序入口
└── src            # 存放源代码实现
    └── Swap.cpp
```

### 示例代码

**1. `include/Swap.h`**
```cpp
#pragma once
void swap(int& a, int& b);
```

**2. `src/Swap.cpp`**
```cpp
#include "Swap.h"

void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
```

**3. `main.cpp`**
```cpp
#include <iostream>
#include "Swap.h" // 只要指明了 -Iinclude，这里就能直接找到

using namespace std;

int main() {
    int a = 10, b = 20;
    swap(a, b);
    cout << "a: " << a << ", b: " << b << endl;
    return 0;
}
```

### 基础编译命令
如果 `.cpp` 和 `.h` 文件不在同一个目录，在编译时一定要加 `-I` 选项指明头文件的路径：

* **最简单的多文件编译**：
  `g++ main.cpp src/Swap.cpp -Iinclude -o a.out`
* **企业级标准编译（增加常用参数）**：
  `g++ main.cpp src/Swap.cpp -Iinclude -std=c++11 -O2 -Wall -g -o b.out`

---

## 4. 静态库与动态库的生成与链接

### 4.1 静态库 (`.a`)
静态库在链接时，会把库里的代码**直接完整复制**一份到最终的可执行文件里。程序运行不再需要依赖原来的库文件，但可执行文件体积会变大。

1. **汇编，生成 `.o` 文件**：
   `g++ src/Swap.cpp -c -Iinclude -o src/Swap.o`
2. **归档，生成静态库 `libswap.a`**：
   `ar rs src/libswap.a src/Swap.o`
3. **链接静态库，生成可执行文件**：
   `g++ main.cpp -Iinclude -Lsrc -lswap -o staticmain`

### 4.2 动态库 (`.so`)
动态库（共享库）在链接时，**不会**把代码复制进可执行文件，而是只留下一个“标记”。程序在运行时才会去系统里寻找。多个程序可以共享同一个动态库，极大地节省了内存。

1. **生成动态库 `libswap.so`**：
   `g++ src/Swap.cpp -Iinclude -fPIC -shared -o src/libswap.so`
2. **链接动态库，生成可执行文件**：
   `g++ main.cpp -Iinclude -Lsrc -lswap -o sharemain`

---

## 5. 运行可执行文件与补充避坑指南

### 运行静态库链接的程序
直接运行即可：
```bash
./staticmain
```

### 运行动态库链接的程序 (⚠️ 易踩坑)
如果你直接输入 `./sharemain` 运行，大概率会报错找不到 `libswap.so`。
* **原因**：Linux 默认只去 `/lib` 和 `/usr/lib` 等系统目录找动态库，不知道你的库在 `src` 文件夹里。
* **解决办法**：临时指定动态库的环境变量 `LD_LIBRARY_PATH`。

```bash
# 将当前目录下的 src 文件夹临时加入动态库搜索路径
export LD_LIBRARY_PATH=./src:$LD_LIBRARY_PATH

# 再次运行，成功！
./sharemain
```