#include "color_helper.h"

#include <QHash>
#include <cmath>

ColorHelper::ColorHelper(QObject *parent)
    : QObject(parent)
{
}

QColor ColorHelper::lighten(const QColor &color, qreal factor)
{
    factor = qBound(0.0, factor, 1.0);
    
    int r = color.red() + static_cast<int>((255 - color.red()) * factor);
    int g = color.green() + static_cast<int>((255 - color.green()) * factor);
    int b = color.blue() + static_cast<int>((255 - color.blue()) * factor);
    
    return QColor(r, g, b, color.alpha());
}

QColor ColorHelper::darken(const QColor &color, qreal factor)
{
    factor = qBound(0.0, factor, 1.0);
    
    int r = static_cast<int>(color.red() * (1.0 - factor));
    int g = static_cast<int>(color.green() * (1.0 - factor));
    int b = static_cast<int>(color.blue() * (1.0 - factor));
    
    return QColor(r, g, b, color.alpha());
}

QColor ColorHelper::withAlpha(const QColor &color, qreal alpha)
{
    alpha = qBound(0.0, alpha, 1.0);
    QColor result = color;
    result.setAlphaF(alpha);
    return result;
}

QColor ColorHelper::blend(const QColor &color1, const QColor &color2, qreal ratio)
{
    ratio = qBound(0.0, ratio, 1.0);
    
    int r = static_cast<int>(color1.red() * (1.0 - ratio) + color2.red() * ratio);
    int g = static_cast<int>(color1.green() * (1.0 - ratio) + color2.green() * ratio);
    int b = static_cast<int>(color1.blue() * (1.0 - ratio) + color2.blue() * ratio);
    int a = static_cast<int>(color1.alpha() * (1.0 - ratio) + color2.alpha() * ratio);
    
    return QColor(r, g, b, a);
}

QColor ColorHelper::contrastColor(const QColor &backgroundColor)
{
    return isDark(backgroundColor) ? QColor(Qt::white) : QColor(Qt::black);
}

bool ColorHelper::isDark(const QColor &color)
{
    // 使用相对亮度公式 (ITU-R BT.709)
    double luminance = 0.2126 * color.redF() + 0.7152 * color.greenF() + 0.0722 * color.blueF();
    return luminance < 0.5;
}

QColor ColorHelper::statusColor(const QString &status)
{
    static const QHash<QString, QColor> statusColors = {
        {"success",    QColor("#4CAF50")},  // 绿色
        {"warning",    QColor("#FF9800")},  // 橙色
        {"error",      QColor("#F44336")},  // 红色
        {"info",       QColor("#2196F3")},  // 蓝色
        {"pending",    QColor("#FF9800")},  // 橙色
        {"processing", QColor("#2196F3")},  // 蓝色
        {"shipped",    QColor("#9C27B0")},  // 紫色
        {"delivered",  QColor("#4CAF50")},  // 绿色
        {"cancelled",  QColor("#F44336")},  // 红色
    };
    
    return statusColors.value(status.toLower(), QColor("#9E9E9E"));
}

QColor ColorHelper::statusBackgroundColor(const QString &status)
{
    QColor baseColor = statusColor(status);
    return lighten(baseColor, 0.85);
}
