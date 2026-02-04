# MPF UI Components

提供跨插件共享的 QML 组件库和 C++ 工具类。

## 目录结构

```
ui-components/
├── CMakeLists.txt
├── cmake/
│   └── MPFUIComponentsConfig.cmake.in
├── src/
│   ├── ui_components_global.h      # 导出宏
│   ├── color_helper.h              # 颜色工具类（QML_ELEMENT）
│   ├── color_helper.cpp
│   ├── input_validator.h           # 输入验证器（QML_ELEMENT）
│   └── input_validator.cpp
├── qml/
│   ├── MPFCard.qml
│   ├── MPFButton.qml
│   └── ...
└── README.md
```

## 安装

```powershell
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/mingw_64" -DCMAKE_INSTALL_PREFIX=C:/Qt/MPF
cmake --build build --config Release
cmake --install build --config Release
```

## 在插件中使用

### CMake 配置

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
find_package(MPFUIComponents REQUIRED)

target_link_libraries(your-plugin PRIVATE
    Qt6::Qml
    Qt6::Quick
    MPF::ui-components
)
```

### QML 中导入

```qml
import MPF.Components 1.0

// 使用组件
MPFCard {
    title: "订单信息"
    
    StatusBadge {
        status: "pending"
    }
}
```

## QML 组件

| 组件 | 说明 |
|------|------|
| `MPFCard` | 通用卡片容器 |
| `MPFButton` | 统一风格按钮 |
| `MPFIconButton` | 图标按钮 |
| `MPFDialog` | 模态对话框 |
| `MPFTextField` | 文本输入框 |
| `StatusBadge` | 状态徽章 |
| `MPFLoadingIndicator` | 加载指示器 |

### MPFCard 示例

```qml
MPFCard {
    title: "用户信息"
    subtitle: "基本资料"
    clickable: true
    
    onClicked: navigateToDetail()
    
    Column {
        spacing: 8
        Label { text: "姓名：张三" }
        Label { text: "邮箱：zhangsan@example.com" }
    }
}
```

### MPFButton 示例

```qml
Row {
    spacing: 8
    
    MPFButton {
        text: "确定"
        type: "primary"
    }
    
    MPFButton {
        text: "取消"
        type: "ghost"
    }
    
    MPFButton {
        text: "删除"
        type: "danger"
        loading: isDeleting
    }
}
```

### MPFDialog 示例

```qml
MPFDialog {
    id: confirmDialog
    title: "确认删除"
    type: "danger"
    
    content: Label {
        text: "确定要删除这条记录吗？此操作不可恢复。"
        wrapMode: Text.WordWrap
    }
    
    onAccepted: {
        deleteRecord()
        close()
    }
}

// 打开对话框
Button {
    text: "删除"
    onClicked: confirmDialog.open()
}
```

### MPFTextField 示例

```qml
MPFTextField {
    id: emailField
    label: "邮箱"
    placeholder: "请输入邮箱地址"
    required: true
    errorMessage: emailError
    
    onEditingFinished: {
        let result = InputValidator.validateEmail(text)
        emailError = result.valid ? "" : result.message
    }
}
```

## C++ 工具类

### ColorHelper - 颜色工具

```qml
import MPF.Components 1.0

Rectangle {
    // 使颜色变亮 20%
    color: ColorHelper.lighten("#2196F3", 0.2)
}

Rectangle {
    // 根据背景自动选择文字颜色
    property color bgColor: "#1E88E5"
    color: bgColor
    
    Text {
        color: ColorHelper.contrastColor(parent.color)
    }
}

Rectangle {
    // 获取状态对应的颜色
    color: ColorHelper.statusBackgroundColor("success")
}
```

**可用方法：**

| 方法 | 说明 |
|------|------|
| `lighten(color, factor)` | 使颜色变亮 |
| `darken(color, factor)` | 使颜色变暗 |
| `withAlpha(color, alpha)` | 设置透明度 |
| `blend(color1, color2, ratio)` | 混合两种颜色 |
| `contrastColor(bgColor)` | 获取对比色（黑/白） |
| `isDark(color)` | 判断是否为深色 |
| `statusColor(status)` | 获取状态颜色 |
| `statusBackgroundColor(status)` | 获取状态背景色 |

### InputValidator - 输入验证器

```qml
import MPF.Components 1.0

TextField {
    id: emailInput
    
    onTextChanged: {
        let result = InputValidator.validateEmail(text)
        if (!result.valid) {
            errorLabel.text = result.message
        }
    }
}

TextField {
    id: passwordInput
    
    onTextChanged: {
        let result = InputValidator.validatePassword(text)
        strengthLabel.text = result.strength  // weak/medium/strong
    }
}
```

**可用方法：**

| 方法 | 说明 |
|------|------|
| `validateEmail(email)` | 验证邮箱格式 |
| `validatePhone(phone)` | 验证手机号（中国） |
| `validateRequired(value, fieldName)` | 验证必填 |
| `validateLength(value, min, max)` | 验证长度 |
| `validateRange(value, min, max)` | 验证数值范围 |
| `validatePattern(value, regex, msg)` | 正则验证 |
| `validatePassword(password)` | 验证密码强度 |
| `validateMatch(v1, v2, fieldName)` | 验证两值相等 |
| `validateUrl(url)` | 验证 URL |
| `validateInteger(value)` | 验证整数 |
| `validateDecimal(value, decimals)` | 验证小数 |

所有验证方法返回：`{valid: bool, message: string, ...其他字段}`

## 运行时部署

确保以下文件部署到正确位置：

```
app/
├── bin/
│   ├── app.exe
│   └── mpf-ui-components.dll
└── qml/
    └── MPF/
        └── Components/
            ├── qmldir
            └── *.qml
```
