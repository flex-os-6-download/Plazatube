#ifndef APPLAUNCHERSERVICE_HPP
#define APPLAUNCHERSERVICE_HPP

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QString>
#include <QProcess>

struct AppEntry {
    QString id;
    QString name;
    QString exec;
    QString icon;
    QString comment;
    QString category;
};

class AppLauncherService : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList applications READ applications NOTIFY applicationsChanged)

public:
    explicit AppLauncherService(QObject *parent = nullptr);

    QVariantList applications() const;

    Q_INVOKABLE void refreshApplications();
    Q_INVOKABLE bool launchApp(const QString &execCmd);
    Q_INVOKABLE QVariantList searchApplications(const QString &query) const;

signals:
    void applicationsChanged();
    void appLaunched(const QString &appName);
    void launchError(const QString &errorMsg);

private:
    QList<AppEntry> m_apps;
    void parseDesktopFiles();
};

#endif // APPLAUNCHERSERVICE_HPP
