import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: taskbarRoot
    color: Qt.rgba(Theme.surface.r, Theme.surface.g, Theme.surface.b, 0.85)
    border.color: Theme.border
    border.width: 1

    signal launcherClicked()
    signal settingsClicked()
    signal dyndiClicked()

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.spacingMD
        anchors.rightMargin: Theme.spacingMD
        spacing: Theme.spacingMD

        // Launcher Start Button
        Rectangle {
            Layout.preferredWidth: 40
            Layout.preferredHeight: 40
            radius: Theme.radiusMedium
            color: launcherMouse.containsMouse ? Theme.accent : Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.4)

            Text {
                anchors.centerIn: parent
                text: "Flex"
                font.bold: true
                font.pixelSize: 13
                color: Theme.textPrimary
            }

            MouseArea {
                id: launcherMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: taskbarRoot.launcherClicked()
            }
        }

        // Active/Pinned Application Task Bar List
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingSM

            Repeater {
                model: (typeof WindowManager !== "undefined" && WindowManager) ? WindowManager.windows : []

                delegate: Rectangle {
                    Layout.preferredWidth: 140
                    Layout.preferredHeight: 36
                    radius: Theme.radiusSmall
                    color: modelData.active ? Theme.accent : (appItemMouse.containsMouse ? Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.6) : Qt.rgba(Theme.surface.r, Theme.surface.g, Theme.surface.b, 0.5))
                    border.color: Theme.border

                    Text {
                        anchors.centerIn: parent
                        text: modelData.title
                        color: Theme.textPrimary
                        font.pixelSize: Theme.fontBody
                        elide: Text.ElideRight
                        width: parent.width - 16
                        horizontalAlignment: Text.AlignHCenter
                    }

                    MouseArea {
                        id: appItemMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if (typeof WindowManager !== "undefined" && WindowManager) {
                                if (modelData.minimized) {
                                    WindowManager.restoreWindow(modelData.id);
                                } else {
                                    WindowManager.focusWindow(modelData.id);
                                }
                            }
                        }
                    }
                }
            }
        }

        // DYNDi Quick Trigger Button
        Rectangle {
            Layout.preferredWidth: 36
            Layout.preferredHeight: 36
            radius: Theme.radiusPill
            color: dyndiMouse.containsMouse ? Theme.accent : Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.3)

            Text {
                anchors.centerIn: parent
                text: "D"
                font.bold: true
                color: Theme.textPrimary
            }

            MouseArea {
                id: dyndiMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: taskbarRoot.dyndiClicked()
            }
        }

        // System Tray & Status Area
        RowLayout {
            spacing: Theme.spacingSM

            // Battery Level Indicator
            Text {
                text: ((typeof SystemStatusService !== "undefined" && SystemStatusService) ? SystemStatusService.batteryLevel : 100) + "% " +
                      ((typeof SystemStatusService !== "undefined" && SystemStatusService && SystemStatusService.isCharging) ? "⚡" : "🔋")
                color: Theme.textSecondary
                font.pixelSize: Theme.fontBody
            }

            // Network Indicator
            Text {
                text: (typeof SystemStatusService !== "undefined" && SystemStatusService && SystemStatusService.isNetworkConnected) ? "🌐" : "❌"
                color: Theme.textSecondary
                font.pixelSize: Theme.fontBody
            }

            // Settings Launcher Button
            Rectangle {
                Layout.preferredWidth: 32
                Layout.preferredHeight: 32
                radius: Theme.radiusSmall
                color: settingsMouse.containsMouse ? Theme.accent : "transparent"

                Text {
                    anchors.centerIn: parent
                    text: "⚙"
                    color: Theme.textPrimary
                    font.pixelSize: 16
                }

                MouseArea {
                    id: settingsMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: taskbarRoot.settingsClicked()
                }
            }

            // Live Clock Display
            Text {
                id: clockText
                color: Theme.textPrimary
                font.bold: true
                font.pixelSize: Theme.fontBody

                Timer {
                    interval: 1000
                    running: true
                    repeat: true
                    triggeredOnStart: true
                    onTriggered: {
                        clockText.text = Qt.formatDateTime(new Date(), "hh:mm A")
                    }
                }
            }
        }
    }
}
