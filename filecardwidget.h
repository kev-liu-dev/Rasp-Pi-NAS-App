#ifndef FILECARDWIDGET_H
#define FILECARDWIDGET_H

#include <QFrame>
#include "MainWindow.h"

class QPushButton;

/**
 * @author Harshi Kamboj
 * @class FileCardWidget
 * @brief Represents a UI widget card for displaying file metadata such as name, icon, and modified date.
 *
 * This widget provides functionality to mark files as favorite or select them. It emits signals
 * when these states change, which can be handled by higher-level components like file views.
 */
class FileCardWidget : public QFrame
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a FileCardWidget with reference to file data and its index.
     * @param data Reference to the file's metadata.
     * @param index Index of the file in the master file list.
     * @param parent Optional parent widget.
     */
    explicit FileCardWidget(FileData &data, int index, QWidget *parent = nullptr);

signals:
    /**
     * @brief Emitted when the favorite button is toggled.
     * @param fileIndex Index of the toggled file.
     * @param isFav New favorite state.
     */
    void favoriteToggled(int fileIndex, bool isFav);

    /**
     * @brief Emitted when the select button is toggled.
     * @param fileIndex Index of the toggled file.
     * @param isSelected New selection state.
     */
    void selectedToggled(int fileIndex, bool isSelected);

private slots:
    /**
     * @brief Slot triggered when favorite button is clicked.
     * Toggles favorite state and emits signal.
     */
    void onFavoriteClicked();

    /**
     * @brief Slot triggered when select button is clicked.
     * Toggles selection state and emits signal.
     */
    void onSelectClicked();

private:
    FileData &m_file;          ///< Reference to the file's data
    int m_index;               ///< Index of the file in the global list

    QPushButton *favoriteBtn;  ///< Button to toggle favorite state
    QPushButton *selectBtn;    ///< Button to toggle selection state

    /**
     * @brief Updates button labels to reflect the current file state.
     */
    void updateButtons();
};

#endif
