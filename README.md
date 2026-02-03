# Qt Modular Plugin Framework v2

独立构建、运行时组合的 Qt 插件框架。

## 项目结构

```
qt-plugin-framework-v2/
├── foundation-sdk/    # 核心 SDK（独立项目）
├── host/              # 宿主应用（独立项目）
└── plugins/
    └── orders/        # 订单插件（独立项目）
```

每个项目都可以**独立构建、独立版本管理、独立发布**。

## 构建顺序

### 1. 构建并安装 foundation-sdk

```bash
cd foundation-sdk
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build .
cmake --install .
```

Windows (MinGW):
```powershell
cd foundation-sdk
mkdir build; cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build .
cmake --install .
```

### 2. 构建 host

```bash
cd host
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/install
cmake --build .
```

Windows:
```powershell
cd host
mkdir build; cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/MPF;C:/Qt/6.8.3/mingw_64"
cmake --build .
```

### 3. 构建插件

```bash
cd plugins/orders
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/install
cmake --build .
```

### 4. 运行

把插件复制到 host 的 `plugins/` 目录，然后运行：

```bash
./host/build/bin/mpf-host
```

## 环境变量

也可以用环境变量指定 SDK 位置：

```bash
export MPF_DIR=/path/to/install/lib/cmake/MPF
```

## 目录约定

运行时目录结构：
```
app/
├── bin/mpf-host
├── plugins/
│   ├── liborders-plugin.so
│   └── libinventory-plugin.so
├── qml/
│   ├── MPF/Host/
│   └── YourCo/Orders/
└── config/
```

## 依赖

- Qt 6.5+ (Core, Gui, Qml, Quick)
- CMake 3.21+
- C++17 编译器
