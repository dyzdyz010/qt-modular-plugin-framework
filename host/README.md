# Host 应用

负责加载插件并提供核心服务实现的宿主应用。

## 构建

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH=C:/Qt/MPF
cmake --build build --config Release
```

Qt Creator 默认会创建类似 `host/build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug` 的目录。

## 运行期目录结构

- 可执行文件：`<host-build-dir>/bin/mpf-host.exe`
- 插件目录：`<host-build-dir>/plugins/`
- QML 目录：`<host-build-dir>/qml/`
- 配置目录：`<host-build-dir>/config/`

## 插件加载

运行时会扫描 `plugins/` 目录下的插件动态库。

## 动态库部署

如果插件或 host 依赖 SDK 内的动态库，需要将 SDK 的 DLL 复制到运行目录。

```powershell
.\scripts\sync-runtime-dlls.ps1 -SdkPrefix C:/Qt/MPF -HostBuildDir <host-build-dir> -Config Release
```
