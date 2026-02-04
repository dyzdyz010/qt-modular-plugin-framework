import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 统一风格按钮组件
 * 
 * 使用方式:
 * MPFButton {
 *     text: "确定"
 *     type: "primary"  // primary, secondary, success, warning, danger, ghost
 *     onClicked: console.log("clicked")
 * }
 */
Button {
    id: root
    
    // 按钮类型
    property string type: "primary"  // primary, secondary, success, warning, danger, ghost
    
    // 尺寸
    property string size: "medium"  // small, medium, large
    
    // 图标
    property string iconSource: ""
    property bool iconOnly: false
    
    // 加载状态
    property bool loading: false
    
    // 圆角
    property int buttonRadius: 6
    
    enabled: !loading
    
    implicitWidth: iconOnly ? implicitHeight : Math.max(implicitContentWidth + leftPadding + rightPadding, minWidth)
    implicitHeight: {
        switch (size) {
        case "small": return 28
        case "large": return 44
        default: return 36
        }
    }
    
    property int minWidth: {
        switch (size) {
        case "small": return 64
        case "large": return 96
        default: return 80
        }
    }
    
    leftPadding: {
        switch (size) {
        case "small": return 12
        case "large": return 24
        default: return 16
        }
    }
    rightPadding: leftPadding
    
    font.pixelSize: {
        switch (size) {
        case "small": return 12
        case "large": return 16
        default: return 14
        }
    }
    
    // 颜色配置
    property color primaryColor: "#2196F3"
    property color successColor: "#4CAF50"
    property color warningColor: "#FF9800"
    property color dangerColor: "#F44336"
    property color secondaryColor: "#757575"
    
    property color buttonColor: {
        switch (type) {
        case "primary": return primaryColor
        case "success": return successColor
        case "warning": return warningColor
        case "danger": return dangerColor
        case "secondary": return secondaryColor
        case "ghost": return "transparent"
        default: return primaryColor
        }
    }
    
    property color textColor: {
        if (type === "ghost") return primaryColor
        if (type === "secondary") return "#FFFFFF"
        return "#FFFFFF"
    }
    
    property color hoverColor: Qt.darker(buttonColor, 1.1)
    property color pressedColor: Qt.darker(buttonColor, 1.2)
    property color disabledColor: "#BDBDBD"
    
    contentItem: RowLayout {
        spacing: 6
        
        // 加载指示器
        Item {
            visible: root.loading
            Layout.preferredWidth: 16
            Layout.preferredHeight: 16
            
            Rectangle {
                id: loadingIndicator
                anchors.centerIn: parent
                width: 14
                height: 14
                radius: 7
                color: "transparent"
                border.width: 2
                border.color: root.textColor
                opacity: 0.3
            }
            
            Rectangle {
                anchors.centerIn: parent
                width: 14
                height: 14
                radius: 7
                color: "transparent"
                border.width: 2
                border.color: "transparent"
                
                Rectangle {
                    width: 4
                    height: 4
                    radius: 2
                    color: root.textColor
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                }
                
                RotationAnimation on rotation {
                    running: root.loading
                    from: 0
                    to: 360
                    duration: 1000
                    loops: Animation.Infinite
                }
            }
        }
        
        // 图标
        Image {
            visible: root.iconSource.length > 0 && !root.loading
            source: root.iconSource
            Layout.preferredWidth: 16
            Layout.preferredHeight: 16
            fillMode: Image.PreserveAspectFit
        }
        
        // 文字
        Text {
            visible: !root.iconOnly
            text: root.text
            font: root.font
            color: root.enabled ? root.textColor : "#FFFFFF"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    
    background: Rectangle {
        radius: root.buttonRadius
        color: {
            if (!root.enabled) return root.disabledColor
            if (root.pressed) return root.pressedColor
            if (root.hovered) return root.hoverColor
            return root.buttonColor
        }
        border.width: root.type === "ghost" ? 1 : 0
        border.color: root.type === "ghost" ? root.primaryColor : "transparent"
        
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
}
