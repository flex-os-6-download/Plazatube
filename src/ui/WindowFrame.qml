import QtQuick
import QtQuick.Controls

Rectangle {
    id: windowFrameRoot
    property var windowData: ({})

    visible: !windowData.minimized
    x: windowData.maximized ? 0 : windowData.x
    y: windowData.maximized ? 0 : windowData.y
    width: windowData.maximized ? parent.width : windowData.width
    height: windowData.maximized ? parent.height : windowData.height
    z: windowData.zIndex

    radius: windowData.maximized ? 0 : Theme.radiusMedium
    color: Theme.surface
    border.color: windowData.active ? Theme.accent : Theme.border
    border.width: windowData.active ? 2 : 1

    Behavior on x { NumberAnimation { duration: Theme.animFast } }
    Behavior on y { NumberAnimation { duration: Theme.animFast } }
    Behavior on width { NumberAnimation { duration: Theme.animFast } }
    Behavior on height { NumberAnimation { duration: Theme.animFast } }

    MouseArea {
        anchors.fill: parent
        onPressed: WindowManager.focusWindow(windowData.id)
    }

    Column {
        anchors.fill: parent

        // Titlebar Header and Window Controls
        WindowDecoration {
            width: parent.width
            windowData: windowFrameRoot.windowData

            MouseArea {
                anchors.fill: parent
                property point clickPos: "0,0"
                onPressed: (mouse) => {
                    WindowManager.focusWindow(windowData.id)
                    clickPos = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: (mouse) => {
                    if (!windowData.maximized && pressed) {
                        var delta = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)
                        WindowManager.moveWindow(windowData.id, windowFrameRoot.x + delta.x, windowFrameRoot.y + delta.y)
                    }
                }
            }
        }

        // Application Content Area
        Rectangle {
            width: parent.width
            height: parent.height - DesignTokens.windowHeaderHeight
            color: Theme.bg

            Text {
                anchors.centerIn: parent
                text: "Application: " + windowData.title + "\nApp ID: " + windowData.appId
                color: Theme.textSecondary
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontTitle
            }
        }
    }

    // Resize Handle in bottom right corner
    Rectangle {
        width: 16
        height: 16
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        color: "transparent"
        visible: !windowData.maximized

        Text {
            anchors.centerIn: parent
            text: "◢"
            color: Theme.textSecondary
            font.pixelSize: 10
        }

        MouseArea {
            anchors.fill: parent
            property point clickPos: "0,0"
            onPressed: (mouse) => {
                clickPos = Qt.point(mouse.x, mouse.y)
            }
            onPositionChanged: (mouse) => {
                if (pressed) {
                    var deltaX = mouse.x - clickPos.x
                    var deltaY = mouse.y - clickPos.y
                    WindowManager.resizeWindow(windowData.id, windowFrameRoot.width + deltaX, windowFrameRoot.height + deltaY)
                }
            }
        }
    }
}
