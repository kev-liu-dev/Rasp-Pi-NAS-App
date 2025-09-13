#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>

class QPushButton;
class QMenu;
class QLabel;

/**
 * @author Harshi Kamboj
 * @class Toolbar
 * @brief A widget that provides file operation tools like rename, delete, sort, upload, and selection toggles.
 *
 * This toolbar is responsive to selection changes and emits signals for user-triggered actions.
 */
class Toolbar : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the Toolbar widget.
     * @param parent Optional parent widget.
     */
    explicit Toolbar(QWidget *parent = nullptr);

signals:
signals:
    /**
     * @brief Emitted when the user requests to rename a file.
     */
    void renameRequested();

    /**
     * @brief Emitted when the user requests to delete selected files.
     */
    void deleteRequested();

    /**
     * @brief Emitted when a sort option is chosen from the dropdown menu.
     * @param sortCriteria An integer representing the sort option.
     */
    void sortRequested(int sortCriteria);

    /**
     * @brief Emitted when "Select All" is toggled.
     */
    void selectAllToggled();

    /**
     * @brief Emitted when the upload button is clicked.
     */
    void uploadRequested();

    /**
     * @brief Emitted when the download button is clicked
     */
    void downloadRequested();

public slots:
    /**
     * @brief Updates the UI and internal state when the file selection changes.
     * @param selectedCount Number of selected files.
     * @param allSelected True if all visible files are selected.
     * @param noneSelected True if no files are selected.
     */
    void onSelectionInfoChanged(int selectedCount, bool allSelected, bool noneSelected);

private slots:
    /**
     * @brief Slot triggered when the "Select All" button is clicked.
     */
    void onSelectAllClicked();

private:
    QPushButton *renameBtn;     ///< Button to rename a file
    QPushButton *deleteBtn;     ///< Button to delete selected files
    QPushButton *sortBtn;       ///< Button to open sort menu
    QPushButton *uploadBtn;     ///< Button to upload a new file
    QPushButton *downloadBtn;   ///< Button to download selected file
    QPushButton *selectAllBtn;  ///< Button to toggle all file selections

    QLabel *selectedCountLabel; ///< Displays the number of selected files

    int currentSelectedCount = 0; ///< Currently selected file count
    bool isAllSelected = false;   ///< Whether all files are selected
    bool isNoneSelected = true;   ///< Whether no files are selected

    /**
     * @brief Updates enabled/disabled states of rename and delete buttons.
     */
    void updateButtonStates();
};

#endif // TOOLBAR_H
