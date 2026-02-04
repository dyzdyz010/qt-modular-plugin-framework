# MPF 快速入门指南

本指南介绍如何在新环境中从零开始构建和运行 MPF (Modular Plugin Framework) 项目。

## 目录

1. [环境准备](#1-环境准备)
2. [项目结构概览](#2-项目结构概览)
3. [构建 Foundation SDK](#3-构建-foundation-sdk)
4. [构建 MPF 库](#4-构建-mpf-库)
5. [构建 Host 应用](#5-构建-host-应用)
6. [构建插件](#6-构建插件)
7. [配置和运行](#7-配置和运行)
8. [故障排除](#8-故障排除)

---

## 1. 环境准备

### 1.1 系统要求

- **操作系统**: Windows 10/11, Linux, macOS
- **Qt 版本**: Qt 6.5+ (推荐 6.8.x)
- **编译器**: 
  - Windows: MinGW 13.1+ 或 MSVC 2022
  - Linux: GCC 11+ 或 Clang 14+
  - macOS: Clang 14+
- **CMake**: 3.21+
- **构建工具**: Ninja (推荐) 或 Make

### 1.2 安装 Qt

1. 下载 [Qt Online Installer](https://www.qt.io/download-qt-installer)
2. 安装时选择：
   - Qt 6.8.x (或最新稳定版)
   - MinGW 13.1.0 64-bit (Windows) 或对应编译器
   - Qt Quick/QML 组件
   - CMake 和 Ninja

### 1.3 验证环境

打开终端，验证工具可用：

```bash
# Windows PowerShell - 设置环境变量
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH

# 验证
cmake --version    # 应显示 3.21+
ninja --version    # 应显示版本号
g++ --version      # 应显示 GCC 13+
```

### 1.4 设置 SDK 安装目录

选择一个目录用于安装 MPF SDK（后续所有组件都会安装到这里）：

```bash
# 推荐目录
# Windows: C:\Qt\MPF
# Linux/macOS: ~/Qt/MPF 或 /opt/mpf
```

---

## 2. 项目结构概览

```
qt-modular-plugin-framework/
├── foundation-sdk/     # 基础 SDK（接口定义）
├── libs/               # 可选库
│   ├── http-client/    # HTTP 客户端库
│   └── ui-components/  # UI 组件库
├── host/               # 宿主应用（壳子）
├── plugins/            # 插件
│   ├── orders/         # 示例插件
│   └── README.md
├── scripts/            # 构建脚本
├── QUICKSTART.md       # 本文档
└── README.md
```

### 构建顺序

```
1. foundation-sdk  →  2. libs (http-client, ui-components)  →  3. host  →  4. plugins
       ↓                              ↓                            ↓            ↓
   安装到 SDK 目录 ────────────────────────────────────────────────────────────────→
```

---

## 3. 构建 Foundation SDK

Foundation SDK 提供插件接口、服务注册等核心功能。

### 3.1 配置

```bash
cd foundation-sdk

# Windows
cmake -B build -G Ninja ^
    -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64" ^
    -DCMAKE_INSTALL_PREFIX="C:/Qt/MPF" ^
    -DCMAKE_BUILD_TYPE=Release

# Linux/macOS
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="$HOME/Qt/6.8.3/gcc_64" \
    -DCMAKE_INSTALL_PREFIX="$HOME/Qt/MPF" \
    -DCMAKE_BUILD_TYPE=Release
```

### 3.2 构建和安装

```bash
cmake --build build
cmake --install build
```

### 3.3 验证安装

```bash
# 检查安装的文件
ls C:/Qt/MPF/include/mpf/           # 应有 iplugin.h 等头文件
ls C:/Qt/MPF/lib/cmake/MPF/         # 应有 MPFConfig.cmake
```

---

## 4. 构建 MPF 库

MPF 提供两个可选库，可以根据需要选择构建。

### 4.1 构建 HTTP 客户端库

```bash
cd libs/http-client

# 配置
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" \
    -DCMAKE_INSTALL_PREFIX="C:/Qt/MPF" \
    -DCMAKE_BUILD_TYPE=Release

# 构建和安装
cmake --build build
cmake --install build
```

验证：
```bash
ls C:/Qt/MPF/include/mpf/http/      # 应有 http_client.h
ls C:/Qt/MPF/bin/                   # 应有 libmpf-http-client.dll
```

### 4.2 构建 UI 组件库

```bash
cd libs/ui-components

# 配置
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" \
    -DCMAKE_INSTALL_PREFIX="C:/Qt/MPF" \
    -DCMAKE_BUILD_TYPE=Release

# 构建和安装
cmake --build build
cmake --install build
```

验证：
```bash
ls C:/Qt/MPF/qml/MPF/Components/    # 应有 MPFCard.qml, MPFButton.qml 等
ls C:/Qt/MPF/bin/                   # 应有 mpf-ui-components.dll
```

### 4.3 使用打包脚本（可选）

项目提供了一键打包脚本：

```bash
cd scripts

# 打包所有库
./package-libs.ps1 -QtPath "C:/Qt/6.8.3/mingw_64" -InstallPath "C:/Qt/MPF"
```

---

## 5. 构建 Host 应用

Host 是宿主应用（壳子），负责加载和管理插件。

### 5.1 使用 Qt Creator（推荐）

1. 打开 Qt Creator
2. 文件 → 打开文件或项目 → 选择 `host/CMakeLists.txt`
3. 配置 Kit（选择 Qt 6.8.x MinGW 64-bit）
4. 在 CMake 配置中添加：
   ```
   CMAKE_PREFIX_PATH = C:/Qt/6.8.3/mingw_64;C:/Qt/MPF
   ```
5. 点击"配置项目"
6. 构建 → 构建项目

### 5.2 使用命令行

```bash
cd host

# 配置
cmake -B build -G Ninja \
    -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" \
    -DCMAKE_BUILD_TYPE=Debug

# 构建
cmake --build build
```

### 5.3 构建产物

构建完成后，目录结构如下：

```
host/build/
├── bin/
│   └── mpf-host.exe        # 可执行文件
├── config/
│   └── paths.json          # 路径配置文件
├── plugins/                # 插件目录（空）
└── qml/
    └── MPF/Host/           # Host QML 模块
```

---

## 6. 构建插件

### 6.1 使用 CMake Presets（推荐）

插件目录包含预配置的 CMakePresets.json：

```bash
cd plugins/orders

# 配置（使用预设）
cmake --preset default

# 构建
cmake --build --preset default
```

### 6.2 使用 Qt Creator

1. 打开 `plugins/orders/CMakeLists.txt`
2. 配置 CMake 变量：
   ```
   CMAKE_PREFIX_PATH = C:/Qt/6.8.3/mingw_64;C:/Qt/MPF
   ```
3. 构建项目

### 6.3 构建产物

```
plugins/orders/build/default/
├── plugins/
│   └── liborders-plugin.dll    # 插件 DLL
└── qml/
    └── YourCo/Orders/          # 插件 QML 模块
```

---

## 7. 配置和运行

### 7.1 配置插件路径

编辑 `host/config/paths.json`（源文件和构建目录都要改）：

```json
{
    "pluginPath": "C:/path/to/plugins/orders/build/default/plugins",
    "qmlPath": "",
    "extraQmlPaths": [
        "C:/path/to/plugins/orders/build/default/qml",
        "C:/Qt/MPF/qml"
    ]
}
```

**重要**：
- `pluginPath`: 指向插件 DLL 所在目录
- `extraQmlPaths`: 包含插件 QML 模块和 SDK QML 组件的路径

### 7.2 配置环境变量（Windows）

插件依赖 SDK 中的 DLL，需要确保能找到它们：

**方法 A：修改 PATH（临时）**
```powershell
$env:PATH = "C:\Qt\MPF\bin;" + $env:PATH
```

**方法 B：修改 PATH（永久）**
1. 打开系统属性 → 环境变量
2. 编辑 Path，添加 `C:\Qt\MPF\bin`

**方法 C：在 Qt Creator 中配置**
1. 项目 → 运行 → 环境
2. 添加 PATH 变量，值为 `C:\Qt\MPF\bin;${PATH}`

### 7.3 运行应用

**从 Qt Creator**：
- 点击运行按钮

**从命令行**：
```bash
# Windows
cd host/build/bin
./mpf-host.exe

# 或者设置环境后运行
$env:PATH = "C:\Qt\MPF\bin;C:\Qt\6.8.3\mingw_64\bin;" + $env:PATH
./mpf-host.exe
```

### 7.4 验证运行

成功运行后，控制台应显示：

```
Plugin path: C:/path/to/plugins
QML path: ...
Added SDK QML import path: "C:/Qt/MPF/qml"
Added SDK bin to PATH: "C:/Qt/MPF/bin"
Discovered plugin: "com.yourco.orders"
Discovered 1 plugins
Loaded plugin: "com.yourco.orders"
```

---

## 8. 故障排除

### 8.1 CMake 找不到 Qt

**错误**：`Could not find a package configuration file provided by "Qt6"`

**解决**：
```bash
# 确保 CMAKE_PREFIX_PATH 包含 Qt 安装路径
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64" ...
```

### 8.2 CMake 找不到 MPF SDK

**错误**：`Could not find a package configuration file provided by "MPF"`

**解决**：
1. 确保已构建和安装 Foundation SDK
2. CMAKE_PREFIX_PATH 包含 SDK 安装路径：
   ```bash
   cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" ...
   ```

### 8.3 插件加载失败 "找不到指定的模块"

**错误**：`Cannot load library xxx.dll: 找不到指定的模块`

**原因**：插件依赖的 DLL（如 `libmpf-http-client.dll`）不在搜索路径中

**解决**：
1. 将 `C:\Qt\MPF\bin` 添加到 PATH 环境变量
2. 或将 DLL 复制到应用程序目录

### 8.4 QML 模块找不到

**错误**：`module "MPF.Components" is not installed`

**解决**：
1. 检查 `paths.json` 中的 `extraQmlPaths` 是否包含：
   - SDK QML 路径：`C:/Qt/MPF/qml`
   - 插件 QML 路径：`/path/to/plugin/build/qml`
2. 确保 UI 组件库已正确安装

### 8.5 插件目录为空

**错误**：`Plugin directory does not exist` 或 `Discovered 0 plugins`

**解决**：
1. 检查 `paths.json` 中的 `pluginPath` 是否正确
2. 确保插件已编译，DLL 存在于指定目录
3. 路径使用正斜杠 `/` 或双反斜杠 `\\`

### 8.6 编译器/工具链问题

**错误**：`CMAKE_CXX_COMPILER not set`

**解决**：
```bash
# Windows - 设置环境变量
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH
```

---

## 附录 A：完整构建脚本

### Windows PowerShell

```powershell
# build-all.ps1

param(
    [string]$QtPath = "C:/Qt/6.8.3/mingw_64",
    [string]$SdkPath = "C:/Qt/MPF",
    [string]$BuildType = "Release"
)

# 设置环境
$env:PATH = "C:\Qt\Tools\mingw1310_64\bin;C:\Qt\Tools\Ninja;C:\Qt\Tools\CMake_64\bin;" + $env:PATH

$ProjectRoot = Split-Path -Parent $PSScriptRoot

# 1. 构建 Foundation SDK
Write-Host "=== Building Foundation SDK ===" -ForegroundColor Green
Push-Location "$ProjectRoot/foundation-sdk"
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$QtPath" -DCMAKE_INSTALL_PREFIX="$SdkPath" -DCMAKE_BUILD_TYPE=$BuildType
cmake --build build
cmake --install build
Pop-Location

# 2. 构建 HTTP Client
Write-Host "=== Building HTTP Client ===" -ForegroundColor Green
Push-Location "$ProjectRoot/libs/http-client"
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$QtPath;$SdkPath" -DCMAKE_INSTALL_PREFIX="$SdkPath" -DCMAKE_BUILD_TYPE=$BuildType
cmake --build build
cmake --install build
Pop-Location

# 3. 构建 UI Components
Write-Host "=== Building UI Components ===" -ForegroundColor Green
Push-Location "$ProjectRoot/libs/ui-components"
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$QtPath;$SdkPath" -DCMAKE_INSTALL_PREFIX="$SdkPath" -DCMAKE_BUILD_TYPE=$BuildType
cmake --build build
cmake --install build
Pop-Location

# 4. 构建 Host
Write-Host "=== Building Host ===" -ForegroundColor Green
Push-Location "$ProjectRoot/host"
cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$QtPath;$SdkPath" -DCMAKE_BUILD_TYPE=Debug
cmake --build build
Pop-Location

# 5. 构建 Orders 插件
Write-Host "=== Building Orders Plugin ===" -ForegroundColor Green
Push-Location "$ProjectRoot/plugins/orders"
cmake --preset default
cmake --build --preset default
Pop-Location

Write-Host "=== Build Complete ===" -ForegroundColor Green
Write-Host "SDK installed to: $SdkPath"
Write-Host "Host built at: $ProjectRoot/host/build"
Write-Host "Plugin built at: $ProjectRoot/plugins/orders/build/default"
```

### 使用方法

```powershell
cd scripts
./build-all.ps1 -QtPath "C:/Qt/6.8.3/mingw_64" -SdkPath "C:/Qt/MPF"
```

---

## 附录 B：目录权限问题（Windows）

如果安装到 `C:\Qt\MPF` 遇到权限问题：

1. 以管理员身份创建目录：
   ```powershell
   New-Item -ItemType Directory -Path "C:\Qt\MPF" -Force
   ```

2. 或者使用用户目录：
   ```powershell
   $SdkPath = "$env:USERPROFILE\Qt\MPF"
   ```

---

## 附录 C：Linux/macOS 特别说明

### 设置库路径

```bash
# Linux
export LD_LIBRARY_PATH="$HOME/Qt/MPF/lib:$LD_LIBRARY_PATH"

# macOS
export DYLD_LIBRARY_PATH="$HOME/Qt/MPF/lib:$DYLD_LIBRARY_PATH"
```

### 插件扩展名

- Windows: `.dll`
- Linux: `.so`
- macOS: `.dylib`

paths.json 中的路径会自动适配。

---

## 附录 D：开发工作流

### 日常开发流程

1. **修改插件代码** → 重新构建插件 → 运行 Host 测试
2. **修改 SDK/库** → 重新安装到 SDK 目录 → 重新构建依赖它的组件
3. **修改 Host** → 重新构建 Host → 运行测试

### 推荐的 IDE 配置

**Qt Creator 多项目设置**：
1. 创建一个工作区
2. 添加所有项目（host, plugins/orders 等）
3. 设置统一的 CMAKE_PREFIX_PATH

这样可以方便地在项目间切换和调试。
