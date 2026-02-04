import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

/**
 * 通用卡片组件
 * 
 * 使用方式:
 * MPFCard {
 *     title: "标题"
 *     subtitle: "副标题"
 *     
 *     // 卡片内容
 *     Text { text: "内容" }
 * }
 */
Rectangle {
    id: root
    
    // 标题区域属性
    property string title: ""
    property string subtitle: ""
    property bool showHeader: title.length > 0 || subtitle.length > 0
    
    // 样式属性
    property color cardColor: "#FFFFFF"
    property color headerColor: "transparent"
    property color borderColor: "#E0E0E0"
    property int borderWidth: 1
    property int cardRadius: 8
    property int contentPadding: 16
    property int headerPadding: 16
    
    // 阴影
    property bool elevated: false
    property int elevation: 4
    
    // 交互
    property bool hoverable: false
    property bool clickable: false
    
    signal clicked()
    signal doubleClicked()
    
    // 内容区域
    default property alias content: contentContainer.data
    
    // 头部右侧操作区
    property alias headerActions: headerActionsContainer.data
    
    implicitWidth: 320
    implicitHeight: headerLayout.implicitHeight + contentContainer.implicitHeight + (showHeader ? 0 : contentPadding)
    
    color: cardColor
    radius: cardRadius
    border.width: borderWidth
    border.color: borderColor
    
    // 阴影层
    layer.enabled: elevated
    layer.effect: Item {
        // 简单的阴影效果
    }
    
    // 悬停效果
    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        color: "#000000"
        opacity: (hoverable || clickable) && mouseArea.containsMouse ? 0.03 : 0
        
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: hoverable || clickable
        cursorShape: clickable ? Qt.PointingHandCursor : Qt.ArrowCursor
        
        onClicked: if (root.clickable) root.clicked()
        onDoubleClicked: if (root.clickable) root.doubleClicked()
    }
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // 头部区域
        Rectangle {
            id: headerLayout
            Layout.fillWidth: true
            Layout.preferredHeight: showHeader ? headerContent.implicitHeight + headerPadding * 2 : 0
            visible: showHeader
            color: headerColor
            radius: cardRadius
            
            // 只圆角顶部
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: cardRadius
                color: headerColor
            }
            
            RowLayout {
                id: headerContent
                anchors.fill: parent
                anchors.margins: headerPadding
                spacing: 12
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    
                    Label {
                        text: root.title
                        font.pixelSize: 16
                        font.bold: true
                        color: "#212121"
                        visible: root.title.length > 0
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                    
                    Label {
                        text: root.subtitle
                        font.pixelSize: 13
                        color: "#757575"
                        visible: root.subtitle.length > 0
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }
                
                // 头部操作区
                Row {
                    id: headerActionsContainer
                    spacing: 8
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }
            }
            
            // 分隔线
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 1
                color: borderColor
            }
        }
        
        // 内容区域
        Item {
            id: contentContainer
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: contentPadding
            
            implicitHeight: childrenRect.height
        }
    }
}
