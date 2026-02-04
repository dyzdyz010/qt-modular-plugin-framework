import QtQuick
import QtQuick.Controls

/**
 * 加载指示器组件
 * 
 * 使用方式:
 * MPFLoadingIndicator {
 *     running: isLoading
 *     text: "加载中..."
 * }
 */
Column {
    id: root
    
    property bool running: true
    property string text: ""
    property string size: "medium"  // small, medium, large
    property color indicatorColor: "#2196F3"
    
    property int indicatorSize: {
        switch (size) {
        case "small": return 24
        case "large": return 48
        default: return 36
        }
    }
    
    spacing: 12
    
    Item {
        width: indicatorSize
        height: indicatorSize
        anchors.horizontalCenter: parent.horizontalCenter
        
        // 外圈
        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "transparent"
            border.width: indicatorSize / 12
            border.color: Qt.alpha(indicatorColor, 0.2)
        }
        
        // 旋转弧
        Canvas {
            id: canvas
            anchors.fill: parent
            
            property real angle: 0
            
            onPaint: {
                var ctx = getContext("2d")
                ctx.reset()
                
                var centerX = width / 2
                var centerY = height / 2
                var radius = (width - indicatorSize / 6) / 2
                var lineWidth = indicatorSize / 12
                
                ctx.beginPath()
                ctx.arc(centerX, centerY, radius, 
                        angle - Math.PI / 2, 
                        angle + Math.PI / 2, false)
                ctx.strokeStyle = indicatorColor
                ctx.lineWidth = lineWidth
                ctx.lineCap = "round"
                ctx.stroke()
            }
            
            NumberAnimation on angle {
                running: root.running
                from: 0
                to: Math.PI * 2
                duration: 1000
                loops: Animation.Infinite
            }
            
            onAngleChanged: requestPaint()
        }
    }
    
    Label {
        visible: root.text.length > 0
        text: root.text
        font.pixelSize: {
            switch (root.size) {
            case "small": return 12
            case "large": return 16
            default: return 14
            }
        }
        color: "#757575"
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
