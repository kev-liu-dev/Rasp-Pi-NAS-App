#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }  // This expects your UI file to generate ui_LoginWindow.h
QT_END_NAMESPACE

/**
 * @author kevin liu
 * @brief Loginwindow class for the app
 */
class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief sets up the Login Window
     * @param parent optional parent widget
     */
    explicit LoginWindow(QWidget *parent = nullptr);

    /**
     * @brief cleans up ui stuff
     */
    ~LoginWindow();

signals:
    void loginSuccessful();

private slots:
    /**
     * @brief handles login button click
     */
    void on_loginButton_clicked();

    /**
     * @brief switches to forgot password page
     */
    void on_forgotButton_clicked();

    /**
     * @brief switches to help page
     */
    void on_helpButton_clicked();

    /**
     * @brief goes back to main page
     */
    void on_backButton_2_clicked();

    /**
     * @brief goes back to main page
     */
    void on_backButton_clicked();

    /**
     * @brief goes back to main page
     */
    void on_backButton_3_clicked();

    /**
     * @brief handles password retrieval
     */
    void on_retrieveButton_clicked();

    /**
    * @brief signs up user and switches to main window
    */
    void on_signupButton_clicked();

    /**
    * @brief signs up user and switches to main window
    */
    void on_signupPageButton_clicked();

    /**
    * @brief goes back to main page from signup page
    */
    void on_backButton_7_clicked();


private:
    Ui::LoginWindow *ui; ///< ui stuff
    QStackedWidget *stackedWidget;///< widget for managing pages
};

#endif // LOGINWINDOW_H
