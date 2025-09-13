#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QString>

class QLabel;

/**
 * @author Harshi Kamboj
 * @class Sidebar
 * @brief A vertical panel containing user info and category buttons.
 *
 * Provides navigation through file categories (e.g., Images, Videos, Documents).
 * Emits a signal when a category is selected.
 */
class Sidebar : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the sidebar widget with optional parent.
     * @param parent The parent widget.
     */
    explicit Sidebar(QWidget *parent = nullptr);

signals:
    /**
     * @brief Signal emitted when a category is selected.
     * @param category The selected category name.
     */
    void categorySelected(const QString &category);

private:
    /**
     * @brief Adds the user avatar, name, and handle to the top of the sidebar.
     */
    void createUserInfo();

    /**
     * @brief Creates clickable buttons for file categories.
     */
    void createCategoryButtons();
};

#endif // SIDEBAR_H
