#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include <QString>
#include <QList>

struct FlexWindow {
    int id;
    QString title;
    QString appId;
    QString iconName;
    bool minimized{false};
    bool maximized{false};
    bool active{false};
    int x{100};
    int y{100};
    int width{800};
    int height{600};
    int zIndex{0};
};

class WindowManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList windows READ windows NOTIFY windowsChanged)
    Q_PROPERTY(int activeWindowId READ activeWindowId NOTIFY activeWindowChanged)

public:
    explicit WindowManager(QObject *parent = nullptr);

    QVariantList windows() const;
    int activeWindowId() const { return m_activeWindowId; }

    Q_INVOKABLE int openWindow(const QString &title, const QString &appId, const QString &iconName = QString());
    Q_INVOKABLE void closeWindow(int id);
    Q_INVOKABLE void minimizeWindow(int id);
    Q_INVOKABLE void maximizeWindow(int id);
    Q_INVOKABLE void restoreWindow(int id);
    Q_INVOKABLE void focusWindow(int id);
    Q_INVOKABLE void moveWindow(int id, int x, int y);
    Q_INVOKABLE void resizeWindow(int id, int width, int height);

signals:
    void windowsChanged();
    void activeWindowChanged();

private:
    QList<FlexWindow> m_windowsList;
    int m_nextId{1};
    int m_activeWindowId{-1};
    int m_highestZ{1};

    int findWindowIndex(int id) const;
};

#endif // WINDOWMANAGER_HPP
