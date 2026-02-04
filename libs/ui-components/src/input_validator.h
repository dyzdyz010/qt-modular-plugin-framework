#pragma once

#include "ui_components_global.h"

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>

/**
 * @brief 输入验证器，供 QML 调用
 * 
 * 使用方式：
 * @code
 * import MPF.Components 1.0
 * 
 * TextField {
 *     onTextChanged: {
 *         let result = InputValidator.validateEmail(text)
 *         if (!result.valid) {
 *             errorLabel.text = result.message
 *         }
 *     }
 * }
 * @endcode
 */
class MPF_UI_COMPONENTS_EXPORT InputValidator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit InputValidator(QObject *parent = nullptr);
    ~InputValidator() override = default;

    /**
     * @brief 验证邮箱格式
     * @param email 邮箱地址
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateEmail(const QString &email);

    /**
     * @brief 验证手机号格式（中国大陆）
     * @param phone 手机号
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validatePhone(const QString &phone);

    /**
     * @brief 验证必填字段
     * @param value 字段值
     * @param fieldName 字段名称（用于错误消息）
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateRequired(const QString &value, const QString &fieldName = QString());

    /**
     * @brief 验证字符串长度
     * @param value 字段值
     * @param minLength 最小长度
     * @param maxLength 最大长度
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateLength(const QString &value, int minLength, int maxLength);

    /**
     * @brief 验证数值范围
     * @param value 数值
     * @param min 最小值
     * @param max 最大值
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateRange(double value, double min, double max);

    /**
     * @brief 验证正则表达式
     * @param value 字段值
     * @param pattern 正则表达式
     * @param message 自定义错误消息
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validatePattern(const QString &value, const QString &pattern, 
                                                    const QString &message = QString());

    /**
     * @brief 验证密码强度
     * @param password 密码
     * @return {valid: bool, message: string, strength: string (weak/medium/strong)}
     */
    Q_INVOKABLE static QVariantMap validatePassword(const QString &password);

    /**
     * @brief 验证两个值是否相等（用于确认密码等场景）
     * @param value1 第一个值
     * @param value2 第二个值
     * @param fieldName 字段名称
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateMatch(const QString &value1, const QString &value2, 
                                                  const QString &fieldName = QString());

    /**
     * @brief 验证 URL 格式
     * @param url URL 字符串
     * @return {valid: bool, message: string}
     */
    Q_INVOKABLE static QVariantMap validateUrl(const QString &url);

    /**
     * @brief 验证整数
     * @param value 值字符串
     * @return {valid: bool, message: string, value: int}
     */
    Q_INVOKABLE static QVariantMap validateInteger(const QString &value);

    /**
     * @brief 验证小数
     * @param value 值字符串
     * @param decimals 允许的小数位数 (-1 表示不限制)
     * @return {valid: bool, message: string, value: double}
     */
    Q_INVOKABLE static QVariantMap validateDecimal(const QString &value, int decimals = -1);

private:
    static QVariantMap makeResult(bool valid, const QString &message = QString());
};
