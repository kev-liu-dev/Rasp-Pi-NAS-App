#ifndef FILEHIERARCHYVIEW_H
#define FILEHIERARCHYVIEW_H

#include <QWidget>
#include "MainWindow.h"
#include <QSet>

class QStackedWidget;

/**
 * @author Harshi Kamboj
 * @class FileHierarchyView
 * @brief A widget that displays files categorized by type and supports sorting, selection, and search filtering.
 *
 * Acts as the main dynamic container for all file previews and interactions, such as selecting,
 * renaming, and deleting files, and filtering by category or search.
 */
class FileHierarchyView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the FileHierarchyView widget.
     * @param parent The parent QWidget.
     */
    explicit FileHierarchyView(QWidget *parent = nullptr);

    /**
     * @brief Sets the global file data pointer.
     * @param files Pointer to the master file list.
     */
    void setFileData(QList<FileData> *files);

    /**
     * @brief Sets the current file category (e.g., Images, Videos).
     * @param category The category name.
     */
    void setCategory(const QString &category);

    /**
     * @brief Rebuilds the view after any change in state or filters.
     */
    void rebuild();

    /**
     * @brief Sorts the current view based on specified criteria.
     * @param criteria Sorting strategy to apply.
     */
    void sort(SortCriteria criteria);

    /**
     * @brief Triggers a full UI refresh manually.
     */
    void updateView();

public slots:
    /**
     * @brief Updates the search term used for filtering files.
     * @param term The search keyword.
     */
    void setSearchTerm(const QString &term);

signals:
    /**
     * @brief Emitted when selection information changes to update the toolbar.
     * @param selectedCount Number of selected files.
     * @param allSelected True if all are selected.
     * @param noneSelected True if none are selected.
     */
    void selectionInfoChanged(int selectedCount, bool allSelected, bool noneSelected);

public slots:
    /**
     * @brief Toggles selection state for all visible files.
     */
    void onSelectAllToggled();

    /**
     * @brief Triggers a rename dialog for a single selected file.
     */
    void onRenameRequested();

    /**
     * @brief Deletes the currently selected files.
     */
    void onDeleteRequested();

    /**
     * @brief Handles favorite toggle from a FileCard.
     * @param fileIndex Index of the toggled file.
     * @param isFav Whether the file is now marked as favorite.
     */
    void fileFavoriteToggled(int fileIndex, bool isFav);

    /**
     * @brief Handles selection toggle from a FileCard.
     * @param fileIndex Index of the toggled file.
     * @param isSelected Whether the file is now selected.
     */
    void fileSelectedToggled(int fileIndex, bool isSelected);

private:
    QStackedWidget *stackedWidget;     ///< Holds the file pages by category
    QList<FileData> *allFiles;           ///< Pointer to the master list of files
    QString currentCategory;             ///< Current file category
    QString searchTerm;                  ///< Current search input for filtering

    /**
     * @brief Generates a UI page for the given list of files.
     * @param files The filtered list of files.
     * @return A QWidget representing the visual layout.
     */
    QWidget* createCategoryPage(const QList<FileData> &files);

    /**
     * @brief Filters the global file list by category and search term.
     * @param category Category to filter by.
     * @return List of matching files.
     */
    QList<FileData> filterFilesByCategory(const QString &category) const;

    /**
     * @brief Updates the toolbar with the current selection count.
     */
    void updateSelectionInfo();

    /**
     * @brief Retrieves the global indices for files shown on the current page.
     * @return A list of global indices.
     */
    QList<int> getCurrentPageFileIndices() const;
};

#endif
