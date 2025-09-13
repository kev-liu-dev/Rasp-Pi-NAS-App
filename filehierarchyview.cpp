#include "FileHierarchyView.h"
#include "FileCardWidget.h"
#include "APIClient.h"
#include <QStackedWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <algorithm>

/**
 * @author Harshi Kamboj
 * @brief Constructs and initializes the FileHierarchyView.
 */
FileHierarchyView::FileHierarchyView(QWidget *parent)
    : QWidget(parent), allFiles(nullptr), currentCategory("All Files")
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    stackedWidget = new QStackedWidget(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    searchTerm.clear(); // No search term initially
}

/**
 * @brief Sets the global file list pointer.
 * @param files Pointer to the master list of files.
 */
void FileHierarchyView::setFileData(QList<FileData> *files)
{
    allFiles = files;
    rebuild(); // Refresh view with new file data
}

/**
 * @brief Updates the current file category and rebuilds the UI.
 * @param category The selected category.
 */
void FileHierarchyView::setCategory(const QString &category)
{
    currentCategory = category;
    rebuild(); // Refresh view for selected category
}

/**
 * @brief Updates the search filter term and refreshes the view.
 * @param term The search keyword.
 */
void FileHierarchyView::setSearchTerm(const QString &term)
{
    searchTerm = term;
    rebuild(); // Refresh view using updated search term
}

/**
 * @brief Filters the master file list based on the selected category and search term.
 * @param category The category to filter by.
 * @return A QList of FileData matching the filter.
 */
QList<FileData> FileHierarchyView::filterFilesByCategory(const QString &category) const
{
    if (!allFiles) return {};

    QList<FileData> result;

    // Category filtering
    if (category == "All Files") {
        result = *allFiles;
    } else if (category == "Favorites") {
        for (auto &f : *allFiles)
            if (f.isFavorite)
                result.append(f);
    } else if (category == "Images") {
        QStringList exts = { ".png", ".jpeg", ".jpg", ".gif", ".tiff", ".webp", ".svg", ".bmp", ".heif" };
        for (auto &f : *allFiles)
            if (exts.contains(f.extension.toLower()))
                result.append(f);
    } else if (category == "Videos") {
        QStringList exts = { ".avi", ".wmv", ".mp4", ".mkv", ".mov", ".avchd", ".flv", ".ogg" };
        for (auto &f : *allFiles)
            if (exts.contains(f.extension.toLower()))
                result.append(f);
    } else if (category == "Music") {
        QStringList exts = { ".wav", ".aiff", ".mp3", ".flac", ".aac", ".m4a", ".ogg" };
        for (auto &f : *allFiles)
            if (exts.contains(f.extension.toLower()))
                result.append(f);
    } else if (category == "Documents") {
        QStringList exts = { ".pdf", ".docx", ".pptx", ".xlsx", ".txt", ".html", ".rtf", ".csv", ".doc", ".ppt" };
        for (auto &f : *allFiles)
            if (exts.contains(f.extension.toLower()))
                result.append(f);
    } else if (category == "Other") {
        QStringList allKnown = { ".png", ".jpeg", ".jpg", ".gif", ".tiff", ".webp", ".svg", ".bmp", ".heif",
                                ".avi", ".wmv", ".mp4", ".mkv", ".mov", ".avchd", ".flv", ".ogg",
                                ".wav", ".aiff", ".mp3", ".flac", ".aac", ".m4a",
                                ".pdf", ".docx", ".pptx", ".xlsx", ".txt", ".html", ".rtf", ".csv", ".doc", ".ppt" };
        for (auto &f : *allFiles)
            if (!allKnown.contains(f.extension.toLower()))
                result.append(f);
    } else {
        result = *allFiles; // fallback
    }

    // Optional search filtering (partial match, case-insensitive)
    if (!searchTerm.isEmpty()) {
        QList<FileData> finalResult;
        for (auto &f : result) {
            if (f.fileName.toLower().contains(searchTerm.toLower())) {
                finalResult.append(f);
            }
        }
        return finalResult;
    }

    return result;
}

/**
 * @brief Creates a scrollable widget for the given file list.
 * @param files The filtered list of files.
 * @return A pointer to a QWidget representing the file grid.
 */
QWidget* FileHierarchyView::createCategoryPage(const QList<FileData> &files)
{
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);

    QWidget *container = new QWidget(scroll);
    QGridLayout *gridLayout = new QGridLayout(container);
    gridLayout->setSpacing(15);

    const int columns = 4;
    int row = 0, col = 0;

    for (int i = 0; i < files.size(); ++i) {
        int globalIndex = -1;

        // Find index in original list
        for (int j = 0; j < allFiles->size(); ++j) {
            if ((*allFiles)[j].fileName == files[i].fileName &&
                (*allFiles)[j].extension == files[i].extension)
            {
                globalIndex = j;
                break;
            }
        }

        if (globalIndex == -1)
            continue;

        // Create FileCard for file
        FileCardWidget *card = new FileCardWidget((*allFiles)[globalIndex], globalIndex, container);

        // Connect card signals to view
        connect(card, &FileCardWidget::favoriteToggled, this, &FileHierarchyView::fileFavoriteToggled);
        connect(card, &FileCardWidget::selectedToggled, this, &FileHierarchyView::fileSelectedToggled);

        gridLayout->addWidget(card, row, col);

        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }

    container->setLayout(gridLayout);
    scroll->setWidget(container);
    return scroll;
}

/**
 * @brief Clears and rebuilds the file view from scratch.
 */
void FileHierarchyView::rebuild()
{
    if (!stackedWidget)
        return;

    QWidget *old = stackedWidget->currentWidget();
    if (old) {
        stackedWidget->removeWidget(old);
        old->deleteLater();
    }

    QList<FileData> filtered = filterFilesByCategory(currentCategory);
    QWidget *page = createCategoryPage(filtered);

    if (page) {
        stackedWidget->addWidget(page);
        stackedWidget->setCurrentWidget(page);
    }

    updateSelectionInfo();
}

/**
 * @brief Sorts the current filtered files based on the specified criteria and rebuilds the layout.
 * @param criteria The sort criteria to apply.
 */
void FileHierarchyView::sort(SortCriteria criteria)
{
    QList<FileData> filtered = filterFilesByCategory(currentCategory);

    std::sort(filtered.begin(), filtered.end(), [criteria](const FileData &a, const FileData &b) {
        switch (criteria) {
        case SortCriteria::NameAsc:
            return a.fileName.toLower() < b.fileName.toLower();
        case SortCriteria::NameDesc:
            return a.fileName.toLower() > b.fileName.toLower();
        case SortCriteria::DateAsc:
            return a.dateModified < b.dateModified;
        case SortCriteria::DateDesc:
            return a.dateModified > b.dateModified;
        case SortCriteria::TypeAsc:
            return a.extension.toLower() < b.extension.toLower();
        case SortCriteria::TypeDesc:
            return a.extension.toLower() > b.extension.toLower();
        }
        return false;
    });

    if (!stackedWidget)
        return;

    QWidget *old = stackedWidget->currentWidget();
    if (old) {
        stackedWidget->removeWidget(old);
        old->deleteLater();
    }

    QWidget *page = createCategoryPage(filtered);
    if (page) {
        stackedWidget->addWidget(page);
        stackedWidget->setCurrentWidget(page);
    }

    updateSelectionInfo();
}

/**
 * @brief Public slot to refresh the view.
 */
void FileHierarchyView::updateView()
{
    rebuild(); // Trigger full refresh
}

/**
 * @brief Returns the global indices for files currently shown on the view.
 * @return A QList of indices corresponding to files in the master list.
 */
QList<int> FileHierarchyView::getCurrentPageFileIndices() const
{
    if (!allFiles) return {};
    QList<FileData> filtered = filterFilesByCategory(currentCategory);

    QList<int> result;
    for (auto &f : filtered) {
        for (int j = 0; j < allFiles->size(); ++j) {
            if ((*allFiles)[j].fileName == f.fileName &&
                (*allFiles)[j].extension == f.extension)
            {
                result.append(j);
                break;
            }
        }
    }
    return result;
}

/**
 * @brief Toggles selection state for all visible files.
 */
void FileHierarchyView::onSelectAllToggled()
{
    if (!allFiles) return;
    QList<int> indices = getCurrentPageFileIndices();
    if (indices.isEmpty()) return;

    // Check if all are selected.
    bool allSelected = true;
    for (int idx : indices) {
        if (!(*allFiles)[idx].isSelected) {
            allSelected = false;
            break;
        }
    }

    // Toggle selection state for all.
    for (int idx : indices) {
        (*allFiles)[idx].isSelected = !allSelected;
    }

    rebuild();
}

/**
 * @brief Triggers a rename dialog for a single selected file.
 *
 * Allows renaming only the file's base name (excluding its extension) and updates the API.
 */
void FileHierarchyView::onRenameRequested()
{
    if (!allFiles) return;
    QList<int> indices = getCurrentPageFileIndices();
    int countSelected = 0;
    int singleIndex = -1;

    for (int idx : indices) {
        if ((*allFiles)[idx].isSelected) {
            countSelected++;
            singleIndex = idx;
        }
    }

    if (countSelected == 1 && singleIndex >= 0) {
        bool ok;
        QString currentName = (*allFiles)[singleIndex].fileName;
        int dotIndex = currentName.lastIndexOf('.');
        QString baseName = (dotIndex != -1) ? currentName.left(dotIndex) : currentName;
        QString extension = (dotIndex != -1) ? currentName.mid(dotIndex) : "";

        QString newBaseName = QInputDialog::getText(
            this,
            "Rename File",
            "Enter new file name (without extension):",
            QLineEdit::Normal,
            baseName,
            &ok
            );

        if (ok && !newBaseName.isEmpty()) {
            QString newFullName = newBaseName + extension;
            APIClient apiClient;  // Create API client instance.
            bool apiSuccess = apiClient.renameFile(currentName, newFullName);
            if (apiSuccess) {
                (*allFiles)[singleIndex].fileName = newFullName;
                rebuild();
            } else {
                QMessageBox::warning(this, "Rename File", "Failed to rename file on the server.");
            }
        }
    }
}

/**
 * @brief Deletes all currently selected files from the list and updates the API.
 */
void FileHierarchyView::onDeleteRequested()
{
    if (!allFiles) return;
    QList<int> indices = getCurrentPageFileIndices();
    if (indices.isEmpty()) return;

    QList<int> indicesToRemove;
    APIClient apiClient;  // Create an API client instance.
    for (int idx : indices) {
        if ((*allFiles)[idx].isSelected) {
            bool apiSuccess = apiClient.deleteFile((*allFiles)[idx].fileName);
            if (apiSuccess) {
                indicesToRemove.append(idx);
            } else {
                QMessageBox::warning(this, "Delete File", "Failed to delete file on the server: " + (*allFiles)[idx].fileName);
            }
        }
    }

    if (indicesToRemove.isEmpty()) return;

    // Remove from highest index to avoid shifting.
    std::sort(indicesToRemove.begin(), indicesToRemove.end(), std::greater<int>());
    for (int idx : indicesToRemove) {
        allFiles->removeAt(idx);
    }

    rebuild();
}

/**
 * @brief Sets the favorite state for a file and updates persistent storage.
 *
 * Uses QSettings to store favorite file names so that the state persists across sessions.
 * @param fileIndex The index of the file.
 * @param isFav The new favorite state.
 */
void FileHierarchyView::fileFavoriteToggled(int fileIndex, bool isFav)
{
    if (!allFiles) return;
    if (fileIndex >= 0 && fileIndex < allFiles->size()) {
        (*allFiles)[fileIndex].isFavorite = isFav;

        // Update QSettings to persist favorite state.
        QSettings settings("YourCompany", "LocalDrive");
        QStringList favorites = settings.value("favorites").toStringList();
        QString fileName = (*allFiles)[fileIndex].fileName;

        if (isFav) {
            if (!favorites.contains(fileName))
                favorites.append(fileName);
        } else {
            favorites.removeAll(fileName);
        }
        settings.setValue("favorites", favorites);
    }
}

/**
 * @brief Updates the selected state for a file and refreshes selection info.
 *
 * Rather than rebuilding the entire view, this updates only the selection info.
 * @param fileIndex The index of the file.
 * @param isSelected The new selection state.
 */
void FileHierarchyView::fileSelectedToggled(int fileIndex, bool isSelected)
{
    if (!allFiles) return;
    if (fileIndex >= 0 && fileIndex < allFiles->size()) {
        (*allFiles)[fileIndex].isSelected = isSelected;
    }
    updateSelectionInfo();
}

/**
 * @brief Updates the toolbar with the current number of selected files.
 *
 * Emits the selectionInfoChanged signal with the count of selected files and
 * whether all or none are selected.
 */
void FileHierarchyView::updateSelectionInfo()
{
    if (!allFiles) {
        emit selectionInfoChanged(0, false, true);
        return;
    }

    QList<int> indices = getCurrentPageFileIndices();
    if (indices.isEmpty()) {
        emit selectionInfoChanged(0, false, true);
        return;
    }

    int countSelected = 0;
    for (int idx : indices) {
        if ((*allFiles)[idx].isSelected) {
            countSelected++;
        }
    }

    bool noneSelected = (countSelected == 0);
    bool allSelected = (countSelected == indices.size());

    emit selectionInfoChanged(countSelected, allSelected, noneSelected);
}
