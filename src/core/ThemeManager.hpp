#ifndef THEMEMANAGER_HPP
#define THEMEMANAGER_HPP

#include <QObject>
#include <QColor>
#include <QSettings>

class ThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool isDarkMode READ isDarkMode WRITE setDarkMode NOTIFY themeChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY themeChanged)
    Q_PROPERTY(QColor surfaceColor READ surfaceColor NOTIFY themeChanged)
    Q_PROPERTY(QColor primaryTextColor READ primaryTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor secondaryTextColor READ secondaryTextColor NOTIFY themeChanged)
    Q_PROPERTY(QColor accentColor READ accentColor WRITE setAccentColor NOTIFY themeChanged)
    Q_PROPERTY(QColor borderColor READ borderColor NOTIFY themeChanged)
    Q_PROPERTY(double scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged)
    Q_PROPERTY(int cornerRadius READ cornerRadius NOTIFY themeChanged)

public:
    explicit ThemeManager(QObject *parent = nullptr);

    bool isDarkMode() const { return m_darkMode; }
    void setDarkMode(bool dark);

    QColor backgroundColor() const;
    QColor surfaceColor() const;
    QColor primaryTextColor() const;
    QColor secondaryTextColor() const;
    QColor accentColor() const { return m_accentColor; }
    void setAccentColor(const QColor &color);

    QColor borderColor() const;
    double scaleFactor() const { return m_scaleFactor; }
    void setScaleFactor(double scale);

    int cornerRadius() const { return 12; }

    Q_INVOKABLE void toggleTheme();
    Q_INVOKABLE void resetToDefaults();

signals:
    void themeChanged();
    void scaleFactorChanged();

private:
    void loadSettings();
    void saveSettings();

    bool m_darkMode{true};
    QColor m_accentColor{"#3B82F6"}; // Default FlexOS blue accent
    double m_scaleFactor{1.0};
    QSettings m_settings;
};

#endif // THEMEMANAGER_HPP
