#include "SystemStatusService.hpp"
#include <QSysInfo>
#include <QFile>
#include <QTextStream>
#include <QDir>

SystemStatusService::SystemStatusService(QObject *parent) : QObject(parent) {
    m_kernelVersion = QSysInfo::kernelVersion();
    m_architecture = QSysInfo::currentCpuArchitecture();

    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &SystemStatusService::querySystemInformation);
    m_pollTimer->start(10000); // 10 second refresh

    querySystemInformation();
}

void SystemStatusService::setVolumeLevel(int level) {
    if (level < 0) level = 0;
    if (level > 100) level = 100;
    if (m_volumeLevel != level) {
        m_volumeLevel = level;
        emit volumeChanged();
    }
}

void SystemStatusService::setMuted(bool muted) {
    if (m_isMuted != muted) {
        m_isMuted = muted;
        emit volumeChanged();
    }
}

void SystemStatusService::refreshStatus() {
    querySystemInformation();
}

void SystemStatusService::querySystemInformation() {
    // Read Linux sysfs power supply if available
    QDir powerSupply("/sys/class/power_supply");
    if (powerSupply.exists()) {
        QStringList supplies = powerSupply.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const auto &sup : supplies) {
            if (sup.startsWith("BAT")) {
                QFile capFile(powerSupply.filePath(sup + "/capacity"));
                if (capFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    m_batteryLevel = capFile.readAll().trimmed().toInt();
                    capFile.close();
                }
                QFile statusFile(powerSupply.filePath(sup + "/status"));
                if (statusFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    m_isCharging = (statusFile.readAll().trimmed() == "Charging");
                    statusFile.close();
                }
                break;
            }
        }
    }

    emit statusChanged();
}
