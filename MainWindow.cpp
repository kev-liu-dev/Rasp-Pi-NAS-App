#include "MainWindow.h"
#include "Sidebar.h"
#include "SearchBar.h"
#include "Toolbar.h"
#include "FileHierarchyView.h"
#include "APIClient.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPalette>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDateTime>
#include <QSettings>

/**
 * @author Harshi Kamboj
 * @brief Constructs the MainWindow and sets up the full application UI.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_fileView(nullptr)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout *topBar = new QHBoxLayout();
    topBar->setContentsMargins(10, 10, 10, 10);
    topBar->setSpacing(10);

    SearchBar *searchBar = new SearchBar(this);
    topBar->addWidget(searchBar, 1);

    Toolbar *toolbar = new Toolbar(this);
    topBar->addWidget(toolbar, 0);

    mainLayout->addLayout(topBar, 0);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setContentsMargins(10, 0, 10, 10);
    contentLayout->setSpacing(10);

    Sidebar *sidebar = new Sidebar(this);
    contentLayout->addWidget(sidebar, 0);

    m_fileView = new FileHierarchyView(this);
    m_fileView->setFileData(&allFiles);
    contentLayout->addWidget(m_fileView, 1);

    mainLayout->addLayout(contentLayout, 1);

    connect(sidebar, &Sidebar::categorySelected, m_fileView, &FileHierarchyView::setCategory);
    connect(toolbar, &Toolbar::sortRequested, [this](int criteria){
        SortCriteria sc = static_cast<SortCriteria>(criteria);
        m_fileView->sort(sc);
    });
    connect(toolbar, &Toolbar::renameRequested, m_fileView, &FileHierarchyView::onRenameRequested);
    connect(toolbar, &Toolbar::deleteRequested, m_fileView, &FileHierarchyView::onDeleteRequested);
    connect(toolbar, &Toolbar::selectAllToggled, m_fileView, &FileHierarchyView::onSelectAllToggled);
    connect(searchBar, &QLineEdit::textChanged, m_fileView, &FileHierarchyView::setSearchTerm);
    connect(toolbar, &Toolbar::uploadRequested, this, &MainWindow::onUploadRequested);
    connect(toolbar, &Toolbar::downloadRequested, this, &MainWindow::onDownloadRequested);
    connect(m_fileView, &FileHierarchyView::selectionInfoChanged,
            toolbar, &Toolbar::onSelectionInfoChanged);

    setWindowTitle("Local Drive Client");
    resize(1000, 600);

    loadStoredFiles();
}

/**
 * @brief Shows the main application window.
 */
void MainWindow::showWindow() {
    show();
}

/**
 * @brief Loads user preferences (placeholder).
 */
void MainWindow::loadUserPreferences() {
    qDebug() << "Loading user preferences...";
}

/**
 * @brief Maps a file extension to its corresponding icon filename.
 * @param extension The file extension (e.g., ".pdf").
 * @return A QString representing the icon filename.
 */
QString MainWindow::getIconForExtension(const QString &extension) {
    QString ext = extension.toLower();
    if(ext == ".pdf")
        return "pdf.png";
    else if(ext == ".doc" || ext == ".docx")
        return "word.png";
    else if(ext == ".xls" || ext == ".xlsx")
        return "excel.png";
    else if(ext == ".ppt" || ext == ".pptx")
        return "ppt.png";
    else if(ext == ".mp3" || ext == ".wav" || ext == ".flac" || ext == ".aac" || ext == ".m4a" || ext == ".ogg")
        return "music.png";
    else if(ext == ".mp4" || ext == ".avi" || ext == ".mkv" || ext == ".mov" || ext == ".wmv" || ext == ".flv")
        return "video.png";
    else if(ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".gif" || ext == ".bmp" ||
             ext == ".svg" || ext == ".tiff" || ext == ".webp" || ext == ".heif")
        return "image.png";
    else if(ext == ".txt" || ext == ".html" || ext == ".rtf" || ext == ".csv")
        return "doc.png";
    else
        return "random.png";
}

/**
 * @brief Loads the list of files stored on the API server and updates the UI.
 *
 * Also loads persisted favorite file names from QSettings.
 */
void MainWindow::loadStoredFiles() {
    APIClient apiClient;
    std::vector<QString> storedFiles = apiClient.listFiles();

    // Load the list of favorite file names from QSettings.
    QSettings settings("YourCompany", "LocalDrive");
    QStringList favorites = settings.value("favorites").toStringList();

    for (const auto &filename : storedFiles) {
        if (filename == ".DS_Store")
            continue;

        FileData fileData;
        fileData.fileName = filename;
        int dotIndex = filename.lastIndexOf('.');
        fileData.extension = (dotIndex != -1) ? filename.mid(dotIndex).toLower() : "";
        fileData.dateModified = QDateTime::currentDateTime(); // Placeholder
        fileData.iconName = getIconForExtension(fileData.extension);

        // Mark as favorite if found in settings.
        fileData.isFavorite = favorites.contains(filename);

        allFiles.append(fileData);
    }
    if(m_fileView)
        m_fileView->updateView();
}

/**
 * @brief Handles the Upload button click.
 *
 * Opens a file dialog to select a file, uploads it via the API, and adds its metadata to the list.
 */
void MainWindow::onUploadRequested() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Upload");
    if(filePath.isEmpty())
        return;

    APIClient apiClient;
    bool success = apiClient.uploadFile(filePath);
    if(success) {
        QMessageBox::information(this, "Upload", "File uploaded successfully!");

        QFileInfo fileInfo(filePath);
        FileData newFile;
        newFile.fileName = fileInfo.fileName();
        newFile.extension = "." + fileInfo.suffix().toLower();
        newFile.dateModified = QDateTime::currentDateTime();
        newFile.iconName = getIconForExtension(newFile.extension);

        allFiles.append(newFile);
        if(m_fileView)
            m_fileView->updateView();
    }
    else {
        QMessageBox::warning(this, "Upload", "File upload failed.");
    }
}

/**
 * @brief Handles the Download button click.
 *
 * Ensures exactly one file is selected, prompts the user for a save location,
 * and downloads the file via the API.
 */
void MainWindow::onDownloadRequested() {
    // Ensure exactly one file is selected.
    int countSelected = 0;
    int selectedIndex = -1;
    for (int i = 0; i < allFiles.size(); ++i) {
        if (allFiles[i].isSelected) {
            countSelected++;
            selectedIndex = i;
        }
    }
    if (countSelected != 1) {
        QMessageBox::warning(this, "Download", "Please select exactly one file to download.");
        return;
    }
    QString fileName = allFiles[selectedIndex].fileName;
    // Ask user where to save the file; default name is the fileName.
    QString savePath = QFileDialog::getSaveFileName(this, "Save Downloaded File", fileName);
    if(savePath.isEmpty())
        return;

    APIClient apiClient;
    bool success = apiClient.downloadFile(fileName, savePath);
    if(success)
        QMessageBox::information(this, "Download", "File downloaded successfully.");
    else
        QMessageBox::warning(this, "Download", "Failed to download file.");
}
