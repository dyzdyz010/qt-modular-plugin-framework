#pragma once

#include <QString>
#include <QStringList>
#include <QColor>

namespace mpf {

/**
 * @brief Theme service interface (pure abstract)
 * 
 * Provides theming capabilities for consistent UI appearance.
 */
class ITheme
{
public:
    virtual ~ITheme() = default;

    // Identity
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
    virtual void setTheme(const QString& themeName) = 0;
    virtual QStringList availableThemes() const = 0;

    static constexpr int apiVersion() { return 1; }
};

} // namespace mpf
