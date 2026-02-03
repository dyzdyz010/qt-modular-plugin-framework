# 插件开发

插件通过 SDK 提供的接口与服务进行交互，使用 CMake 的 `find_package` 查找 SDK 与库。

## 构建示例

```powershell
cmake -S orders -B build/orders -DCMAKE_PREFIX_PATH=C:/Qt/MPF
cmake --build build/orders --config Release
```

## CMake 使用

```cmake
find_package(MPF REQUIRED)
target_link_libraries(your-plugin PRIVATE MPF::foundation-sdk)
```

需要一次性引入 SDK 和可用公共库时，可改用：

```cmake
find_package(MPF REQUIRED)
target_link_libraries(your-plugin PRIVATE MPF::all)
```

如需使用 HTTP Client：

```cmake
find_package(MPFHttpClient REQUIRED)
target_link_libraries(your-plugin PRIVATE MPF::http-client)
```

## 运行期注意

插件为动态库，运行时需要放到 host 的 `plugins/` 目录。
若插件依赖 SDK 动态库，请确保 DLL 已同步到 host 运行目录。
