import QtQuick
import QtQuick.Layouts

Rectangle {
    id: decorationRoot
    height: DesignTokens.windowHeaderHeight
    color: Qt.rgba(Theme.surface.r, Theme.surface.g, Theme.surface.b, 0.95)
    border.color: Theme.border
    border.width: 1

    property var windowData: ({})

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.spacingMD
        anchors.rightMargin: Theme.spacingSM
        spacing: Theme.spacingSM

        // Window Title & App Icon
        Text {
            Layout.fillWidth: true
            text: windowData.title || "Window"
            color: Theme.textPrimary
            font.bold: true
            font.pixelSize: Theme.fontBody
            elide: Text.ElideRight
        }

        // Minimize Control Button
        Rectangle {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            radius: Theme.radiusSmall
            color: minMouse.containsMouse ? Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.5) : "transparent"

            Text {
                anchors.centerIn: parent
                text: "—"
                color: Theme.textPrimary
                font.bold: true
            }

            MouseArea {
                id: minMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: WindowManager.minimizeWindow(windowData.id)
            }
        }

        // Maximize / Restore Control Button
        Rectangle {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            radius: Theme.radiusSmall
            color: maxMouse.containsMouse ? Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.5) : "transparent"

            Text {
                anchors.centerIn: parent
                text: windowData.maximized ? "❐" : "□"
                color: Theme.textPrimary
                font.bold: true
            }

            MouseArea {
                id: maxMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: WindowManager.maximizeWindow(windowData.id)
            }
        }

        // Close Control Button
        Rectangle {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            radius: Theme.radiusSmall
            color: closeMouse.containsMouse ? Theme.error : "transparent"

            Text {
                anchors.centerIn: parent
                text: "✕"
                color: closeMouse.containsMouse ? "#FFFFFF" : Theme.textPrimary
                font.bold: true
            }

            MouseArea {
                id: closeMouse
                anchors.fill: parent
                hoverEnabled: true
                onClicked: WindowManager.closeWindow(windowData.id)
            }
        }
    }
}
