import QtQuick
import QtQuick.Window

Window {
    id: mainWindow
    width: 1280
    height: 800
    visible: true
    title: "FlexOS Desktop"

    DesktopShell {
        anchors.fill: parent
    }
}
