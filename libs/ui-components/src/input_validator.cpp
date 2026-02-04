#include "input_validator.h"

#include <QRegularExpression>
#include <QUrl>

InputValidator::InputValidator(QObject *parent)
    : QObject(parent)
{
}

QVariantMap InputValidator::makeResult(bool valid, const QString &message)
{
    return {
        {"valid", valid},
        {"message", message}
    };
}

QVariantMap InputValidator::validateEmail(const QString &email)
{
    if (email.trimmed().isEmpty()) {
        return makeResult(false, QStringLiteral("邮箱地址不能为空"));
    }
    
    static const QRegularExpression emailRegex(
        QStringLiteral(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)")
    );
    
    if (!emailRegex.match(email).hasMatch()) {
        return makeResult(false, QStringLiteral("邮箱格式不正确"));
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validatePhone(const QString &phone)
{
    if (phone.trimmed().isEmpty()) {
        return makeResult(false, QStringLiteral("手机号不能为空"));
    }
    
    // 中国大陆手机号正则
    static const QRegularExpression phoneRegex(
        QStringLiteral(R"(^1[3-9]\d{9}$)")
    );
    
    QString cleanPhone = phone;
    cleanPhone.remove(QRegularExpression(QStringLiteral(R"([\s\-])")));
    
    if (!phoneRegex.match(cleanPhone).hasMatch()) {
        return makeResult(false, QStringLiteral("手机号格式不正确"));
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validateRequired(const QString &value, const QString &fieldName)
{
    if (value.trimmed().isEmpty()) {
        QString msg = fieldName.isEmpty() 
            ? QStringLiteral("此字段为必填项")
            : QStringLiteral("%1不能为空").arg(fieldName);
        return makeResult(false, msg);
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validateLength(const QString &value, int minLength, int maxLength)
{
    int len = value.length();
    
    if (len < minLength) {
        return makeResult(false, QStringLiteral("长度不能少于 %1 个字符").arg(minLength));
    }
    
    if (maxLength > 0 && len > maxLength) {
        return makeResult(false, QStringLiteral("长度不能超过 %1 个字符").arg(maxLength));
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validateRange(double value, double min, double max)
{
    if (value < min) {
        return makeResult(false, QStringLiteral("值不能小于 %1").arg(min));
    }
    
    if (value > max) {
        return makeResult(false, QStringLiteral("值不能大于 %1").arg(max));
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validatePattern(const QString &value, const QString &pattern, 
                                             const QString &message)
{
    QRegularExpression regex(pattern);
    
    if (!regex.isValid()) {
        return makeResult(false, QStringLiteral("无效的正则表达式"));
    }
    
    if (!regex.match(value).hasMatch()) {
        return makeResult(false, message.isEmpty() ? QStringLiteral("格式不正确") : message);
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validatePassword(const QString &password)
{
    if (password.isEmpty()) {
        return {
            {"valid", false},
            {"message", QStringLiteral("密码不能为空")},
            {"strength", QStringLiteral("none")}
        };
    }
    
    if (password.length() < 6) {
        return {
            {"valid", false},
            {"message", QStringLiteral("密码长度至少为 6 位")},
            {"strength", QStringLiteral("weak")}
        };
    }
    
    int score = 0;
    
    // 长度评分
    if (password.length() >= 8) score++;
    if (password.length() >= 12) score++;
    
    // 包含小写字母
    if (password.contains(QRegularExpression(QStringLiteral("[a-z]")))) score++;
    
    // 包含大写字母
    if (password.contains(QRegularExpression(QStringLiteral("[A-Z]")))) score++;
    
    // 包含数字
    if (password.contains(QRegularExpression(QStringLiteral("\\d")))) score++;
    
    // 包含特殊字符
    if (password.contains(QRegularExpression(QStringLiteral("[!@#$%^&*(),.?\":{}|<>]")))) score++;
    
    QString strength;
    QString message;
    
    if (score <= 2) {
        strength = QStringLiteral("weak");
        message = QStringLiteral("密码强度较弱，建议使用大小写字母、数字和特殊字符的组合");
    } else if (score <= 4) {
        strength = QStringLiteral("medium");
        message = QStringLiteral("密码强度中等");
    } else {
        strength = QStringLiteral("strong");
        message = QStringLiteral("密码强度较高");
    }
    
    return {
        {"valid", true},
        {"message", message},
        {"strength", strength}
    };
}

QVariantMap InputValidator::validateMatch(const QString &value1, const QString &value2, 
                                           const QString &fieldName)
{
    if (value1 != value2) {
        QString msg = fieldName.isEmpty()
            ? QStringLiteral("两次输入不一致")
            : QStringLiteral("两次输入的%1不一致").arg(fieldName);
        return makeResult(false, msg);
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validateUrl(const QString &url)
{
    if (url.trimmed().isEmpty()) {
        return makeResult(false, QStringLiteral("URL 不能为空"));
    }
    
    QUrl parsedUrl(url);
    
    if (!parsedUrl.isValid()) {
        return makeResult(false, QStringLiteral("URL 格式不正确"));
    }
    
    if (parsedUrl.scheme().isEmpty() || 
        (parsedUrl.scheme() != QStringLiteral("http") && parsedUrl.scheme() != QStringLiteral("https"))) {
        return makeResult(false, QStringLiteral("URL 必须以 http:// 或 https:// 开头"));
    }
    
    return makeResult(true);
}

QVariantMap InputValidator::validateInteger(const QString &value)
{
    if (value.trimmed().isEmpty()) {
        return makeResult(false, QStringLiteral("请输入数值"));
    }
    
    bool ok;
    int intValue = value.toInt(&ok);
    
    if (!ok) {
        return makeResult(false, QStringLiteral("请输入有效的整数"));
    }
    
    return {
        {"valid", true},
        {"message", QString()},
        {"value", intValue}
    };
}

QVariantMap InputValidator::validateDecimal(const QString &value, int decimals)
{
    if (value.trimmed().isEmpty()) {
        return makeResult(false, QStringLiteral("请输入数值"));
    }
    
    bool ok;
    double doubleValue = value.toDouble(&ok);
    
    if (!ok) {
        return makeResult(false, QStringLiteral("请输入有效的数值"));
    }
    
    if (decimals >= 0) {
        int dotIndex = value.indexOf('.');
        if (dotIndex != -1) {
            int actualDecimals = value.length() - dotIndex - 1;
            if (actualDecimals > decimals) {
                return makeResult(false, QStringLiteral("小数位数不能超过 %1 位").arg(decimals));
            }
        }
    }
    
    return {
        {"valid", true},
        {"message", QString()},
        {"value", doubleValue}
    };
}
