import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 统一风格文本输入框
 * 
 * 使用方式:
 * MPFTextField {
 *     label: "邮箱"
 *     placeholder: "请输入邮箱地址"
 *     errorMessage: "邮箱格式不正确"
 *     required: true
 * }
 */
Column {
    id: root
    
    // 基本属性
    property alias text: textField.text
    property alias placeholder: textField.placeholderText
    property alias echoMode: textField.echoMode
    property alias readOnly: textField.readOnly
    property alias validator: textField.validator
    property alias inputMethodHints: textField.inputMethodHints
    property alias maximumLength: textField.maximumLength
    
    // 标签
    property string label: ""
    property bool required: false
    
    // 错误状态
    property string errorMessage: ""
    property bool hasError: errorMessage.length > 0
    
    // 帮助文本
    property string helperText: ""
    
    // 样式
    property int fieldRadius: 6
    property color borderColor: hasError ? "#F44336" : (textField.activeFocus ? "#2196F3" : "#E0E0E0")
    property color backgroundColor: "#FFFFFF"
    property int fieldHeight: 40
    
    // 前缀后缀
    property string prefix: ""
    property string suffix: ""
    
    // 信号
    signal accepted()
    signal editingFinished()
    
    spacing: 4
    
    // 标签行
    RowLayout {
        visible: label.length > 0
        width: parent.width
        spacing: 4
        
        Label {
            text: root.label
            font.pixelSize: 13
            color: hasError ? "#F44336" : "#424242"
        }
        
        Label {
            visible: root.required
            text: "*"
            font.pixelSize: 13
            color: "#F44336"
        }
        
        Item { Layout.fillWidth: true }
    }
    
    // 输入框容器
    Rectangle {
        width: parent.width
        height: fieldHeight
        radius: fieldRadius
        color: backgroundColor
        border.width: textField.activeFocus ? 2 : 1
        border.color: root.borderColor
        
        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }
        
        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 8
            
            // 前缀
            Label {
                visible: root.prefix.length > 0
                text: root.prefix
                font.pixelSize: 14
                color: "#757575"
            }
            
            TextField {
                id: textField
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                font.pixelSize: 14
                color: "#212121"
                
                background: Item {}
                
                onAccepted: root.accepted()
                onEditingFinished: root.editingFinished()
            }
            
            // 后缀
            Label {
                visible: root.suffix.length > 0
                text: root.suffix
                font.pixelSize: 14
                color: "#757575"
            }
            
            // 清除按钮
            Button {
                visible: textField.text.length > 0 && !textField.readOnly && textField.activeFocus
                flat: true
                text: "✕"
                font.pixelSize: 12
                implicitWidth: 24
                implicitHeight: 24
                
                onClicked: textField.text = ""
                
                background: Rectangle {
                    radius: 12
                    color: parent.hovered ? "#E0E0E0" : "transparent"
                }
            }
        }
    }
    
    // 错误/帮助文本
    Label {
        visible: hasError || helperText.length > 0
        text: hasError ? errorMessage : helperText
        font.pixelSize: 11
        color: hasError ? "#F44336" : "#757575"
        width: parent.width
        wrapMode: Text.WordWrap
    }
    
    // 便捷方法
    function clear() {
        textField.text = ""
    }
    
    function focus() {
        textField.forceActiveFocus()
    }
    
    function selectAll() {
        textField.selectAll()
    }
}
