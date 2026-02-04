import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 统一风格对话框组件
 * 
 * 使用方式:
 * MPFDialog {
 *     title: "确认删除"
 *     
 *     content: Column {
 *         Text { text: "确定要删除这条记录吗？" }
 *     }
 *     
 *     onAccepted: deleteRecord()
 * }
 */
Dialog {
    id: root
    
    // 对话框类型
    property string type: "default"  // default, info, success, warning, danger
    
    // 按钮文本
    property string acceptText: qsTr("确定")
    property string rejectText: qsTr("取消")
    
    // 是否显示关闭按钮
    property bool showCloseButton: true
    
    // 是否显示底部按钮
    property bool showFooter: true
    
    // 内容区域
    property alias content: contentContainer.data
    
    // 加载状态
    property bool loading: false
    
    anchors.centerIn: parent
    modal: true
    
    implicitWidth: 400
    
    // 圆角
    property int dialogRadius: 8
    
    // 颜色
    property color headerColor: {
        switch (type) {
        case "info": return "#E3F2FD"
        case "success": return "#E8F5E9"
        case "warning": return "#FFF3E0"
        case "danger": return "#FFEBEE"
        default: return "#FAFAFA"
        }
    }
    
    property color accentColor: {
        switch (type) {
        case "info": return "#2196F3"
        case "success": return "#4CAF50"
        case "warning": return "#FF9800"
        case "danger": return "#F44336"
        default: return "#2196F3"
        }
    }
    
    background: Rectangle {
        radius: dialogRadius
        color: "#FFFFFF"
        
        layer.enabled: true
        layer.effect: Item {}
    }
    
    header: Rectangle {
        implicitHeight: 56
        color: headerColor
        radius: dialogRadius
        
        // 底部方角
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: dialogRadius
            color: headerColor
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 20
            anchors.rightMargin: 12
            spacing: 12
            
            // 类型图标
            Label {
                visible: root.type !== "default"
                font.pixelSize: 20
                text: {
                    switch (root.type) {
                    case "info": return "ℹ️"
                    case "success": return "✅"
                    case "warning": return "⚠️"
                    case "danger": return "❌"
                    default: return ""
                    }
                }
            }
            
            Label {
                text: root.title
                font.pixelSize: 16
                font.bold: true
                color: "#212121"
                Layout.fillWidth: true
                elide: Text.ElideRight
            }
            
            // 关闭按钮
            Button {
                visible: root.showCloseButton
                flat: true
                text: "✕"
                font.pixelSize: 14
                implicitWidth: 32
                implicitHeight: 32
                
                onClicked: root.reject()
                
                background: Rectangle {
                    radius: 4
                    color: parent.hovered ? "#E0E0E0" : "transparent"
                }
            }
        }
        
        // 底部边线
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 1
            color: "#E0E0E0"
        }
    }
    
    contentItem: Item {
        id: contentContainer
        implicitHeight: childrenRect.height
    }
    
    footer: Rectangle {
        visible: showFooter
        implicitHeight: 60
        color: "#FAFAFA"
        radius: dialogRadius
        
        // 顶部方角
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: dialogRadius
            color: "#FAFAFA"
        }
        
        // 顶部边线
        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#E0E0E0"
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12
            
            Item { Layout.fillWidth: true }
            
            MPFButton {
                text: root.rejectText
                type: "ghost"
                enabled: !root.loading
                onClicked: root.reject()
            }
            
            MPFButton {
                text: root.acceptText
                type: root.type === "danger" ? "danger" : "primary"
                loading: root.loading
                onClicked: root.accept()
            }
        }
    }
    
    // 进入动画
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 150 }
        NumberAnimation { property: "scale"; from: 0.9; to: 1; duration: 150; easing.type: Easing.OutQuad }
    }
    
    // 退出动画
    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 100 }
    }
}
