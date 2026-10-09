#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QDebug>

#include "ThemeManager.hpp"
#include "SettingsService.hpp"
#include "WindowManager.hpp"
#include "AppLauncherService.hpp"
#include "DyndiController.hpp"
#include "SystemStatusService.hpp"
#include "GeminiProvider.hpp"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName("FlexOS");
    app.setApplicationName("FlexOS Desktop Shell");

    QQmlApplicationEngine engine;

    // Instantiate core C++ service singletons
    ThemeManager themeManager;
    SettingsService settingsService;
    WindowManager windowManager;
    AppLauncherService appLauncherService;
    DyndiController dyndiController;
    SystemStatusService systemStatusService;
    GeminiProvider geminiProvider;

    // Expose core singletons to QML context
    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("ThemeManager", &themeManager);
    rootContext->setContextProperty("SettingsService", &settingsService);
    rootContext->setContextProperty("WindowManager", &windowManager);
    rootContext->setContextProperty("AppLauncherService", &appLauncherService);
    rootContext->setContextProperty("DyndiController", &dyndiController);
    rootContext->setContextProperty("SystemStatusService", &systemStatusService);
    rootContext->setContextProperty("GeminiProvider", &geminiProvider);

    const QUrl url(QStringLiteral("qrc:/src/ui/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                         if (!obj && url == objUrl)
                             QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
