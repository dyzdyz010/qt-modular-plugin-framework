# Orders Plugin - MPF 插件模板

这是一个完整的 MPF (Modular Plugin Framework) 插件示例，展示了如何创建一个功能完整的业务插件。

## 目录结构

```
orders/
├── CMakeLists.txt          # CMake 构建配置
├── CMakePresets.json       # CMake 预设配置
├── orders_plugin.json      # 插件元数据
├── README.md               # 本文档
├── include/
│   ├── orders_plugin.h     # 插件主类
│   ├── orders_service.h    # 业务服务
│   └── order_model.h       # 数据模型
├── src/
│   ├── orders_plugin.cpp   # 插件主类实现
│   ├── orders_service.cpp  # 业务服务实现
│   └── order_model.cpp     # 数据模型实现
└── qml/
    ├── OrdersPage.qml      # 主页面
    ├── OrderDetailPage.qml # 详情页
    ├── OrderCard.qml       # 列表卡片组件
    └── CreateOrderDialog.qml # 创建对话框
```

## 插件元数据文件 (orders_plugin.json)

```json
{
    "id": "com.yourco.orders",        // 插件唯一标识符
    "name": "Orders Plugin",          // 显示名称
    "version": "1.0.0",               // 版本号
    "description": "描述文本",         // 描述
    "vendor": "YourCo",               // 开发者/公司
    "requires": [                     // 依赖列表
        {"type": "service", "id": "INavigation", "min": "1.0"}
    ],
    "provides": ["OrdersService"],    // 提供的服务
    "qmlModules": ["YourCo.Orders"],  // QML 模块 URI
    "priority": 10,                   // 加载优先级 (数字越小越先)
    "loadOnStartup": true             // 是否启动时加载
}
```

### 字段说明

| 字段 | 说明 | 示例 |
|------|------|------|
| `id` | 插件唯一标识符，格式 `com.company.plugin` | `com.yourco.orders` |
| `name` | 用户可见的插件名称 | `Orders Plugin` |
| `version` | 语义化版本号 | `1.0.0` |
| `requires` | 依赖的服务或插件 | 见下表 |
| `provides` | 此插件提供的服务 | `["OrdersService"]` |
| `qmlModules` | QML 模块 URI 列表 | `["YourCo.Orders"]` |
| `priority` | 加载顺序 | `10` (越小越先) |

### requires 依赖格式

```json
// 服务依赖
{"type": "service", "id": "INavigation", "min": "1.0", "optional": false}

// 插件依赖
{"type": "plugin", "id": "com.yourco.core", "min": "1.0", "optional": true}
```

## 创建新插件的步骤

### 1. 复制模板

```bash
cp -r plugins/orders plugins/your-plugin
cd plugins/your-plugin
```

### 2. 重命名文件

- `orders_plugin.json` → `your_plugin.json`
- `orders_plugin.h` → `your_plugin.h`
- `orders_plugin.cpp` → `your_plugin.cpp`
- `orders_service.h/cpp` → `your_service.h/cpp`

### 3. 修改 CMakeLists.txt

```cmake
# 修改项目名称
project(your-plugin VERSION 1.0.0 LANGUAGES CXX)

# 修改源文件列表
add_library(your-plugin SHARED
    src/your_plugin.cpp
    src/your_service.cpp
    ...
)

# 修改 QML 模块 URI
qt_add_qml_module(your-plugin
    URI YourCo.YourModule
    ...
)
```

### 4. 修改元数据

编辑 `your_plugin.json`，更新所有字段。

### 5. 修改 C++ 代码

- 更新命名空间
- 更新类名
- 更新 `Q_PLUGIN_METADATA` 的 FILE 路径
- 更新 `qmlModuleUri()` 返回值
- 实现业务逻辑

### 6. 修改 QML 文件

- 更新 import 语句中的模块 URI
- 实现 UI 逻辑

## 使用的 MPF 功能

### 1. 服务注册表 (ServiceRegistry)

```cpp
// 获取系统服务
auto* nav = m_registry->get<mpf::INavigation>();
auto* menu = m_registry->get<mpf::IMenu>();
auto* settings = m_registry->get<mpf::ISettings>();
```

### 2. 导航服务 (INavigation)

```cpp
// 注册路由
nav->registerRoute("orders", "qrc:/YourCo/Orders/qml/OrdersPage.qml");

// QML 中导航
Navigation.push("orders/detail", {orderId: "123"})
```

### 3. 菜单服务 (IMenu)

```cpp
mpf::MenuItem item;
item.id = "orders";
item.label = tr("Orders");
item.icon = "📦";
item.route = "orders";
item.order = 10;
menu->registerItem(item);

// 设置徽章
menu->setBadge("orders", "5");
```

### 4. 日志系统

```cpp
MPF_LOG_DEBUG("Tag", "Debug message");
MPF_LOG_INFO("Tag", "Info message");
MPF_LOG_WARNING("Tag", "Warning message");
MPF_LOG_ERROR("Tag", "Error message");
```

### 5. QML 类型注册

```cpp
// 单例
qmlRegisterSingletonInstance("YourCo.Orders", 1, 0, "OrdersService", service);

// 可实例化类型
qmlRegisterType<OrderModel>("YourCo.Orders", 1, 0, "OrderModel");
```

## 使用的 MPF 库

### HTTP 客户端 (mpf-http-client)

```cpp
#include <mpf/http/http_client.h>

// 创建客户端
auto client = std::make_unique<mpf::http::HttpClient>(this);

// 发送请求
mpf::http::HttpClient::RequestOptions options;
options.timeoutMs = 10000;
options.headers["Authorization"] = "Bearer token";

QNetworkReply* reply = client->get(QUrl("https://api.example.com"), options);
```

### UI 组件库 (mpf-ui-components)

QML 中使用：

```qml
import MPF.Components 1.0

// 卡片
MPFCard {
    title: "标题"
    cardColor: "#FFFFFF"
    clickable: true
    onClicked: console.log("clicked")
}

// 按钮
MPFButton {
    text: "确定"
    type: "primary"  // primary, secondary, success, warning, danger, ghost
    size: "medium"   // small, medium, large
    loading: false
}

// 状态徽章
StatusBadge {
    status: "pending"  // 自动映射颜色
}

// 对话框
MPFDialog {
    title: "确认"
    type: "danger"
    onAccepted: doSomething()
}

// 输入框
MPFTextField {
    label: "用户名"
    placeholder: "请输入"
    required: true
    errorMessage: "不能为空"
}
```

## 构建和运行

### 命令行构建

```bash
cd plugins/orders
cmake --preset default
cmake --build --preset default
```

### Qt Creator 构建

1. 打开 `CMakeLists.txt`
2. 选择 Kit
3. 构建项目

### 配置插件路径

编辑 `host/config/paths.json`:

```json
{
    "pluginPath": "path/to/plugins/directory",
    "qmlPath": "",
    "extraQmlPaths": [
        "path/to/plugin/qml",
        "C:/Qt/MPF/qml"
    ]
}
```

## 常见问题

### 1. 插件加载失败 "找不到指定的模块"

这通常是因为插件依赖的 DLL 不在搜索路径中。确保：
- `C:/Qt/MPF/bin` 在 PATH 环境变量中
- 或者复制依赖 DLL 到应用程序目录

### 2. QML 组件找不到

确保 QML 导入路径正确配置：
- 在 `paths.json` 的 `extraQmlPaths` 中添加路径
- 或者在代码中使用 `engine->addImportPath()`

### 3. 服务获取失败

检查：
- 插件元数据中的 `requires` 是否正确声明依赖
- 依赖的服务是否已注册（检查加载顺序）
