# FlexOS Operating System

![FlexOS Architecture](https://img.shields.io/badge/FlexOS-Linux%20OS%20Shell-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Build](https://img.shields.io/badge/Build-Qt6%20%7C%20CMake-orange)

FlexOS is an open-source Linux-based operating system desktop shell, window management interface, and system engine built in C++17 and Qt 6 / QML.

## Features & Architecture

- **Native C++ Engine**: High-performance backend components (`ThemeManager`, `WindowManager`, `DyndiController`, `AppLauncherService`, `SettingsService`, `SystemStatusService`, `GeminiProvider`).
- **Original Visual Design**:
  - Dark/Light Theme Engine with centralized design tokens (`Theme.qml`, `DesignTokens.qml`).
  - Interactive Taskbar with live clock, battery, network, app list, launcher trigger, and settings.
  - Distinctive Window Frame & Decorations with move, resize, minimize, maximize, and focus handling.
- **DYNDi Interface**: Expandable UI for notifications, system activity, and integrated Google Gemini AI assistant (with offline fallback).
- **Application Launcher**: Real XDG `.desktop` file parser scanning `/usr/share/applications` and spawning processes via `QProcess`.
- **System Settings**: Multi-category GUI for visual adjustments, scale factors, and Gemini API key management.

## Prerequisites & Installation

On Ubuntu/Debian Linux systems:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake qt6-base-dev qt6-declarative-dev \
  qml6-module-qtqml qml6-module-qtquick qml6-module-qtquick-controls \
  qml6-module-qtquick-layouts qml6-module-qtquick-window qml6-module-qtquick-templates
```

## Building & Testing

Using the custom build tool:

```bash
/home/jules/self_created_tools/builder_tool.py --test
```

Or using standard CMake:

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
QT_QPA_PLATFORM=offscreen ctest --output-on-failure
```

To run the FlexOS desktop shell executable:

```bash
./build/flexos_shell
```

## License

This project is licensed under the [MIT License](LICENSE).
