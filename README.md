# Qt Modular Plugin Framework

独立构建、运行时组合的 Qt6 插件框架。

## 项目结构

```
qt-modular-plugin-framework/
├── foundation-sdk/    # 核心 SDK（必须先构建安装）
├── host/              # 宿主应用
└── plugins/
    └── orders/        # 示例插件
```

## 快速开始 (Windows + Qt Creator)

### 前置条件
- Qt 6.5+ (推荐 6.8)
- CMake 3.21+
- MinGW 或 MSVC

### 步骤 1: 构建并安装 foundation-sdk

**必须先完成这一步！** host 和 plugin 都依赖 SDK。

```powershell
cd foundation-sdk
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build . --config Release
cmake --install .
```

安装后检查 `C:/Qt/MPF/lib/cmake/MPF/MPFConfig.cmake` 是否存在。

### 步骤 2: 在 Qt Creator 中配置 host

1. 打开 `host/CMakeLists.txt`
2. 在 Kit 设置中添加 CMake 参数：
   ```
   -DCMAKE_PREFIX_PATH=C:/Qt/MPF
   ```
   或者在 Qt Creator: Projects → Build Settings → CMake → Add → `CMAKE_PREFIX_PATH` = `C:/Qt/MPF`

3. 构建运行

### 步骤 3: 构建插件

```powershell
cd plugins/orders
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/MPF
cmake --build . --config Release
```

### 步骤 4: 运行

将插件复制到 host 的 plugins 目录：
```powershell
copy plugins\orders\build\plugins\orders-plugin.dll host\build\plugins\
```

运行 `host/build/bin/mpf-host.exe`

---

## Linux 构建

```bash
# 1. SDK
cd foundation-sdk && mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
make -j$(nproc) && make install

# 2. Host
cd ../../host && mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/.local
make -j$(nproc)

# 3. Plugin
cd ../../plugins/orders && mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/.local
make -j$(nproc)

# 4. Run
cp build/plugins/liborders-plugin.so ../../host/build/plugins/
cd ../../host/build && ./bin/mpf-host
```

---

## 架构说明

### foundation-sdk
- 定义插件接口 (`IPlugin`, `INavigation`, `IMenu`, etc.)
- 提供 `ServiceRegistry` 服务注册中心
- 安装后生成 `MPF::foundation-sdk` CMake target

### host
- 加载并管理插件
- 提供核心服务实现
- QML shell 界面

### plugins
- 实现 `IPlugin` 接口
- 通过 `ServiceRegistry` 获取/注册服务
- 自带 QML 模块

## 常见问题

### Q: CMake 找不到 MPF
确保先安装了 foundation-sdk，并设置 `CMAKE_PREFIX_PATH` 指向安装目录。

### Q: Qt Creator 配置
在 Projects → Build Settings → CMake 中添加：
- `CMAKE_PREFIX_PATH` = `C:/Qt/MPF` (或你的安装路径)

### Q: 运行时找不到插件
确保插件 .dll/.so 在 host 可执行文件同级的 `plugins/` 目录下。
