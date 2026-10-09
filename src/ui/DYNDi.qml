import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: dyndiRoot

    readonly property bool isExpanded: (typeof DyndiController !== "undefined" && DyndiController) ? (DyndiController.state === "Expanded" || DyndiController.state === "Assistant") : false

    width: isExpanded ? DesignTokens.dyndiExpandedWidth : DesignTokens.dyndiCompactWidth
    height: isExpanded ? DesignTokens.dyndiExpandedHeight : DesignTokens.dyndiCompactHeight
    radius: DesignTokens.radiusPill

    color: Qt.rgba(15 / 255, 23 / 255, 42 / 255, 0.95)
    border.color: ((typeof DyndiController !== "undefined" && DyndiController) && DyndiController.state === "Assistant") ? Theme.accent : Theme.border
    border.width: isExpanded ? 2 : 1

    Behavior on width { NumberAnimation { duration: Theme.animNormal; easing.type: Easing.OutBack } }
    Behavior on height { NumberAnimation { duration: Theme.animNormal; easing.type: Easing.OutBack } }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (typeof DyndiController !== "undefined" && DyndiController) {
                DyndiController.toggle()
            }
        }
    }

    // Compact State View
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Theme.spacingMD
        anchors.rightMargin: Theme.spacingMD
        visible: !dyndiRoot.isExpanded
        spacing: Theme.spacingSM

        Rectangle {
            Layout.preferredWidth: 8
            Layout.preferredHeight: 8
            radius: 4
            color: ((typeof DyndiController !== "undefined" && DyndiController) && DyndiController.hasPendingNotification) ? Theme.warning : Theme.success
        }

        Text {
            Layout.fillWidth: true
            text: (typeof DyndiController !== "undefined" && DyndiController) ? DyndiController.title : "FlexOS Desktop"
            color: Theme.textPrimary
            font.bold: true
            font.pixelSize: Theme.fontCaption
            elide: Text.ElideRight
        }

        Text {
            text: ((typeof DyndiController !== "undefined" && DyndiController) && DyndiController.hasPendingNotification) ? "🔔" : "✦"
            color: Theme.textSecondary
            font.pixelSize: 12
        }
    }

    // Expanded State View
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingMD
        visible: dyndiRoot.isExpanded
        spacing: Theme.spacingSM

        // Header
        RowLayout {
            Layout.fillWidth: true

            Text {
                text: "DYNDi Hub"
                font.bold: true
                font.pixelSize: Theme.fontTitle
                color: Theme.textPrimary
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                Layout.preferredWidth: 24
                Layout.preferredHeight: 24
                radius: 12
                color: Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.5)

                Text {
                    anchors.centerIn: parent
                    text: "✕"
                    color: Theme.textPrimary
                    font.pixelSize: 10
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (typeof DyndiController !== "undefined" && DyndiController) {
                            DyndiController.collapse()
                        }
                    }
                }
            }
        }

        // Gemini Assistant Panel Area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: Theme.radiusMedium
            color: Theme.surface
            border.color: Theme.border

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.spacingSM
                spacing: Theme.spacingXS

                Text {
                    text: "Gemini AI Assistant"
                    font.bold: true
                    color: Theme.accent
                    font.pixelSize: Theme.fontBody
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    Text {
                        width: parent.width
                        text: ((typeof GeminiProvider !== "undefined" && GeminiProvider) && GeminiProvider.lastResponse) ? GeminiProvider.lastResponse : "Ask Gemini Assistant anything..."
                        color: Theme.textPrimary
                        font.pixelSize: Theme.fontCaption
                        wrapMode: Text.WordWrap
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.spacingXS

                    TextInput {
                        id: promptInput
                        Layout.fillWidth: true
                        color: Theme.textPrimary
                        font.pixelSize: Theme.fontBody

                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            border.color: Theme.border
                            radius: Theme.radiusSmall
                            z: -1
                        }

                        Text {
                            text: "Type prompt..."
                            color: Theme.textSecondary
                            visible: !promptInput.text
                            font.pixelSize: Theme.fontBody
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: 50
                        Layout.preferredHeight: 28
                        radius: Theme.radiusSmall
                        color: ((typeof GeminiProvider !== "undefined" && GeminiProvider) && GeminiProvider.isBusy) ? Theme.warning : Theme.accent

                        Text {
                            anchors.centerIn: parent
                            text: ((typeof GeminiProvider !== "undefined" && GeminiProvider) && GeminiProvider.isBusy) ? "..." : "Send"
                            color: "#FFFFFF"
                            font.bold: true
                            font.pixelSize: Theme.fontCaption
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (promptInput.text) {
                                    if (typeof DyndiController !== "undefined" && DyndiController) {
                                        DyndiController.setAssistantState(promptInput.text, "Processing...")
                                    }
                                    if (typeof GeminiProvider !== "undefined" && GeminiProvider) {
                                        GeminiProvider.sendPrompt(promptInput.text)
                                    }
                                    promptInput.text = ""
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
