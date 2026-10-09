#include "AppLauncherService.hpp"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDebug>

AppLauncherService::AppLauncherService(QObject *parent) : QObject(parent) {
    parseDesktopFiles();
}

QVariantList AppLauncherService::applications() const {
    QVariantList list;
    for (const auto &app : m_apps) {
        QVariantMap map;
        map["id"] = app.id;
        map["name"] = app.name;
        map["exec"] = app.exec;
        map["icon"] = app.icon;
        map["comment"] = app.comment;
        map["category"] = app.category;
        list.append(map);
    }
    return list;
}

void AppLauncherService::refreshApplications() {
    parseDesktopFiles();
    emit applicationsChanged();
}

void AppLauncherService::parseDesktopFiles() {
    m_apps.clear();

    // Default System Applications for FlexOS Shell
    m_apps.append({"flexos-settings", "FlexOS Settings", "flexos-settings", "preferences-system", "System Settings and Configurations", "System"});
    m_apps.append({"flexos-terminal", "FlexOS Terminal", "xterm", "utilities-terminal", "Command Line Terminal Emulator", "System"});
    m_apps.append({"flexos-browser", "Firefox Web Browser", "firefox", "firefox", "Browse the World Wide Web", "Network"});

    QStringList searchDirs = {
        "/usr/share/applications",
        "/usr/local/share/applications",
        QDir::homePath() + "/.local/share/applications"
    };

    for (const auto &dirPath : searchDirs) {
        QDir dir(dirPath);
        if (!dir.exists()) continue;

        QStringList files = dir.entryList({"*.desktop"}, QDir::Files);
        for (const auto &file : files) {
            QFile f(dir.filePath(file));
            if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) continue;

            QTextStream in(&f);
            bool inDesktopEntry = false;
            bool noDisplay = false;
            AppEntry entry;
            entry.id = file.chopped(8); // Remove .desktop

            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (line == "[Desktop Entry]") {
                    inDesktopEntry = true;
                    continue;
                }
                if (line.startsWith("[") && line.endsWith("]") && line != "[Desktop Entry]") {
                    inDesktopEntry = false;
                }

                if (inDesktopEntry) {
                    if (line.startsWith("Name=") && entry.name.isEmpty()) {
                        entry.name = line.mid(5);
                    } else if (line.startsWith("Exec=") && entry.exec.isEmpty()) {
                        entry.exec = line.mid(5).remove("%u").remove("%U").remove("%f").remove("%F").trimmed();
                    } else if (line.startsWith("Icon=") && entry.icon.isEmpty()) {
                        entry.icon = line.mid(5);
                    } else if (line.startsWith("Comment=") && entry.comment.isEmpty()) {
                        entry.comment = line.mid(8);
                    } else if (line.startsWith("Categories=") && entry.category.isEmpty()) {
                        entry.category = line.mid(11).split(";").first();
                    } else if (line == "NoDisplay=true") {
                        noDisplay = true;
                    }
                }
            }
            f.close();

            if (!noDisplay && !entry.name.isEmpty() && !entry.exec.isEmpty()) {
                if (entry.category.isEmpty()) entry.category = "Utility";
                m_apps.append(entry);
            }
        }
    }
}

bool AppLauncherService::launchApp(const QString &execCmd) {
    if (execCmd.isEmpty()) return false;

    bool started = QProcess::startDetached(execCmd, QStringList());
    if (started) {
        emit appLaunched(execCmd);
    } else {
        emit launchError(QString("Failed to launch application process: %1").arg(execCmd));
    }
    return started;
}

QVariantList AppLauncherService::searchApplications(const QString &query) const {
    QVariantList results;
    if (query.trimmed().isEmpty()) {
        return applications();
    }

    QString q = query.trimmed().toLower();
    for (const auto &app : m_apps) {
        if (app.name.toLower().contains(q) || app.category.toLower().contains(q) || app.comment.toLower().contains(q)) {
            QVariantMap map;
            map["id"] = app.id;
            map["name"] = app.name;
            map["exec"] = app.exec;
            map["icon"] = app.icon;
            map["comment"] = app.comment;
            map["category"] = app.category;
            results.append(map);
        }
    }
    return results;
}
