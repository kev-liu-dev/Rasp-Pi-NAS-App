#include <QApplication>
#include "LoginWindow.h"    // Custom login window
#include "MainWindow.h"     // Main application window

/**
 * @author Harshi Kamboj
 * @brief Entry point for the Local Drive Client application.
 *
 * Initializes the Qt application, launches the login window,
 * and displays the main window upon successful login.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Application exit status code.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // Qt application instance

    // Create and show the login window first
    LoginWindow *loginWin = new LoginWindow();
    loginWin->show();

    /**
     * @brief Lambda function to launch the main window after login.
     *
     * Connects the login success signal to the main UI display logic.
     */
    QObject::connect(loginWin, &LoginWindow::loginSuccessful, [=]() {
        MainWindow *mainWin = new MainWindow();
        mainWin->loadUserPreferences(); // Load user-specific settings (placeholder)
        mainWin->showWindow();          // Show the main app UI
        loginWin->close();              // Close the login window
    });

    return app.exec(); // Start Qt event loop
}
