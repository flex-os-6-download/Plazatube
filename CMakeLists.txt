cmake_minimum_required(VERSION 3.16)

project(FlexOS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Test)

include_directories(src/core)

# Core library source files
set(CORE_SOURCES
    src/core/ThemeManager.cpp
    src/core/SettingsService.cpp
    src/core/WindowManager.cpp
    src/core/AppLauncherService.cpp
    src/core/DyndiController.cpp
    src/core/SystemStatusService.cpp
    src/core/GeminiProvider.cpp
)

set(CORE_HEADERS
    src/core/ThemeManager.hpp
    src/core/SettingsService.hpp
    src/core/WindowManager.hpp
    src/core/AppLauncherService.hpp
    src/core/DyndiController.hpp
    src/core/SystemStatusService.hpp
    src/core/GeminiProvider.hpp
)

add_library(flexos_core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(flexos_core PUBLIC Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick Qt6::Network)

# QML resources
qt_add_resources(flexos_core "qml_resources"
    PREFIX "/"
    FILES
        src/ui/Main.qml
        src/ui/Theme.qml
        src/ui/DesignTokens.qml
        src/ui/DesktopShell.qml
        src/ui/Taskbar.qml
        src/ui/WindowFrame.qml
        src/ui/WindowDecoration.qml
        src/ui/AppLauncher.qml
        src/ui/DYNDi.qml
        src/ui/SettingsApp.qml
)

# FlexOS Desktop Shell Executable
add_executable(flexos_shell src/main.cpp)
target_link_libraries(flexos_shell PRIVATE flexos_core Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)

# FlexOS Unit & Integration Tests
enable_testing()
add_executable(flexos_tests tests/test_core.cpp)
target_link_libraries(flexos_tests PRIVATE flexos_core Qt6::Test Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick)
add_test(NAME flexos_tests COMMAND flexos_tests)
