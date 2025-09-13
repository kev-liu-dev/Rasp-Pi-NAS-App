#ifndef APICLIENT_H
#define APICLIENT_H

#include <QString>
#include <vector>

/**
 * @class APIClient
 * @brief Encapsulates API communications with the backend.
 *
 * Provides methods for uploading, renaming, deleting, listing, and downloading files.
 */
class APIClient {
public:
    /**
     * @brief Constructs the API client with a default server URL.
     * @param serverUrl The base URL of the API server (default: "http://localhost:8080").
     */
    APIClient(const QString &serverUrl = "http://localhost:8080");

    bool uploadFile(const QString &filePath);
    std::vector<QString> listFiles();
    bool renameFile(const QString &oldName, const QString &newName);
    bool deleteFile(const QString &filename);

    /**
     * @brief Downloads a file from the API server.
     * @param filename The name of the file to download.
     * @param destinationPath The full path (including filename) where the file will be saved.
     * @return true if the download succeeded; false otherwise.
     */
    bool downloadFile(const QString &filename, const QString &destinationPath);

private:
    QString m_serverUrl;
};

#endif
