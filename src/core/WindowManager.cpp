#include "WindowManager.hpp"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {}

QVariantList WindowManager::windows() const {
    QVariantList list;
    for (const auto &win : m_windowsList) {
        QVariantMap map;
        map["id"] = win.id;
        map["title"] = win.title;
        map["appId"] = win.appId;
        map["iconName"] = win.iconName;
        map["minimized"] = win.minimized;
        map["maximized"] = win.maximized;
        map["active"] = win.active;
        map["x"] = win.x;
        map["y"] = win.y;
        map["width"] = win.width;
        map["height"] = win.height;
        map["zIndex"] = win.zIndex;
        list.append(map);
    }
    return list;
}

int WindowManager::findWindowIndex(int id) const {
    for (int i = 0; i < m_windowsList.size(); ++i) {
        if (m_windowsList[i].id == id) return i;
    }
    return -1;
}

int WindowManager::openWindow(const QString &title, const QString &appId, const QString &iconName) {
    FlexWindow win;
    win.id = m_nextId++;
    win.title = title;
    win.appId = appId;
    win.iconName = iconName;
    win.x = 120 + (win.id % 5) * 30;
    win.y = 80 + (win.id % 5) * 30;
    win.width = 850;
    win.height = 550;
    win.zIndex = ++m_highestZ;
    win.active = true;

    for (auto &w : m_windowsList) {
        w.active = false;
    }

    m_windowsList.append(win);
    m_activeWindowId = win.id;

    emit activeWindowChanged();
    emit windowsChanged();
    return win.id;
}

void WindowManager::closeWindow(int id) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        m_windowsList.removeAt(idx);
        if (m_activeWindowId == id) {
            m_activeWindowId = -1;
            int maxZ = -1;
            for (const auto &w : m_windowsList) {
                if (!w.minimized && w.zIndex > maxZ) {
                    maxZ = w.zIndex;
                    m_activeWindowId = w.id;
                }
            }
            if (m_activeWindowId != -1) {
                focusWindow(m_activeWindowId);
            } else {
                emit activeWindowChanged();
            }
        }
        emit windowsChanged();
    }
}

void WindowManager::minimizeWindow(int id) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        m_windowsList[idx].minimized = true;
        m_windowsList[idx].active = false;
        if (m_activeWindowId == id) {
            m_activeWindowId = -1;
            emit activeWindowChanged();
        }
        emit windowsChanged();
    }
}

void WindowManager::maximizeWindow(int id) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        m_windowsList[idx].maximized = !m_windowsList[idx].maximized;
        focusWindow(id);
        emit windowsChanged();
    }
}

void WindowManager::restoreWindow(int id) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        m_windowsList[idx].minimized = false;
        focusWindow(id);
        emit windowsChanged();
    }
}

void WindowManager::focusWindow(int id) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        for (auto &w : m_windowsList) {
            w.active = (w.id == id);
        }
        m_windowsList[idx].minimized = false;
        m_windowsList[idx].zIndex = ++m_highestZ;
        m_activeWindowId = id;
        emit activeWindowChanged();
        emit windowsChanged();
    }
}

void WindowManager::moveWindow(int id, int x, int y) {
    int idx = findWindowIndex(id);
    if (idx != -1) {
        m_windowsList[idx].x = x;
        m_windowsList[idx].y = y;
        emit windowsChanged();
    }
}

void WindowManager::resizeWindow(int id, int width, int height) {
    int idx = findWindowIndex(id);
    if (idx != -1 && width >= 300 && height >= 200) {
        m_windowsList[idx].width = width;
        m_windowsList[idx].height = height;
        emit windowsChanged();
    }
}
