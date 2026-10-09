#ifndef SYSTEMSTATUSSERVICE_HPP
#define SYSTEMSTATUSSERVICE_HPP

#include <QObject>
#include <QString>
#include <QTimer>

class SystemStatusService : public QObject {
    Q_OBJECT
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY statusChanged)
    Q_PROPERTY(bool isCharging READ isCharging NOTIFY statusChanged)
    Q_PROPERTY(bool isNetworkConnected READ isNetworkConnected NOTIFY statusChanged)
    Q_PROPERTY(QString networkName READ networkName NOTIFY statusChanged)
    Q_PROPERTY(int volumeLevel READ volumeLevel WRITE setVolumeLevel NOTIFY volumeChanged)
    Q_PROPERTY(bool isMuted READ isMuted WRITE setMuted NOTIFY volumeChanged)
    Q_PROPERTY(QString kernelVersion READ kernelVersion CONSTANT)
    Q_PROPERTY(QString architecture READ architecture CONSTANT)

public:
    explicit SystemStatusService(QObject *parent = nullptr);

    int batteryLevel() const { return m_batteryLevel; }
    bool isCharging() const { return m_isCharging; }
    bool isNetworkConnected() const { return m_isNetworkConnected; }
    QString networkName() const { return m_networkName; }

    int volumeLevel() const { return m_volumeLevel; }
    void setVolumeLevel(int level);

    bool isMuted() const { return m_isMuted; }
    void setMuted(bool muted);

    QString kernelVersion() const { return m_kernelVersion; }
    QString architecture() const { return m_architecture; }

    Q_INVOKABLE void refreshStatus();

signals:
    void statusChanged();
    void volumeChanged();

private:
    void querySystemInformation();

    int m_batteryLevel{100};
    bool m_isCharging{true};
    bool m_isNetworkConnected{true};
    QString m_networkName{"Ethernet / Wi-Fi"};
    int m_volumeLevel{80};
    bool m_isMuted{false};
    QString m_kernelVersion;
    QString m_architecture;
    QTimer *m_pollTimer{nullptr};
};

#endif // SYSTEMSTATUSSERVICE_HPP
