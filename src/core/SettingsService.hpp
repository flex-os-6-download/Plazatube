#ifndef SETTINGSSERVICE_HPP
#define SETTINGSSERVICE_HPP

#include <QObject>
#include <QSettings>
#include <QString>

class SettingsService : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool animationsEnabled READ animationsEnabled WRITE setAnimationsEnabled NOTIFY settingsChanged)
    Q_PROPERTY(bool reduceMotion READ reduceMotion WRITE setReduceMotion NOTIFY settingsChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY settingsChanged)

public:
    explicit SettingsService(QObject *parent = nullptr);

    bool animationsEnabled() const { return m_animationsEnabled; }
    void setAnimationsEnabled(bool enabled);

    bool reduceMotion() const { return m_reduceMotion; }
    void setReduceMotion(bool reduce);

    QString hostname() const { return m_hostname; }
    void setHostname(const QString &host);

    Q_INVOKABLE QString getSetting(const QString &key, const QString &defaultValue = QString()) const;
    Q_INVOKABLE void setSetting(const QString &key, const QString &value);

signals:
    void settingsChanged();

private:
    void load();
    void save();

    bool m_animationsEnabled{true};
    bool m_reduceMotion{false};
    QString m_hostname{"flexos-device"};
    QSettings m_settings;
};

#endif // SETTINGSSERVICE_HPP
