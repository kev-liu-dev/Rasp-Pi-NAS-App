#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QList>
#include <QString>
#include <QWidget>


/**
 * @author Harshi Kamboj
 * @enum SortCriteria
 * @brief Defines possible sorting methods for file display.
 */
enum class SortCriteria {
    NameAsc,     /// Sort by name ascending
    NameDesc,    ///< Sort by name descending
    DateAsc,     ///< Sort by date modified ascending
    DateDesc,    ///< Sort by date modified descending
    TypeAsc,     ///< Sort by file type ascending
    TypeDesc     ///< Sort by file type descending
};

struct FileData {
    QString iconName;         ///< Icon used for display (e.g., "pdf.png")
    QString fileName;         ///< File name shown to user
    QString extension;        ///< File extension/type
    bool isFavorite = false;  ///< Whether marked as favorite
    bool isSelected = false;  ///< Whether currently selected
    QDateTime dateModified;   ///< Last modified date
};

class FileHierarchyView;

/**
 * @class MainWindow
 * @brief The main interface window for the Local Drive Client.
 *
 * Manages layout and connections between sidebar, toolbar, search bar, and file view components.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the MainWindow and sets up the full application layout.
     * @param parent Optional parent QWidget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Displays the main application window.
     */
    void showWindow();

    /**
     * @brief Placeholder function for loading user preferences (e.g., theme, sort options).
     */
    void loadUserPreferences();

private slots:
    void onUploadRequested();
    void onDownloadRequested();  // New slot for downloading

private:
    QList<FileData> allFiles;
    FileHierarchyView *m_fileView;

    QString getIconForExtension(const QString &extension);
    void loadStoredFiles();
};

#endif // MAINWINDOW_H
