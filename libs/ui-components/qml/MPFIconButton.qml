import QtQuick
import QtQuick.Controls

/**
 * 图标按钮组件
 * 
 * 使用方式:
 * MPFIconButton {
 *     icon: "🗑️"
 *     tooltip: "删除"
 *     onClicked: deleteItem()
 * }
 */
Button {
    id: root
    
    property string icon: ""
    property string tooltip: ""
    property int iconSize: 18
    property color iconColor: "#757575"
    property color hoverColor: "#212121"
    property color backgroundColor: "transparent"
    property color hoverBackgroundColor: "#F5F5F5"
    property int buttonRadius: 4
    
    implicitWidth: 36
    implicitHeight: 36
    
    flat: true
    
    ToolTip.visible: tooltip.length > 0 && hovered
    ToolTip.text: tooltip
    ToolTip.delay: 500
    
    contentItem: Text {
        text: root.icon
        font.pixelSize: root.iconSize
        color: root.hovered ? root.hoverColor : root.iconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
    
    background: Rectangle {
        radius: root.buttonRadius
        color: root.hovered ? root.hoverBackgroundColor : root.backgroundColor
        
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
}
