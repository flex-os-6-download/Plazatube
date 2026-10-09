import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: settingsRoot
    width: Math.min(parent ? parent.width - 40 : 800, 800)
    height: Math.min(parent ? parent.height - 80 : 540, 540)
    radius: Theme.radiusLarge
    color: Theme.surface
    border.color: Theme.border
    border.width: 1

    property string currentTab: "Appearance"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Settings Window Titlebar
        Rectangle {
            Layout.fillWidth: true
            height: DesignTokens.windowHeaderHeight
            color: Qt.rgba(Theme.bg.r, Theme.bg.g, Theme.bg.b, 0.5)

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: Theme.spacingMD
                anchors.rightMargin: Theme.spacingMD

                Text {
                    text: "FlexOS Settings"
                    font.bold: true
                    font.pixelSize: Theme.fontBody
                    color: Theme.textPrimary
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    Layout.preferredWidth: 24
                    Layout.preferredHeight: 24
                    radius: 12
                    color: closeSettingsMouse.containsMouse ? Theme.error : "transparent"

                    Text {
                        anchors.centerIn: parent
                        text: "✕"
                        color: Theme.textPrimary
                    }

                    MouseArea {
                        id: closeSettingsMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: settingsRoot.visible = false
                    }
                }
            }
        }

        // Main Content Area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Sidebar Tabs
            Rectangle {
                Layout.preferredWidth: 200
                Layout.fillHeight: true
                color: Theme.bg
                border.color: Theme.border

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.spacingSM
                    spacing: Theme.spacingXS

                    Repeater {
                        model: ["Appearance", "Display", "Sound", "Network", "Gemini AI", "About FlexOS"]

                        delegate: Rectangle {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 38
                            radius: Theme.radiusSmall
                            color: currentTab === modelData ? Theme.accent : (tabMouse.containsMouse ? Qt.rgba(Theme.border.r, Theme.border.g, Theme.border.b, 0.3) : "transparent")

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                color: currentTab === modelData ? "#FFFFFF" : Theme.textPrimary
                                font.bold: currentTab === modelData
                                font.pixelSize: Theme.fontBody
                            }

                            MouseArea {
                                id: tabMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: currentTab = modelData
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }

            // Tab Page Content Area
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Theme.surface

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: Theme.spacingLG
                    spacing: Theme.spacingMD

                    Text {
                        text: currentTab + " Settings"
                        font.pixelSize: Theme.fontHeader
                        font.bold: true
                        color: Theme.textPrimary
                    }

                    // Appearance Page
                    ColumnLayout {
                        visible: currentTab === "Appearance"
                        spacing: Theme.spacingMD
                        Layout.fillWidth: true

                        RowLayout {
                            Layout.fillWidth: true
                            Text { text: "Dark Mode Appearance:"; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
                            Item { Layout.fillWidth: true }
                            Switch {
                                checked: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.isDarkMode : true
                                onCheckedChanged: {
                                    if (typeof ThemeManager !== "undefined" && ThemeManager) {
                                        ThemeManager.setDarkMode(checked)
                                    }
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Text { text: "Interface Scaling:"; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
                            Item { Layout.fillWidth: true }
                            Text { text: Math.round(((typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.scaleFactor : 1.0) * 100) + "%"; color: Theme.textSecondary; font.pixelSize: Theme.fontBody }
                            Button {
                                text: "-"
                                onClicked: {
                                    if (typeof ThemeManager !== "undefined" && ThemeManager) {
                                        ThemeManager.setScaleFactor(ThemeManager.scaleFactor - 0.1)
                                    }
                                }
                            }
                            Button {
                                text: "+"
                                onClicked: {
                                    if (typeof ThemeManager !== "undefined" && ThemeManager) {
                                        ThemeManager.setScaleFactor(ThemeManager.scaleFactor + 0.1)
                                    }
                                }
                            }
                        }

                        Button {
                            text: "Reset Visual Defaults"
                            onClicked: {
                                if (typeof ThemeManager !== "undefined" && ThemeManager) {
                                    ThemeManager.resetToDefaults()
                                }
                            }
                        }
                    }

                    // Gemini AI Page
                    ColumnLayout {
                        visible: currentTab === "Gemini AI"
                        spacing: Theme.spacingMD
                        Layout.fillWidth: true

                        Text { text: "Google Gemini API Configuration"; color: Theme.textPrimary; font.bold: true; font.pixelSize: Theme.fontBody }

                        RowLayout {
                            Layout.fillWidth: true
                            Text { text: "API Key:"; color: Theme.textPrimary; font.pixelSize: Theme.fontBody }
                            TextInput {
                                id: apiKeyInput
                                Layout.fillWidth: true
                                color: Theme.textPrimary
                                font.pixelSize: Theme.fontBody
                                echoMode: TextInput.Password
                                text: ""
                                Rectangle {
                                    anchors.fill: parent
                                    color: "transparent"
                                    border.color: Theme.border
                                    radius: Theme.radiusSmall
                                    z: -1
                                }
                            }
                            Button {
                                text: "Save Key"
                                onClicked: {
                                    if (typeof GeminiProvider !== "undefined" && GeminiProvider) {
                                        GeminiProvider.setApiKey(apiKeyInput.text)
                                    }
                                }
                            }
                        }

                        Text {
                            text: "Note: When no key is set, DYNDi Gemini assistant runs in offline response mode."
                            color: Theme.textSecondary
                            font.pixelSize: Theme.fontCaption
                            wrapMode: Text.WordWrap
                            Layout.fillWidth: true
                        }
                    }

                    // About Page
                    ColumnLayout {
                        visible: currentTab === "About FlexOS"
                        spacing: Theme.spacingSM
                        Layout.fillWidth: true

                        Text { text: "FlexOS Operating System Shell"; font.bold: true; font.pixelSize: Theme.fontTitle; color: Theme.textPrimary }
                        Text { text: "Version: 1.0.0-alpha"; color: Theme.textSecondary; font.pixelSize: Theme.fontBody }
                        Text { text: "Kernel: " + ((typeof SystemStatusService !== "undefined" && SystemStatusService) ? SystemStatusService.kernelVersion : "Linux 6.x"); color: Theme.textSecondary; font.pixelSize: Theme.fontBody }
                        Text { text: "Architecture: " + ((typeof SystemStatusService !== "undefined" && SystemStatusService) ? SystemStatusService.architecture : "x86_64 / arm64"); color: Theme.textSecondary; font.pixelSize: Theme.fontBody }
                        Text { text: "License: MIT Open Source License"; color: Theme.textSecondary; font.pixelSize: Theme.fontBody }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
