#include "APIClient.h"
#include <QFile>
#include <QFileInfo>
#include <QByteArray>
#include <QUrl>
#include <string>
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"

/**
 * @brief Constructs the API client with the given server URL.
 */
APIClient::APIClient(const QString &serverUrl)
    : m_serverUrl(serverUrl)
{
}

/**
 * @brief Uploads a file to the API server.
 */
bool APIClient::uploadFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    QByteArray fileData = file.readAll();
    file.close();

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    std::string stdFileData = fileData.toStdString();
    std::string stdFileName = fileName.toStdString();

    httplib::Client cli(m_serverUrl.toStdString().c_str());
    httplib::MultipartFormDataItems items = {
        { "file", stdFileData, stdFileName, "application/octet-stream" }
    };

    auto res = cli.Post("/api/upload", items);
    return (res && res->status == 200);
}

/**
 * @brief Retrieves the list of files stored on the API server.
 */
std::vector<QString> APIClient::listFiles() {
    std::vector<QString> result;
    httplib::Client cli(m_serverUrl.toStdString().c_str());
    auto res = cli.Get("/api/files");
    if (res && res->status == 200) {
        try {
            auto jsonData = nlohmann::json::parse(res->body);
            for (const auto &item : jsonData) {
                std::string fileStr = item.get<std::string>();
                result.push_back(QString::fromStdString(fileStr));
            }
        } catch (...) {
            // Parsing failed.
        }
    }
    return result;
}

/**
 * @brief Renames a file on the API server.
 */
bool APIClient::renameFile(const QString &oldName, const QString &newName) {
    httplib::Client cli(m_serverUrl.toStdString().c_str());
    // URL-encode the file names to handle spaces and special characters.
    QString encodedOld = QUrl::toPercentEncoding(oldName);
    QString encodedNew = QUrl::toPercentEncoding(newName);
    std::string body = "old=" + encodedOld.toStdString() + "&new=" + encodedNew.toStdString();

    auto res = cli.Post("/api/rename", body, "application/x-www-form-urlencoded");
    return (res && res->status == 200);
}

/**
 * @brief Deletes a file from the API server.
 */
bool APIClient::deleteFile(const QString &filename) {
    httplib::Client cli(m_serverUrl.toStdString().c_str());
    std::string body = "file=" + filename.toStdString();
    auto res = cli.Post("/api/delete", body, "application/x-www-form-urlencoded");
    return (res && res->status == 200);
}

/**
 * @brief Downloads a file from the API server.
 */
bool APIClient::downloadFile(const QString &filename, const QString &destinationPath) {
    httplib::Client cli(m_serverUrl.toStdString().c_str());
    // URL-encode the filename to safely include it in the URL.
    QString encodedFilename = QUrl::toPercentEncoding(filename);
    std::string endpoint = "/api/download/" + encodedFilename.toStdString();

    auto res = cli.Get(endpoint.c_str());
    if (res && res->status == 200) {
        QFile file(destinationPath);
        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }
        file.write(QByteArray::fromStdString(res->body));
        file.close();
        return true;
    }
    return false;
}
