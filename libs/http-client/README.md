# HTTP Client Library

Qt6 Network 封装的 HTTP 客户端库，当前默认构建为动态库，并导出 `MPF::http-client` 目标。

## 构建与安装

```powershell
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build build --config Release
cmake --install build --config Release
```

安装后会生成：
- 头文件：`<prefix>/include/mpf/http/http_client.h`
- 导入库/库文件：`<prefix>/lib`
- 运行时 DLL：`<prefix>/bin`
- CMake 包：`<prefix>/lib/cmake/MPFHttpClient`

## 使用（插件/Host）

```cmake
find_package(MPFHttpClient REQUIRED)
target_link_libraries(your-target PRIVATE MPF::http-client)
```

## 运行期

动态库需要随 host 一起部署。Windows 建议将 `<prefix>/bin/*.dll` 复制到 `host/build/bin`。
