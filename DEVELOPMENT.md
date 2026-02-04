# MPF 开发者合作指南

本文档指导 MPF (Modular Plugin Framework) 各模块的开发者如何独立开发、测试和协作。

## 目录

1. [项目架构](#项目架构)
2. [环境准备](#环境准备)
3. [角色与职责](#角色与职责)
4. [SDK 开发者指南](#sdk-开发者指南)
5. [库开发者指南](#库开发者指南)
6. [Host 开发者指南](#host-开发者指南)
7. [插件开发者指南](#插件开发者指南)
8. [测试指南](#测试指南)
9. [发布流程](#发布流程)
10. [常见问题](#常见问题)

---

## 项目架构

```
qt-modular-plugin-framework/
├── foundation-sdk/     # 核心 SDK - 接口定义 (所有人依赖)
├── libs/               # 可选库
│   ├── http-client/    # HTTP 客户端
│   └── ui-components/  # UI 组件库
├── host/               # 宿主应用 (壳)
├── plugins/            # 插件
│   ├── orders/         # 示例插件
│   └── rules/          # 示例插件
└── scripts/            # 构建脚本
```

### 依赖关系

```
                    ┌─────────────────┐
                    │ foundation-sdk  │
                    │   (接口定义)    │
                    └────────┬────────┘
                             │
         ┌───────────────────┼───────────────────┐
         │                   │                   │
         ▼                   ▼                   ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
│   http-client   │ │  ui-components  │ │      host       │
│     (库)        │ │      (库)       │ │    (宿主)       │
└────────┬────────┘ └────────┬────────┘ └────────┬────────┘
         │                   │                   │
         └───────────────────┼───────────────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │    plugins      │
                    │   (业务插件)    │
                    └─────────────────┘
```

---

## 环境准备

### 系统要求

| 平台 | 编译器 | Qt 版本 | CMake |
|------|--------|---------|-------|
| Windows | MinGW 13.1+ / MSVC 2022 | 6.5+ (推荐 6.8) | 3.21+ |
| Linux | GCC 11+ / Clang 14+ | 6.5+ | 3.21+ |
| macOS | Clang 14+ | 6.5+ | 3.21+ |

### 工具安装

```bash
# Linux (Ubuntu/Debian)
sudo apt install build-essential ninja-build cmake git

# macOS
brew install ninja cmake

# Windows - 使用 Qt Online Installer 安装 MinGW, Ninja, CMake
```

### 环境变量配置

**Linux/macOS:**
```bash
# 编辑 ~/.bashrc 或 ~/.zshrc
export QT_DIR="$HOME/Qt/6.8.3/gcc_64"      # Qt 安装路径
export MPF_SDK="$HOME/mpf-sdk"              # MPF SDK 安装路径

# 或使用提供的脚本
source scripts/setup-env.sh --qt-dir /opt/qt6 --sdk-path ~/mpf-sdk
```

**Windows (PowerShell):**
```powershell
# 临时设置
$env:QT_DIR = "C:\Qt\6.8.3\mingw_64"
$env:MPF_SDK = "C:\Qt\MPF"

# 或使用提供的脚本
. .\scripts\setup-env.ps1 -QtDir "C:\Qt\6.8.3\mingw_64" -SdkPath "C:\Qt\MPF"

# 永久设置 - 添加到系统环境变量
[Environment]::SetEnvironmentVariable("QT_DIR", "C:\Qt\6.8.3\mingw_64", "User")
[Environment]::SetEnvironmentVariable("MPF_SDK", "C:\Qt\MPF", "User")
```

---

## 角色与职责

| 角色 | 职责 | 依赖 | 产出 |
|------|------|------|------|
| **SDK 开发者** | 定义核心接口、服务注册机制 | Qt | `MPF::foundation-sdk` |
| **库开发者** | 开发可选功能库 | SDK | `MPF::mpf-http-client` 等 |
| **Host 开发者** | 实现宿主应用、服务实现 | SDK, 库 | `mpf-host` 可执行文件 |
| **插件开发者** | 开发业务插件 | SDK, 库 | `libxxx-plugin.so/dll` |

---

## SDK 开发者指南

SDK 定义了所有模块共享的接口，是框架的核心。

### 职责

1. 定义 `IPlugin` 接口和生命周期规范
2. 定义服务接口 (`INavigation`, `IMenu`, `ISettings`, `ITheme`, `IEventBus`, `ILogger`)
3. 提供 `ServiceRegistry` 抽象类
4. 版本管理和 API 兼容性

### 开发流程

```bash
cd foundation-sdk

# 配置
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="$QT_DIR" \
    -DCMAKE_INSTALL_PREFIX="$MPF_SDK" \
    -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build build

# 安装到 SDK 目录
cmake --install build
```

### 接口变更规范

1. **新增接口**: 增加 `apiVersion()` 返回值
2. **修改接口**: 保持向后兼容，弃用旧方法而非删除
3. **破坏性变更**: 需要主版本号升级，提前通知所有开发者

```cpp
// 示例: 接口版本管理
class INavigation {
public:
    // v1 方法
    virtual bool push(const QString& route) = 0;
    
    // v2 新增方法
    virtual bool push(const QString& route, const QVariantMap& params) = 0;
    
    static constexpr int apiVersion() { return 2; }
};
```

---

## 库开发者指南

库开发者负责开发可选功能模块。

### 开发流程

```bash
cd libs/http-client  # 或 ui-components

# 配置 (依赖 SDK)
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="$QT_DIR;$MPF_SDK" \
    -DCMAKE_INSTALL_PREFIX="$MPF_SDK" \
    -DCMAKE_BUILD_TYPE=Release

# 构建
cmake --build build

# 安装
cmake --install build
```

### 导出规范

1. 头文件安装到 `$MPF_SDK/include/mpf/xxx/`
2. 库文件安装到 `$MPF_SDK/lib/`
3. QML 模块安装到 `$MPF_SDK/qml/`
4. CMake 配置安装到 `$MPF_SDK/lib/cmake/`

### 示例: 创建新库

```cmake
# libs/my-lib/CMakeLists.txt
cmake_minimum_required(VERSION 3.21)
project(mpf-my-lib VERSION 1.0.0)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(MPF REQUIRED)

add_library(mpf-my-lib SHARED
    src/my_lib.cpp
)

target_link_libraries(mpf-my-lib
    PRIVATE Qt6::Core
    PUBLIC MPF::foundation-sdk
)

# 安装配置...
```

---

## Host 开发者指南

Host 开发者实现宿主应用和核心服务。

### 职责

1. 实现 `ServiceRegistry` 具体类
2. 实现所有服务接口 (`NavigationService`, `MenuService` 等)
3. 实现 `PluginManager` 和 `PluginLoader`
4. 提供 QML Shell

### 开发流程

```bash
cd host

# 配置
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="$QT_DIR;$MPF_SDK" \
    -DCMAKE_BUILD_TYPE=Debug

# 构建
cmake --build build

# 运行 (需要配置 paths.json)
./build/bin/mpf-host
```

### 配置文件

`host/config/paths.json` 支持:

```json
{
  "pluginPath": "../plugins",           // 相对路径
  "pluginPath": "/absolute/path",       // 绝对路径
  "pluginPath": "${MPF_SDK}/plugins",   // 环境变量展开
  "pluginPath": "%USERPROFILE%/plugins" // Windows 环境变量
}
```

---

## 插件开发者指南

这是大多数开发者的角色 - 开发业务功能插件。

### 快速开始

1. **复制模板**
   ```bash
   cp -r plugins/orders plugins/my-plugin
   cd plugins/my-plugin
   ```

2. **重命名文件和类**
   ```bash
   # 重命名文件
   mv src/orders_plugin.cpp src/my_plugin.cpp
   mv include/orders_plugin.h include/my_plugin.h
   # ... 其他文件
   ```

3. **修改 CMakeLists.txt**
   - 项目名称
   - 源文件列表
   - QML 模块 URI

4. **修改 JSON 元数据**
   - `my_plugin.json` 中的 id, name, version 等

5. **配置环境变量**
   ```bash
   export QT_DIR=/path/to/qt
   export MPF_SDK=/path/to/mpf-sdk
   ```

6. **构建**
   ```bash
   # Linux
   cmake --preset linux-debug
   cmake --build build/linux-debug

   # Windows
   cmake --preset windows-debug
   cmake --build build/windows-debug
   ```

### CMake Presets 说明

插件使用 CMake Presets 简化构建配置:

| Preset | 平台 | 构建类型 |
|--------|------|----------|
| `linux-debug` | Linux | Debug |
| `linux-release` | Linux | Release |
| `windows-debug` | Windows | Debug |
| `windows-release` | Windows | Release |
| `macos-debug` | macOS | Debug |
| `macos-release` | macOS | Release |

**前提条件**: 设置 `QT_DIR` 和 `MPF_SDK` 环境变量。

### 独立测试

插件可以独立于其他插件进行测试:

1. **配置 paths.json** 只加载你的插件:
   ```json
   {
     "pluginPath": "/path/to/my-plugin/build/plugins",
     "extraQmlPaths": [
       "/path/to/my-plugin/build/qml",
       "${MPF_SDK}/qml"
     ]
   }
   ```

2. **运行 Host**:
   ```bash
   cd host/build/bin
   ./mpf-host
   ```

3. **查看日志** 确认插件加载成功:
   ```
   Discovered plugin: "com.myco.myplugin"
   Loaded plugin: "com.myco.myplugin"
   [INFO] MyPlugin Initialized successfully
   ```

### 调试技巧

```cpp
// 使用 MPF 日志系统
#include <mpf/logger.h>

MPF_LOG_DEBUG("MyPlugin", "Debug message");
MPF_LOG_INFO("MyPlugin", "Info message");
MPF_LOG_WARNING("MyPlugin", "Warning: %s", qPrintable(msg));
MPF_LOG_ERROR("MyPlugin", "Error occurred");
```

---

## 测试指南

### 单元测试

```bash
cd host/tests
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$QT_DIR;$MPF_SDK"
cmake --build build
ctest --test-dir build
```

### 集成测试

1. 构建所有模块
2. 配置 `paths.json` 加载测试插件
3. 运行 Host
4. 验证功能

### 自动化测试脚本

```bash
# 全量构建并测试
./scripts/build-all.sh --qt-dir /opt/qt6 --sdk-path ~/mpf-sdk
./scripts/run-tests.sh  # (待实现)
```

---

## 发布流程

### SDK/库发布

1. 更新版本号 (`CMakeLists.txt`)
2. 更新 CHANGELOG
3. 构建 Release 版本
4. 打包安装目录
5. 创建 Git Tag

### 插件发布

1. 确保依赖版本兼容
2. 更新 `xxx_plugin.json` 版本
3. 构建 Release 版本
4. 测试插件加载
5. 打包 `libxxx-plugin.so/dll` + `qml/` 目录

---

## 常见问题

### Q: CMake 找不到 MPF SDK

**A:** 确保设置了 `CMAKE_PREFIX_PATH` 或 `MPF_SDK` 环境变量:
```bash
export MPF_SDK=/path/to/mpf-sdk
cmake -DCMAKE_PREFIX_PATH="$QT_DIR;$MPF_SDK" ...
```

### Q: 插件加载失败 "找不到指定的模块"

**A:** DLL 依赖问题。确保 SDK bin 目录在 PATH 中:
```bash
# Linux
export LD_LIBRARY_PATH="$MPF_SDK/lib:$LD_LIBRARY_PATH"

# Windows
$env:PATH = "$env:MPF_SDK\bin;$env:PATH"
```

### Q: QML 模块找不到

**A:** 检查 `paths.json` 中的 `extraQmlPaths`:
```json
{
  "extraQmlPaths": [
    "${MPF_SDK}/qml",
    "/path/to/plugin/build/qml"
  ]
}
```

### Q: 如何在不同平台间共享代码?

**A:** 使用环境变量和 CMake Presets:
- 设置 `QT_DIR` 和 `MPF_SDK` 环境变量
- 使用平台特定的 preset: `cmake --preset linux-debug`
- `paths.json` 使用 `${VAR}` 或 `%VAR%` 语法

### Q: 如何调试插件?

**A:** 
1. 使用 Debug 构建
2. 在 Qt Creator 中设置断点
3. 或使用 `gdb` / `lldb`:
   ```bash
   gdb --args ./mpf-host
   (gdb) break MyPlugin::initialize
   (gdb) run
   ```

---

## 附录: 一键构建

### Linux/macOS

```bash
# 设置环境
source scripts/setup-env.sh --qt-dir /opt/qt6 --sdk-path ~/mpf-sdk

# 全量构建
./scripts/build-all.sh
```

### Windows

```powershell
# 设置环境
. .\scripts\setup-env.ps1 -QtDir "C:\Qt\6.8.3\mingw_64" -SdkPath "C:\Qt\MPF"

# 全量构建
.\scripts\build-all.ps1 -QtPath $env:QT_DIR -SdkPath $env:MPF_SDK
```

---

## 联系方式

- **Issues**: GitHub Issues
- **讨论**: GitHub Discussions
- **文档**: 本仓库 `docs/` 目录

---

*最后更新: 2026-02-04*
