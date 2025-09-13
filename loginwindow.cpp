#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "MainWindow.h"
#include "apiLogin.h"
#include <QMessageBox>
#include <QStackedWidget>
#include <QVBoxLayout>

/**
 * @class LoginWindow
 * @author Kevin Liu
 * @brief sets up the login window
 * @details This class holds the login, signup, help, and forgot password pages.
 * It uses QStackedWidget to organize all these pages within the stacked widget.
 * It communicates with a backend API to verify credentials for each user.
 * @param parent optional pointer to the parent QWidget container
 */
LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);

    stackedWidget->addWidget(ui->centralwidget);

    /**
     * @brief adds help page (index 1) to stacked widget
     */
    QWidget *page2 = ui->page2;
    stackedWidget->addWidget(page2);

    /**
     * @brief adds login success page (index 2) to stacked widget
     */
    QWidget *page3 = ui->page3;
    stackedWidget->addWidget(page3);

    /**
     * @brief adds forgot password page (index 3) to stacked widget
     */
    QWidget *page4 = ui->page4;
    stackedWidget->addWidget(page4);

    /**
     * @brief adds signup page (index 4) to stacked widget
     */
    QWidget *page5 = ui->page5;
    stackedWidget->addWidget(page5);

    setCentralWidget(stackedWidget);
}

/**
 * @brief cleans up ui when closing
 * @details deallocates memory by cleaning up the UI, and is automatically called when the window closes
 */
LoginWindow::~LoginWindow()
{
    delete ui;
}

/**
 * @brief opens help page
 * @details switches to the help page which has index 1, from the login page
 */
void LoginWindow::on_helpButton_clicked()
{
    stackedWidget->setCurrentIndex(1);
}

/**
 * @brief exit help page
 * @details returns to main login back from the help page
 */
void LoginWindow::on_backButton_2_clicked()
{
    stackedWidget->setCurrentIndex(0);
}

/**
 * @brief login button click event
 * @details checks login info, shows success or error with API
 * calls the API login function to verify credentials against database
 * displays error message if failed
 * @param username inputted by user from login page (for loginSuccess function within this function)
 * @param password inputted by user from login page (for loginSuccess function within this function)
 */
void LoginWindow::on_loginButton_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    bool loginSuccess = login(username.toStdString(), password.toStdString());
    if (loginSuccess) {
        QMessageBox::information(this, "Login Successful", "Welcome!");
        // Create and show MainWindow, then close the login window.
        MainWindow *mainWin = new MainWindow();
        mainWin->showWindow();
        emit loginSuccessful();
        this->close();
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password");
    }
}


/**
 * @brief goes back to main page from forgot password
 * @details switches to the login page from forgot password page
 */
void LoginWindow::on_backButton_3_clicked()
{
    stackedWidget->setCurrentIndex(0);
}

/**
 * @brief forget password event
 * @details user enters username and the progam attempts to recover the users password through API by calling the
 * restore_password API function
 * @param username the username the user enters
 */
void LoginWindow::on_retrieveButton_clicked()
{
    QString username = ui->NameLineEdit_2->text();


    std::string password = restore_password(username.toStdString());
    if (!password.empty()) {
        QMessageBox::information(this, "Password Retrieved",
                                 QString("Your password is: %1").arg(QString::fromStdString(password)));
    } else {
        QMessageBox::warning(this, "Error", "Username not found");
    }
}

/**
 * @brief opens forgot password page
 */
void LoginWindow::on_forgotButton_clicked()
{
    stackedWidget->setCurrentIndex(3);
}


/**
 * @brief button that switches to sign up page
 */
void LoginWindow::on_signupPageButton_clicked()
{
    stackedWidget->setCurrentIndex(4);
}



/**
 * @brief sign up button evnet
 * @details calls the API login function to add credentials to database.
 * @param username the user's inputted username
 * @param password the user's inputted password

 */

void LoginWindow::on_signupButton_clicked()
{

    QString username = ui->usernameLineEdit_signup->text();
    QString password = ui->passwordLineEdit_signup->text();

    bool signupSuccess = signup(username.toStdString(), password.toStdString());

    if (signupSuccess) {
        QMessageBox::information(this, "Signup Successful", "Account created!");
        stackedWidget->setCurrentIndex(2);
    } else {
        QMessageBox::warning(this, "Signup Failed", "Username already exists");
    }
}

/**
 * @brief goes back to login page from the success login page
 */
void LoginWindow::on_backButton_clicked()
{
    stackedWidget->setCurrentIndex(0);
}



/**
 * @brief goes back to main login page from sign up page
 */
void LoginWindow::on_backButton_7_clicked()
{
    stackedWidget->setCurrentIndex(0);
}
