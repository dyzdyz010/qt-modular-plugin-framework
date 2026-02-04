import QtQuick
import QtQuick.Controls

/**
 * 状态徽章组件
 * 
 * 使用方式:
 * StatusBadge {
 *     status: "pending"  // 或使用 type
 *     text: "待处理"     // 可选，不填则使用状态名
 * }
 */
Rectangle {
    id: root
    
    // 状态类型 (会自动映射颜色)
    property string status: ""
    
    // 类型别名 (success, warning, error, info)
    property string type: ""
    
    // 显示文本
    property string text: ""
    
    // 尺寸
    property string size: "medium"  // small, medium, large
    
    // 样式
    property bool outlined: false
    
    // 内部状态计算
    property string _effectiveStatus: status.length > 0 ? status : type
    
    // 颜色映射
    property var statusColors: ({
        "success":    { bg: "#E8F5E9", fg: "#2E7D32", border: "#4CAF50" },
        "warning":    { bg: "#FFF3E0", fg: "#E65100", border: "#FF9800" },
        "error":      { bg: "#FFEBEE", fg: "#C62828", border: "#F44336" },
        "info":       { bg: "#E3F2FD", fg: "#1565C0", border: "#2196F3" },
        "pending":    { bg: "#FFF3E0", fg: "#E65100", border: "#FF9800" },
        "processing": { bg: "#E3F2FD", fg: "#1565C0", border: "#2196F3" },
        "shipped":    { bg: "#F3E5F5", fg: "#7B1FA2", border: "#9C27B0" },
        "delivered":  { bg: "#E8F5E9", fg: "#2E7D32", border: "#4CAF50" },
        "cancelled":  { bg: "#FFEBEE", fg: "#C62828", border: "#F44336" },
        "default":    { bg: "#F5F5F5", fg: "#757575", border: "#9E9E9E" }
    })
    
    property var currentColors: statusColors[_effectiveStatus.toLowerCase()] || statusColors["default"]
    
    implicitWidth: label.implicitWidth + horizontalPadding * 2
    implicitHeight: {
        switch (size) {
        case "small": return 18
        case "large": return 28
        default: return 22
        }
    }
    
    property int horizontalPadding: {
        switch (size) {
        case "small": return 6
        case "large": return 12
        default: return 8
        }
    }
    
    radius: height / 2
    color: outlined ? "transparent" : currentColors.bg
    border.width: outlined ? 1 : 0
    border.color: currentColors.border
    
    Label {
        id: label
        anchors.centerIn: parent
        text: root.text.length > 0 ? root.text : root._effectiveStatus
        font.pixelSize: {
            switch (root.size) {
            case "small": return 10
            case "large": return 14
            default: return 12
            }
        }
        font.capitalization: Font.Capitalize
        color: currentColors.fg
    }
}
