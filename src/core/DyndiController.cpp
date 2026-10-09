#include "DyndiController.hpp"

DyndiController::DyndiController(QObject *parent) : QObject(parent) {}

QString DyndiController::state() const {
    switch (m_state) {
        case Compact: return "Compact";
        case Expanding: return "Expanding";
        case Expanded: return "Expanded";
        case Collapsing: return "Collapsing";
        case Notification: return "Notification";
        case Assistant: return "Assistant";
        case Busy: return "Busy";
        case Error: return "Error";
        default: return "Compact";
    }
}

void DyndiController::updateState(State newState) {
    if (m_state != newState) {
        m_state = newState;
        emit stateChanged();
    }
}

void DyndiController::expand() {
    if (m_state == Compact || m_state == Notification) {
        updateState(Expanded);
    }
}

void DyndiController::collapse() {
    if (m_state != Compact) {
        updateState(Compact);
    }
}

void DyndiController::toggle() {
    if (m_state == Compact) {
        expand();
    } else {
        collapse();
    }
}

void DyndiController::pushNotification(const QString &title, const QString &message, const QString &icon) {
    QVariantMap notif;
    notif["title"] = title;
    notif["message"] = message;
    notif["icon"] = icon;
    m_notificationQueue.append(notif);

    m_title = title;
    m_subtitle = message;
    m_iconName = icon;

    updateState(Notification);
    emit contentChanged();
    emit notificationsChanged();
}

void DyndiController::dismissNotification() {
    if (!m_notificationQueue.isEmpty()) {
        m_notificationQueue.removeFirst();
    }

    if (!m_notificationQueue.isEmpty()) {
        const auto &next = m_notificationQueue.first();
        m_title = next["title"].toString();
        m_subtitle = next["message"].toString();
        m_iconName = next["icon"].toString();
        emit contentChanged();
    } else {
        m_title = "FlexOS Desktop";
        m_subtitle = "System Operational";
        m_iconName = "emblem-system";
        collapse();
        emit contentChanged();
    }
    emit notificationsChanged();
}

void DyndiController::setAssistantState(const QString &prompt, const QString &status) {
    m_title = "Gemini Assistant";
    m_subtitle = prompt.isEmpty() ? status : prompt;
    m_iconName = "stars";
    updateState(Assistant);
    emit contentChanged();
}
