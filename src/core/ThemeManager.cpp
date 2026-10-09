#include "ThemeManager.hpp"

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent), m_settings("FlexOS", "Theme") {
    loadSettings();
}

void ThemeManager::setDarkMode(bool dark) {
    if (m_darkMode != dark) {
        m_darkMode = dark;
        saveSettings();
        emit themeChanged();
    }
}

void ThemeManager::setAccentColor(const QColor &color) {
    if (m_accentColor != color) {
        m_accentColor = color;
        saveSettings();
        emit themeChanged();
    }
}

void ThemeManager::setScaleFactor(double scale) {
    if (qFuzzyCompare(m_scaleFactor, scale) == false && scale >= 0.5 && scale <= 3.0) {
        m_scaleFactor = scale;
        saveSettings();
        emit scaleFactorChanged();
    }
}

QColor ThemeManager::backgroundColor() const {
    return m_darkMode ? QColor("#0F172A") : QColor("#F8FAFC");
}

QColor ThemeManager::surfaceColor() const {
    return m_darkMode ? QColor("#1E293B") : QColor("#FFFFFF");
}

QColor ThemeManager::primaryTextColor() const {
    return m_darkMode ? QColor("#F8FAFC") : QColor("#0F172A");
}

QColor ThemeManager::secondaryTextColor() const {
    return m_darkMode ? QColor("#94A3B8") : QColor("#64748B");
}

QColor ThemeManager::borderColor() const {
    return m_darkMode ? QColor("#334155") : QColor("#E2E8F0");
}

void ThemeManager::toggleTheme() {
    setDarkMode(!m_darkMode);
}

void ThemeManager::resetToDefaults() {
    m_darkMode = true;
    m_accentColor = QColor("#3B82F6");
    m_scaleFactor = 1.0;
    saveSettings();
    emit themeChanged();
    emit scaleFactorChanged();
}

void ThemeManager::loadSettings() {
    m_darkMode = m_settings.value("darkMode", true).toBool();
    m_accentColor = QColor(m_settings.value("accentColor", "#3B82F6").toString());
    m_scaleFactor = m_settings.value("scaleFactor", 1.0).toDouble();
}

void ThemeManager::saveSettings() {
    m_settings.setValue("darkMode", m_darkMode);
    m_settings.setValue("accentColor", m_accentColor.name());
    m_settings.setValue("scaleFactor", m_scaleFactor);
}
