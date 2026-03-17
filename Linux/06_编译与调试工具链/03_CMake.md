## 1. 前言与语法特性
**CMake** 是一个跨平台的安装/编译工具，可以用简单的语句来描述所有平台的安装和编译过程。它是绝大多数 C++ 开源项目和企业级项目的标配。

### 基本语法格式
* **参数传递**：参数使用括弧 `()` 括起。
* **参数分隔**：参数之间使用空格或分号 `;` 分开。
* **大小写规则**：指令是大小写无关的（推荐全小写），但**参数和变量是大小写相关的**。
* **变量取值**：使用 `${}` 方式取值；但是在 `IF` 控制语句中，直接使用变量名即可。

### 基础语法代码示例
```cmake
# 定义一个变量
set(SERVER_NAME "DouDizhuServer")

# 使用 ${} 输出变量
message("当前构建的服务端是：${SERVER_NAME}")

# 在 IF 语句中直接使用变量名 SERVER_NAME
if(SERVER_NAME)
    message("服务名称已定义！")
endif()
```

---

## 2. 核心指令 (CMakeLists.txt 必备)

| 指令 | 语法与说明 |
| :--- | :--- |
| `cmake_minimum_required` | **指定 CMake 的最小版本要求**<br>`cmake_minimum_required(VERSION versionNumber)` |
| `project` | **定义工程名称，并指定支持的语言**<br>`project(projectname [CXX] [C])` |
| `set` | **显式地定义变量**<br>`set(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])` |
| `include_directories` | **向工程添加特定的头文件搜索路径** (相当于 g++ 的 `-I`)<br>`include_directories([AFTER \| BEFORE] [SYSTEM] dir1 dir2...)` |
| `link_directories` | **向工程添加特定的库文件搜索路径** (相当于 g++ 的 `-L`)<br>`link_directories(dir1 dir2...)` |
| `add_library` | **生成库文件 (补充)**<br>`add_library(libname [SHARED \| STATIC \| MODULE] source1 source2 ...)`<br>*(注：`SHARED` 生成动态库 `.so`，`STATIC` 生成静态库 `.a`)* |
| `add_compile_options` | **添加编译参数 (补充)**<br>`add_compile_options(<option> ...)`<br>*(示例：`add_compile_options(-std=c++11 -Wall -O2)`)* |
| `add_executable` | **生成可执行文件 (补充)**<br>`add_executable(exename source1 source2 ...)` |
| `target_link_libraries` | **为 target 添加需要链接的共享库 (补充)** (相当于 g++ 的 `-l`)<br>`target_link_libraries(target library1 library2 ...)` |
| `add_subdirectory` | **向当前工程添加存放源文件的子目录 (补充)**<br>`add_subdirectory(source_dir [binary_dir])`<br>*(注：告诉 CMake 去这个子目录里找它的 `CMakeLists.txt` 并执行)* |
| `aux_source_directory` | **自动发现目录下的所有源代码文件 (补充)**<br>`aux_source_directory(dir var)`<br>*(示例：`aux_source_directory(. DIR_SRCS)` 将当前目录下所有源文件存入变量 `DIR_SRCS`)* |

---

## 3. CMake 常用变量与示例

CMake 提供了很多内置变量来控制编译行为：

* **`CMAKE_C_FLAGS`**：设置 C 语言编译选项
* **`CMAKE_CXX_FLAGS`**：设置 C++ 语言编译选项（即 g++ 编译选项）
* **`CMAKE_BUILD_TYPE`**：设置编译类型（`Debug` 还是 `Release`）

**常用变量补充与实战示例：**
```cmake
# 1. 设置 C++ 编译选项 (追加 C++11 支持和 Wall 警告)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

# 2. 设置编译类型为 Debug (为了配合 GDB 调试，自动带上 -g 参数)
set(CMAKE_BUILD_TYPE Debug)
# 如果发布上线，则改为 Release (自动带上 -O3 优化参数，不带调试信息)
# set(CMAKE_BUILD_TYPE Release)

# 3. 指定可执行文件的输出路径 (统一放到工程目录下的 bin 文件夹)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 4. 指定库文件的输出路径 (统一放到 lib 文件夹)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
```

---

## 4. CMake 编译工程与构建方式

### CMake 目录结构
项目主目录必须存在一个 `CMakeLists.txt`（注意 `Lists` 有 `s`）文件。
当项目很大时，有两种方式设置编译规则：
1. **多 CMakeLists 模式**：包含源文件的子文件夹也包含自己的 `CMakeLists.txt`，主目录通过 `add_subdirectory` 添加子目录。
2. **单 CMakeLists 模式**：子文件夹没有 `CMakeLists.txt`，所有的编译规则全部写在主目录的配置文件中。

### 标准编译流程
1. 手动编写 `CMakeLists.txt`
2. 执行命令 `cmake PATH` 生成 `Makefile`（PATH 是顶层 CMakeLists.txt 所在的目录）
3. 执行命令 `make` 进行自动化编译

### 内部构建 vs 外部构建 (核心重点)

* **内部构建 (In-source build)**：
  直接在当前工程目录下敲 `cmake .`。
  * **缺点**：会产生一大堆中间件（如 `CMakeCache.txt`, `Makefile` 等），这些中间件和你的 C++ 源码混在一起，显得极其杂乱无章，非常不推荐！
  
* **外部构建 (Out-of-source build) —— 业界绝对标准**：
  将所有的编译输出文件放到一个独立的文件夹中，保持源码目录的纯洁。
  * **标准执行步骤**：
    ```bash
    mkdir build       # 1. 在工程根目录下创建一个独立的 build 文件夹
    cd build          # 2. 进入该文件夹
    cmake ..          # 3. 编译上层的 CMakeLists.txt，生成 Makefile
    make              # 4. 执行编译
    ```