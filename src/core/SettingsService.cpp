#include "SettingsService.hpp"

SettingsService::SettingsService(QObject *parent)
    : QObject(parent), m_settings("FlexOS", "Settings") {
    load();
}

void SettingsService::setAnimationsEnabled(bool enabled) {
    if (m_animationsEnabled != enabled) {
        m_animationsEnabled = enabled;
        save();
        emit settingsChanged();
    }
}

void SettingsService::setReduceMotion(bool reduce) {
    if (m_reduceMotion != reduce) {
        m_reduceMotion = reduce;
        if (m_reduceMotion) {
            m_animationsEnabled = false;
        }
        save();
        emit settingsChanged();
    }
}

void SettingsService::setHostname(const QString &host) {
    if (m_hostname != host && !host.trimmed().isEmpty()) {
        m_hostname = host.trimmed();
        save();
        emit settingsChanged();
    }
}

QString SettingsService::getSetting(const QString &key, const QString &defaultValue) const {
    return m_settings.value(key, defaultValue).toString();
}

void SettingsService::setSetting(const QString &key, const QString &value) {
    m_settings.setValue(key, value);
    emit settingsChanged();
}

void SettingsService::load() {
    m_animationsEnabled = m_settings.value("animationsEnabled", true).toBool();
    m_reduceMotion = m_settings.value("reduceMotion", false).toBool();
    m_hostname = m_settings.value("hostname", "flexos-device").toString();
}

void SettingsService::save() {
    m_settings.setValue("animationsEnabled", m_animationsEnabled);
    m_settings.setValue("reduceMotion", m_reduceMotion);
    m_settings.setValue("hostname", m_hostname);
}
