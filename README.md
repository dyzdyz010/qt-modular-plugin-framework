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

> 实际协作可采用多仓库方式：`foundation-sdk`、`host`、每个 `libs/<lib>`、每个 `plugins/<plugin>` 分别独立开发。流程见 `CONTRIBUTING.md`。

## 快速开始 (Windows + Qt Creator)

### 前置条件
- Qt 6.5+ (推荐 6.8)
- CMake 3.21+
- MinGW 或 MSVC

### 步骤 1: 构建并安装 SDK（foundation + libs）

**必须先完成这一步！** host 和 plugin 的开发期都依赖 SDK。

```powershell
.\scripts\install-sdk.ps1 -SdkPrefix C:/Qt/MPF -Config Release
```

安装后检查：
- `C:/Qt/MPF/lib/cmake/MPF/MPFConfig.cmake`
- `C:/Qt/MPF/lib/cmake/MPFHttpClient/MPFHttpClientConfig.cmake`

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

### 步骤 4: 同步 SDK 动态库到 host 运行目录

```powershell
.\scripts\sync-runtime-dlls.ps1 -SdkPrefix C:/Qt/MPF -HostBuildDir <host-build-dir> -Config Release
```

### 步骤 5: 运行

将插件复制到 host 的 plugins 目录：
```powershell
copy plugins\orders\build\plugins\orders-plugin.dll <host-build-dir>\plugins\
```

运行 `<host-build-dir>\bin\mpf-host.exe`

---

## Linux 构建

```bash
# 1. SDK
cmake -S foundation-sdk -B build/sdk/foundation-sdk -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build/sdk/foundation-sdk -j$(nproc)
cmake --install build/sdk/foundation-sdk

cmake -S libs/http-client -B build/sdk/http-client -DCMAKE_INSTALL_PREFIX=$HOME/.local
cmake --build build/sdk/http-client -j$(nproc)
cmake --install build/sdk/http-client

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
cp $HOME/.local/lib/libmpf-http-client.so ../../host/build/bin/
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
### libs
- 例如 `http-client` 导出 `MPF::http-client`
- 默认构建为动态库，运行时需跟随 host 部署

### CMake 使用建议
- 常规：`find_package(MPF REQUIRED)` + `MPF::foundation-sdk`
- 一键：`find_package(MPF REQUIRED)` + `MPF::all`（会自动吸收可用的公共库，如 `MPF::http-client`）

## 常见问题

### Q: CMake 找不到 MPF
确保先安装了 foundation-sdk，并设置 `CMAKE_PREFIX_PATH` 指向安装目录。

### Q: Qt Creator 配置
在 Projects → Build Settings → CMake 中添加：
- `CMAKE_PREFIX_PATH` = `C:/Qt/MPF` (或你的安装路径)

### Q: 运行时找不到插件
确保插件 .dll/.so 在 host 可执行文件同级的 `plugins/` 目录下。

### Q: 运行时找不到 DLL
确保 SDK 中的 DLL 已复制到 host 的运行目录（例如 `host/build/<kit>/bin`）。Windows 可用 `scripts/sync-runtime-dlls.ps1`。

### Q: Qt Creator 的 build 目录不在 host/build
Qt Creator 默认会创建类似 `host/build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug` 的目录。使用脚本时，把该目录作为 `-HostBuildDir` 传入即可。
