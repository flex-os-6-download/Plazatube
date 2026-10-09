import QtQuick
import QtQuick.Controls

Item {
    id: desktopShell
    anchors.fill: parent

    // Desktop background gradient / wallpaper
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: Theme.bg }
            GradientStop { position: 1.0; color: Qt.darker(Theme.bg, 1.3) }
        }
    }

    // Interactive desktop overlay surface for windows
    Item {
        id: windowArea
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: taskbar.top

        // Render virtual windows managed by WindowManager
        Repeater {
            model: (typeof WindowManager !== "undefined" && WindowManager) ? WindowManager.windows : []

            delegate: WindowFrame {
                windowData: modelData
            }
        }
    }

    // Top DYNDi container overlay
    DYNDi {
        id: dyndiComponent
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 12
        z: 9999
    }

    // Application Launcher Overlay
    AppLauncher {
        id: appLauncherOverlay
        anchors.fill: parent
        visible: false
        z: 9000
    }

    // Settings Application Window Overlay
    SettingsApp {
        id: settingsAppOverlay
        anchors.centerIn: parent
        visible: false
        z: 8000
    }

    // Bottom Taskbar
    Taskbar {
        id: taskbar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: DesignTokens.taskbarHeight
        z: 7000

        onLauncherClicked: {
            appLauncherOverlay.visible = !appLauncherOverlay.visible;
        }

        onSettingsClicked: {
            settingsAppOverlay.visible = !settingsAppOverlay.visible;
        }

        onDyndiClicked: {
            if (typeof DyndiController !== "undefined" && DyndiController) {
                DyndiController.toggle();
            }
        }
    }
}
