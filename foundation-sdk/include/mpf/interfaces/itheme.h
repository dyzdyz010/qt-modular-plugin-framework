#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include <QString>

namespace mpf {

/**
 * @brief Theme/Styling interface
 * 
 * Provides centralized theming for UI components.
 * Plugins should bind to theme properties for consistent styling.
 */
class ITheme : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name NOTIFY themeChanged)
    Q_PROPERTY(bool isDark READ isDark NOTIFY themeChanged)
    
    // Colors
    Q_PROPERTY(QColor primaryColor READ primaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor accentColor READ accentColor NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor surfaceColor READ surfaceColor NOTIFY themeChanged)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY themeChanged)
    Q_PROPERTY(QColor textSecondaryColor READ textSecondaryColor NOTIFY themeChanged)
    Q_PROPERTY(QColor errorColor READ errorColor NOTIFY themeChanged)
    Q_PROPERTY(QColor warningColor READ warningColor NOTIFY themeChanged)
    Q_PROPERTY(QColor successColor READ successColor NOTIFY themeChanged)
    
    // Spacing
    Q_PROPERTY(int spacingTiny READ spacingTiny NOTIFY themeChanged)
    Q_PROPERTY(int spacingSmall READ spacingSmall NOTIFY themeChanged)
    Q_PROPERTY(int spacingMedium READ spacingMedium NOTIFY themeChanged)
    Q_PROPERTY(int spacingLarge READ spacingLarge NOTIFY themeChanged)
    
    // Border radius
    Q_PROPERTY(int radiusSmall READ radiusSmall NOTIFY themeChanged)
    Q_PROPERTY(int radiusMedium READ radiusMedium NOTIFY themeChanged)
    Q_PROPERTY(int radiusLarge READ radiusLarge NOTIFY themeChanged)

public:
    using QObject::QObject;
    virtual ~ITheme() = default;

    // Theme info
    virtual QString name() const = 0;
    virtual bool isDark() const = 0;

    // Colors
    virtual QColor primaryColor() const = 0;
    virtual QColor accentColor() const = 0;
    virtual QColor backgroundColor() const = 0;
    virtual QColor surfaceColor() const = 0;
    virtual QColor textColor() const = 0;
    virtual QColor textSecondaryColor() const = 0;
    virtual QColor errorColor() const = 0;
    virtual QColor warningColor() const = 0;
    virtual QColor successColor() const = 0;

    // Spacing
    virtual int spacingTiny() const = 0;
    virtual int spacingSmall() const = 0;
    virtual int spacingMedium() const = 0;
    virtual int spacingLarge() const = 0;

    // Border radius
    virtual int radiusSmall() const = 0;
    virtual int radiusMedium() const = 0;
    virtual int radiusLarge() const = 0;

    // Theme switching
    Q_INVOKABLE virtual void setTheme(const QString& themeName) = 0;
    Q_INVOKABLE virtual QStringList availableThemes() const = 0;

signals:
    void themeChanged();

public:
    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf

#define MPF_ITheme_iid "com.mpf.ITheme/1.0"
Q_DECLARE_INTERFACE(mpf::ITheme, MPF_ITheme_iid)
