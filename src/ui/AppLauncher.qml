import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: launcherRoot
    color: Qt.rgba(Theme.bg.r, Theme.bg.g, Theme.bg.b, 0.92)

    MouseArea {
        anchors.fill: parent
        onClicked: launcherRoot.visible = false
    }

    Rectangle {
        id: container
        width: Math.min(parent.width - 40, 720)
        height: Math.min(parent.height - 100, 560)
        anchors.centerIn: parent
        radius: Theme.radiusLarge
        color: Theme.surface
        border.color: Theme.border
        border.width: 1

        MouseArea {
            anchors.fill: parent
            // Prevent clicks inside container from dismissing launcher
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Theme.spacingLG
            spacing: Theme.spacingMD

            // Launcher Header & Search Input
            RowLayout {
                Layout.fillWidth: true
                spacing: Theme.spacingMD

                Text {
                    text: "FlexOS Apps"
                    font.pixelSize: Theme.fontHeader
                    font.bold: true
                    color: Theme.textPrimary
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    Layout.preferredWidth: 300
                    Layout.preferredHeight: 40
                    radius: Theme.radiusSmall
                    color: Theme.bg
                    border.color: searchInput.activeFocus ? Theme.accent : Theme.border

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: Theme.spacingSM
                        anchors.rightMargin: Theme.spacingSM

                        Text { text: "🔍"; color: Theme.textSecondary }

                        TextInput {
                            id: searchInput
                            Layout.fillWidth: true
                            color: Theme.textPrimary
                            font.pixelSize: Theme.fontBody
                            focus: launcherRoot.visible

                            Text {
                                text: "Search applications..."
                                color: Theme.textSecondary
                                visible: !searchInput.text && !searchInput.activeFocus
                                font.pixelSize: Theme.fontBody
                            }
                        }
                    }
                }
            }

            // Applications Grid View
            GridView {
                id: appGrid
                Layout.fillWidth: true
                Layout.fillHeight: true
                cellWidth: 150
                cellHeight: 120
                clip: true

                model: (typeof AppLauncherService !== "undefined" && AppLauncherService) ? AppLauncherService.searchApplications(searchInput.text) : []

                delegate: Rectangle {
                    width: appGrid.cellWidth - Theme.spacingSM
                    height: appGrid.cellHeight - Theme.spacingSM
                    radius: Theme.radiusMedium
                    color: appMouse.containsMouse ? Qt.rgba(Theme.accent.r, Theme.accent.g, Theme.accent.b, 0.2) : "transparent"
                    border.color: appMouse.containsMouse ? Theme.accent : "transparent"

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: Theme.spacingXS

                        Rectangle {
                            Layout.alignment: Qt.AlignHCenter
                            Layout.preferredWidth: 48
                            Layout.preferredHeight: 48
                            radius: Theme.radiusMedium
                            color: Theme.accent

                            Text {
                                anchors.centerIn: parent
                                text: modelData.name ? modelData.name.substring(0, 1) : "A"
                                font.bold: true
                                font.pixelSize: 22
                                color: "#FFFFFF"
                            }
                        }

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: modelData.name
                            color: Theme.textPrimary
                            font.pixelSize: Theme.fontBody
                            font.bold: true
                            elide: Text.ElideRight
                            Layout.maximumWidth: 130
                        }

                        Text {
                            Layout.alignment: Qt.AlignHCenter
                            text: modelData.category
                            color: Theme.textSecondary
                            font.pixelSize: Theme.fontCaption
                        }
                    }

                    MouseArea {
                        id: appMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            if (typeof WindowManager !== "undefined" && WindowManager) {
                                if (modelData.id === "flexos-settings") {
                                    WindowManager.openWindow("FlexOS Settings", "flexos-settings", "preferences-system")
                                } else {
                                    if (typeof AppLauncherService !== "undefined" && AppLauncherService) {
                                        AppLauncherService.launchApp(modelData.exec)
                                    }
                                    WindowManager.openWindow(modelData.name, modelData.id, modelData.icon)
                                }
                            }
                            launcherRoot.visible = false
                        }
                    }
                }
            }
        }
    }
}
