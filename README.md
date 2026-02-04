# Qt Modular Plugin Framework (MPF)

一个现代化的 Qt6 模块化插件框架，支持独立构建、运行时组合。

## 特性

- **模块化设计**: SDK、库、Host、插件独立开发
- **服务注册中心**: 解耦的服务发现和依赖注入
- **QML 集成**: 完整的 QML 模块支持
- **可选库**: HTTP 客户端、UI 组件库
- **跨平台**: Windows、Linux、macOS

## 快速开始

详细步骤请参阅 **[QUICKSTART.md](QUICKSTART.md)**

### 一键构建（Windows）

```powershell
cd scripts
.\build-all.ps1 -QtPath "C:/Qt/6.8.3/mingw_64" -SdkPath "C:/Qt/MPF"
```

### 分步构建

```bash
# 1. 安装 SDK
cd foundation-sdk
cmake -B build -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build build && cmake --install build

# 2. 安装库
cd ../libs/http-client
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build build && cmake --install build

# 3. 构建 Host
cd ../../host
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF"
cmake --build build

# 4. 构建插件
cd ../plugins/orders
cmake --preset default && cmake --build --preset default
```

## 项目结构

```
qt-modular-plugin-framework/
├── foundation-sdk/     # 核心 SDK - 接口定义、服务注册
├── libs/               # 可选库
│   ├── http-client/    # HTTP 客户端库
│   └── ui-components/  # UI 组件库 (MPFCard, MPFButton...)
├── host/               # 宿主应用 - 插件加载、QML Shell
├── plugins/            # 插件
│   ├── orders/         # 订单管理示例插件
│   └── README.md
├── scripts/            # 构建脚本
├── QUICKSTART.md       # 快速入门指南
├── CONTRIBUTING.md     # 贡献指南
└── README.md           # 本文档
```

## 架构

```
┌─────────────────────────────────────────────────────────────────┐
│                         Host Application                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────────┐  │
│  │ QML Engine  │  │   Service   │  │    Plugin Manager       │  │
│  │   (Shell)   │  │  Registry   │  │  (Load/Init/Start/Stop) │  │
│  └─────────────┘  └─────────────┘  └─────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
         │                 │                      │
         │                 │                      ▼
         │                 │         ┌─────────────────────────┐
         │                 │         │       Plugins           │
         │                 │         │  ┌─────────┐ ┌───────┐  │
         ▼                 ▼         │  │ Orders  │ │ Users │  │
┌─────────────────────────────────┐  │  │ Plugin  │ │Plugin │  │
│        Foundation SDK           │  │  └─────────┘ └───────┘  │
│  IPlugin, INavigation, IMenu    │  └─────────────────────────┘
│  ISettings, ITheme, IEventBus   │               │
└─────────────────────────────────┘               │
         │                                        │
         ▼                                        ▼
┌─────────────────────────────────────────────────────────────────┐
│                        MPF Libraries                             │
│  ┌───────────────────┐  ┌──────────────────────────────────────┐│
│  │   http-client     │  │          ui-components               ││
│  │  HttpClient API   │  │  MPFCard, MPFButton, StatusBadge...  ││
│  └───────────────────┘  └──────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────┘
```

## 核心概念

### ServiceRegistry (服务注册中心)

```cpp
// 注册服务
registry->add<INavigation>(navigationService, "1.0", "host");

// 获取服务
auto* nav = registry->get<INavigation>();
nav->registerRoute("orders", "qrc:/YourCo/Orders/qml/OrdersPage.qml");
```

### IPlugin (插件接口)

```cpp
class MyPlugin : public IPlugin {
    bool initialize(ServiceRegistry* registry) override;
    bool start() override;
    void stop() override;
    QString qmlModuleUri() const override { return "MyApp.MyPlugin"; }
};
```

### QML 集成

```qml
import MyApp.MyPlugin 1.0
import MPF.Components 1.0

Page {
    MPFCard {
        title: "My Card"
        MPFButton {
            text: "Click Me"
            onClicked: MyService.doSomething()
        }
    }
}
```

## 可用服务

| 服务 | 接口 | 说明 |
|------|------|------|
| 导航 | `INavigation` | 页面路由和导航 |
| 菜单 | `IMenu` | 侧边栏菜单管理 |
| 设置 | `ISettings` | 配置存储 |
| 主题 | `ITheme` | 主题颜色和样式 |
| 事件总线 | `IEventBus` | 跨插件通信 |
| 日志 | `ILogger` | 统一日志记录 |

## 可用库

### HTTP 客户端 (mpf-http-client)

```cpp
#include <mpf/http/http_client.h>

auto client = std::make_unique<HttpClient>(this);
client->get(QUrl("https://api.example.com/data"));
```

### UI 组件库 (mpf-ui-components)

```qml
import MPF.Components 1.0

MPFCard { }           // 卡片容器
MPFButton { }         // 统一风格按钮
MPFDialog { }         // 对话框
MPFTextField { }      // 输入框
StatusBadge { }       // 状态徽章
MPFLoadingIndicator { } // 加载指示器
```

## 创建新插件

1. 复制 `plugins/orders` 模板
2. 重命名文件和类
3. 修改 `CMakeLists.txt` 和元数据
4. 实现业务逻辑

详见 [plugins/orders/README.md](plugins/orders/README.md)

## 配置

### paths.json

```json
{
    "pluginPath": "path/to/plugins",
    "qmlPath": "",
    "extraQmlPaths": [
        "path/to/plugin/qml",
        "C:/Qt/MPF/qml"
    ]
}
```

## 常见问题

### CMake 找不到 MPF

```bash
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64;C:/Qt/MPF" ...
```

### 插件加载失败 "找不到模块"

将 SDK bin 目录添加到 PATH：
```powershell
$env:PATH = "C:\Qt\MPF\bin;" + $env:PATH
```

### QML 组件找不到

确保 `paths.json` 中配置了正确的 `extraQmlPaths`。

更多问题请参阅 [QUICKSTART.md](QUICKSTART.md#8-故障排除)

## 许可证

MIT License

## 贡献

欢迎贡献！请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)
