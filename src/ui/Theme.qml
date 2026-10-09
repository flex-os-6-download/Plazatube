import QtQuick

pragma Singleton

Item {
    // Colors derived safely from ThemeManager
    readonly property color bg: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.backgroundColor : "#0F172A"
    readonly property color surface: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.surfaceColor : "#1E293B"
    readonly property color textPrimary: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.primaryTextColor : "#F8FAFC"
    readonly property color textSecondary: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.secondaryTextColor : "#94A3B8"
    readonly property color accent: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.accentColor : "#3B82F6"
    readonly property color border: (typeof ThemeManager !== "undefined" && ThemeManager) ? ThemeManager.borderColor : "#334155"

    // Functional colors
    readonly property color success: "#10B981"
    readonly property color warning: "#F59E0B"
    readonly property color error: "#EF4444"
    readonly property color overlay: "#80000000"

    // Geometry & Radius Design Tokens
    readonly property int radiusSmall: 6
    readonly property int radiusMedium: 12
    readonly property int radiusLarge: 20
    readonly property int radiusPill: 999

    // Spacing Design Tokens
    readonly property int spacingXS: 4
    readonly property int spacingSM: 8
    readonly property int spacingMD: 16
    readonly property int spacingLG: 24
    readonly property int spacingXL: 32

    // Animation Durations
    readonly property int animFast: (typeof SettingsService !== "undefined" && SettingsService && SettingsService.reduceMotion) ? 0 : 150
    readonly property int animNormal: (typeof SettingsService !== "undefined" && SettingsService && SettingsService.reduceMotion) ? 0 : 250
    readonly property int animSlow: (typeof SettingsService !== "undefined" && SettingsService && SettingsService.reduceMotion) ? 0 : 400

    // Font Sizes
    readonly property int fontCaption: 11
    readonly property int fontBody: 14
    readonly property int fontTitle: 18
    readonly property int fontHeader: 24
}
