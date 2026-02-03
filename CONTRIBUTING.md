# 多人协作流程

本文件描述团队协作的推荐流程，适用于 SDK/Host/Plugins/Libs **多仓库**的开发与发布。

## 角色与职责
- **SDK 维护者**：维护 `foundation-sdk` 与公共 libs，负责安装包与 CMake 导出。
- **Host 开发者**：维护宿主应用与运行期目录结构。
- **插件开发者**：只依赖 SDK 前缀进行构建，输出插件产物供 Host 加载。

## 统一约定
- **每个组件独立仓库**：`foundation-sdk`、`host`、每个 `libs/<lib>`、每个 `plugins/<plugin>`。
- **协作入口是 SDK 前缀**：公共接口与库产物通过 SDK 前缀交付。
- **公共库只通过 `find_package` 引用**，不要写死相对路径。
- **运行期 DLL 由 Host 发布包携带**，插件只负责自身产物。
- **构建方式**：优先使用脚本或统一的 CMake 参数，避免手工参数不一致。

## 开发流程
1. **拉取代码**
   - SDK、Host、每个 Lib、每个 Plugin 都是独立仓库。
   - 从 `main` 拉最新代码，创建功能分支：`feature/<topic>`。
2. **安装/更新 SDK**
   - 在 SDK 仓库执行：
     ```powershell
     .\scripts\install-sdk.ps1 -SdkPrefix C:/Qt/MPF -Config Release -QtPrefix C:/Qt/6.8.3/mingw_64
     ```
3. **开发与本地构建**
   - Host/Plugin 使用：
     ```powershell
     -DCMAKE_PREFIX_PATH=C:/Qt/MPF
     ```
   - 插件产物输出到自身构建目录，Host 负责加载。
4. **运行期验证**
   - 同步 DLL 到 Host 运行目录（在 SDK 仓库执行）：
     ```powershell
     .\scripts\sync-runtime-dlls.ps1 -SdkPrefix C:/Qt/MPF -HostBuildDir <host-build-dir> -Config Release
     ```
   - 把插件复制到 `<host-build-dir>/plugins/`，再启动 host。

## Libs 打包与发布
- 单独打包库（在 SDK 仓库执行，或在对应 Lib 仓库复用脚本）：
  ```powershell
  .\scripts\package-libs.ps1 -Libs http-client -QtPrefix C:/Qt/6.8.3/mingw_64
  ```
- 批量打包所有库（SDK 仓库可用）：
  ```powershell
  .\scripts\package-libs.ps1 -All -QtPrefix C:/Qt/6.8.3/mingw_64
  ```

## 分支与提交规范
- **分支命名**：`feature/`、`fix/`、`chore/`、`docs/`
- **提交信息**：使用中文、Conventional Commit 风格
  - 示例：`feat: 新增订单查询接口`

## 代码评审与合并
- 提交 PR 前：
  - 确保 SDK 可安装、Host 能运行、插件可加载
  - 关键流程有自测说明
- 评审重点：
  - 破坏性变更（ABI、接口、目录结构、SDK 安装路径）
  - 运行期 DLL 部署风险
  - 文档与脚本同步更新

## 常见问题
- **Qt Creator 的 build 目录不同**：传真实 build 目录给脚本即可（例如 `host/build/Desktop_Qt_6_8_3_MinGW_64_bit-Debug`）。
- **找不到 DLL**：确认已执行 `sync-runtime-dlls.ps1` 并放到 host 运行目录。
