#include "Sidebar.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QFrame>
#include <QPixmap>

/**
 * @author Harshi Kamboj
 * @brief Constructs the Sidebar widget.
 * Initializes layout and fills with user info and category buttons.
 */
Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent)
{
    // Set sidebar background color
    setStyleSheet("background-color: #FFFFFF;");

    // Vertical layout for stacking elements
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // Add avatar and user info
    createUserInfo();

    // Add file category buttons
    createCategoryButtons();

    // Push everything to the top, leaving space at bottom
    layout->addStretch();

    setLayout(layout);
}

/**
 * @brief Creates avatar and admin label area at the top of the sidebar.
 */
void Sidebar::createUserInfo()
{
    // User avatar image
    QLabel *avatarLabel = new QLabel(this);
    QPixmap avatarPix(":/icons/avatar.png");
    avatarPix = avatarPix.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    avatarLabel->setPixmap(avatarPix);
    avatarLabel->setAlignment(Qt::AlignHCenter);

    // Display user name
    QLabel *userName = new QLabel("Admin", this);
    userName->setStyleSheet("font-weight: bold; font-size: 16px; color: #000000;");
    userName->setAlignment(Qt::AlignHCenter);

    // Display user handle/tag
    QLabel *userHandle = new QLabel("@adminuser", this);
    userHandle->setStyleSheet("color: #777; font-size: 12px;");
    userHandle->setAlignment(Qt::AlignHCenter);

    // Add all widgets to the layout
    QVBoxLayout *vbox = qobject_cast<QVBoxLayout*>(layout());
    vbox->addWidget(avatarLabel);
    vbox->addWidget(userName);
    vbox->addWidget(userHandle);
    vbox->addSpacing(20); // Space between avatar and categories
}

/**
 * @brief Creates file category navigation buttons and connects them to emit a signal.
 */
void Sidebar::createCategoryButtons()
{
    // File categories to display
    QStringList categories = { "All Files", "Images", "Videos", "Music", "Documents", "Favorites", "Other" };
    QVBoxLayout *vbox = qobject_cast<QVBoxLayout*>(layout());

    // Style for all category buttons
    QString buttonStyle = R"(
        QPushButton {
            background-color: transparent;
            color: #000000;
            font-size: 14px;
            text-align: left;
            border: none;
            padding: 4px 0;
        }
        QPushButton:hover {
            background-color: #E6E6E6;
        }
    )";

    // Create a button for each category and connect signal
    for (const QString &cat : categories) {
        QPushButton *btn = new QPushButton(cat, this);
        btn->setStyleSheet(buttonStyle);
        vbox->addWidget(btn);

        // Emit signal when a button is clicked
        connect(btn, &QPushButton::clicked, this, [this, cat]() {
            emit categorySelected(cat);
        });
    }
}
