# Foundation SDK

提供插件接口与基础服务定义的 SDK（header-only），通过 CMake install 导出 `MPF::foundation-sdk` 目标。

## 安装

```powershell
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build build --config Release
cmake --install build --config Release
```

## 使用（插件/Host）

```cmake
find_package(MPF REQUIRED)
target_link_libraries(your-target PRIVATE MPF::foundation-sdk)
```

如需一次性引入 SDK 及可用的公共库（例如 http-client），可以使用：

```cmake
find_package(MPF REQUIRED)
target_link_libraries(your-target PRIVATE MPF::bundle)
```

## 说明

- 仅包含头文件，不产出二进制库。
- 运行期不需要额外部署。
