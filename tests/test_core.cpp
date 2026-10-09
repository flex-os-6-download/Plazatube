#include <QtTest>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ThemeManager.hpp"
#include "SettingsService.hpp"
#include "WindowManager.hpp"
#include "AppLauncherService.hpp"
#include "DyndiController.hpp"
#include "SystemStatusService.hpp"
#include "GeminiProvider.hpp"

class TestCore : public QObject {
    Q_OBJECT

private slots:
    void testThemeManager();
    void testSettingsService();
    void testWindowManager();
    void testAppLauncherService();
    void testDyndiController();
    void testSystemStatusService();
    void testGeminiProvider();
    void testOffscreenQmlShellLoading();
};

void TestCore::testThemeManager() {
    ThemeManager theme;
    QCOMPARE(theme.isDarkMode(), true);
    theme.setDarkMode(false);
    QCOMPARE(theme.isDarkMode(), false);
    QCOMPARE(theme.backgroundColor(), QColor("#F8FAFC"));

    theme.setScaleFactor(1.5);
    QCOMPARE(theme.scaleFactor(), 1.5);

    theme.resetToDefaults();
    QCOMPARE(theme.isDarkMode(), true);
    QCOMPARE(theme.scaleFactor(), 1.0);
}

void TestCore::testSettingsService() {
    SettingsService settings;
    settings.setReduceMotion(false);
    settings.setAnimationsEnabled(true);
    QCOMPARE(settings.animationsEnabled(), true);

    settings.setReduceMotion(true);
    QCOMPARE(settings.reduceMotion(), true);
    QCOMPARE(settings.animationsEnabled(), false);

    settings.setHostname("flexos-test-node");
    QCOMPARE(settings.hostname(), QString("flexos-test-node"));
}

void TestCore::testWindowManager() {
    WindowManager wm;
    QCOMPARE(wm.windows().size(), 0);

    int win1 = wm.openWindow("Test Window 1", "test.app1");
    QVERIFY(win1 > 0);
    QCOMPARE(wm.windows().size(), 1);

    int win2 = wm.openWindow("Test Window 2", "test.app2");
    QCOMPARE(wm.windows().size(), 2);
    QCOMPARE(wm.activeWindowId(), win2);

    wm.minimizeWindow(win2);
    QCOMPARE(wm.activeWindowId(), -1);

    wm.restoreWindow(win2);
    QCOMPARE(wm.activeWindowId(), win2);

    wm.closeWindow(win1);
    QCOMPARE(wm.windows().size(), 1);

    wm.closeWindow(win2);
    QCOMPARE(wm.windows().size(), 0);
}

void TestCore::testAppLauncherService() {
    AppLauncherService launcher;
    QVERIFY(launcher.applications().size() >= 3); // Includes built-in FlexOS default apps

    auto results = launcher.searchApplications("Firefox");
    QVERIFY(results.size() >= 1);
}

void TestCore::testDyndiController() {
    DyndiController dyndi;
    QCOMPARE(dyndi.state(), QString("Compact"));

    dyndi.expand();
    QCOMPARE(dyndi.state(), QString("Expanded"));

    dyndi.collapse();
    QCOMPARE(dyndi.state(), QString("Compact"));

    dyndi.pushNotification("Test Event", "System notice message");
    QCOMPARE(dyndi.state(), QString("Notification"));
    QCOMPARE(dyndi.hasPendingNotification(), true);

    dyndi.dismissNotification();
    QCOMPARE(dyndi.hasPendingNotification(), false);
}

void TestCore::testSystemStatusService() {
    SystemStatusService status;
    QVERIFY(status.batteryLevel() >= 0 && status.batteryLevel() <= 100);
    QVERIFY(!status.kernelVersion().isEmpty());
    QVERIFY(!status.architecture().isEmpty());

    status.setVolumeLevel(65);
    QCOMPARE(status.volumeLevel(), 65);
}

void TestCore::testGeminiProvider() {
    GeminiProvider gemini;
    QCOMPARE(gemini.isBusy(), false);

    gemini.sendPrompt("Hello Gemini");
    QVERIFY(gemini.conversationHistory().size() >= 1);
}

void TestCore::testOffscreenQmlShellLoading() {
    QQmlApplicationEngine engine;

    ThemeManager themeManager;
    SettingsService settingsService;
    WindowManager windowManager;
    AppLauncherService appLauncherService;
    DyndiController dyndiController;
    SystemStatusService systemStatusService;
    GeminiProvider geminiProvider;

    engine.rootContext()->setContextProperty("ThemeManager", &themeManager);
    engine.rootContext()->setContextProperty("SettingsService", &settingsService);
    engine.rootContext()->setContextProperty("WindowManager", &windowManager);
    engine.rootContext()->setContextProperty("AppLauncherService", &appLauncherService);
    engine.rootContext()->setContextProperty("DyndiController", &dyndiController);
    engine.rootContext()->setContextProperty("SystemStatusService", &systemStatusService);
    engine.rootContext()->setContextProperty("GeminiProvider", &geminiProvider);

    engine.load(QUrl("qrc:/src/ui/Main.qml"));
    QCOMPARE(engine.rootObjects().isEmpty(), false);
}

QTEST_MAIN(TestCore)
#include "test_core.moc"
