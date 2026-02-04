#pragma once

#include "ui_components_global.h"

#include <QObject>
#include <QColor>
#include <QtQml/qqmlregistration.h>

/**
 * @brief 颜色工具类，供 QML 调用
 * 
 * 使用方式：
 * @code
 * import MPF.Components 1.0
 * 
 * Rectangle {
 *     color: ColorHelper.lighten("#2196F3", 0.2)
 * }
 * @endcode
 */
class MPF_UI_COMPONENTS_EXPORT ColorHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit ColorHelper(QObject *parent = nullptr);
    ~ColorHelper() override = default;

    /**
     * @brief 使颜色变亮
     * @param color 原始颜色
     * @param factor 变亮因子 (0.0 - 1.0)
     */
    Q_INVOKABLE static QColor lighten(const QColor &color, qreal factor);

    /**
     * @brief 使颜色变暗
     * @param color 原始颜色
     * @param factor 变暗因子 (0.0 - 1.0)
     */
    Q_INVOKABLE static QColor darken(const QColor &color, qreal factor);

    /**
     * @brief 设置颜色透明度
     * @param color 原始颜色
     * @param alpha 透明度 (0.0 - 1.0)
     */
    Q_INVOKABLE static QColor withAlpha(const QColor &color, qreal alpha);

    /**
     * @brief 混合两种颜色
     * @param color1 第一种颜色
     * @param color2 第二种颜色
     * @param ratio color2 的比例 (0.0 - 1.0)
     */
    Q_INVOKABLE static QColor blend(const QColor &color1, const QColor &color2, qreal ratio);

    /**
     * @brief 获取对比色（适合文字显示）
     * @param backgroundColor 背景颜色
     * @return 黑色或白色，取决于背景亮度
     */
    Q_INVOKABLE static QColor contrastColor(const QColor &backgroundColor);

    /**
     * @brief 判断颜色是否为深色
     * @param color 要判断的颜色
     */
    Q_INVOKABLE static bool isDark(const QColor &color);

    /**
     * @brief 根据状态返回语义颜色
     * @param status 状态字符串 (success, warning, error, info)
     */
    Q_INVOKABLE static QColor statusColor(const QString &status);

    /**
     * @brief 根据状态返回浅色背景
     * @param status 状态字符串
     */
    Q_INVOKABLE static QColor statusBackgroundColor(const QString &status);
};
