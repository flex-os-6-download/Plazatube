#ifndef DYNDICONTROLLER_HPP
#define DYNDICONTROLLER_HPP

#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVariantMap>

class DyndiController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString title READ title NOTIFY contentChanged)
    Q_PROPERTY(QString subtitle READ subtitle NOTIFY contentChanged)
    Q_PROPERTY(QString iconName READ iconName NOTIFY contentChanged)
    Q_PROPERTY(bool hasPendingNotification READ hasPendingNotification NOTIFY notificationsChanged)
    Q_PROPERTY(int notificationCount READ notificationCount NOTIFY notificationsChanged)

public:
    enum State {
        Compact,
        Expanding,
        Expanded,
        Collapsing,
        Notification,
        Assistant,
        Busy,
        Error
    };
    Q_ENUM(State)

    explicit DyndiController(QObject *parent = nullptr);

    QString state() const;
    QString title() const { return m_title; }
    QString subtitle() const { return m_subtitle; }
    QString iconName() const { return m_iconName; }
    bool hasPendingNotification() const { return !m_notificationQueue.isEmpty(); }
    int notificationCount() const { return m_notificationQueue.size(); }

    Q_INVOKABLE void expand();
    Q_INVOKABLE void collapse();
    Q_INVOKABLE void toggle();
    Q_INVOKABLE void pushNotification(const QString &title, const QString &message, const QString &icon = "dialog-information");
    Q_INVOKABLE void dismissNotification();
    Q_INVOKABLE void setAssistantState(const QString &prompt, const QString &status = "Ready");

signals:
    void stateChanged();
    void contentChanged();
    void notificationsChanged();

private:
    State m_state{Compact};
    QString m_title{"FlexOS Desktop"};
    QString m_subtitle{"System Operational"};
    QString m_iconName{"emblem-system"};
    QList<QVariantMap> m_notificationQueue;

    void updateState(State newState);
};

#endif // DYNDICONTROLLER_HPP
