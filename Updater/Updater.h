#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QUrl>
#include <QDebug>

#include "../Logger/Logger.h"

class QNetworkAccessManager;

class Updater : public QObject {
    Q_OBJECT
public:
    explicit Updater(QObject *parent = nullptr);
    void checkForUpdates();

private slots:
    void onVersionJsonDownloaded();
    void onDatabaseDownloaded();

private:
    void downloadDatabase(const QString& databaseName);
    bool isUpdateNeeded(const QJsonObject& remoteVersion);
    QJsonObject loadLocalVersion();
    void updateLocalVersion(const QJsonObject& newVersion);

private:
    QNetworkAccessManager* _manager;

    const QString _remoteVersionUrl = "https://raw.githubusercontent.com/OlexandrNikolaiev/dotaplusfordesperatepeople/main/version.json";
    const QString _remoteRepoBaseUrl = "https://raw.githubusercontent.com/OlexandrNikolaiev/dotaplusfordesperatepeople/main";
    const QString _localVersionFile = "version.json";

    QJsonObject _remoteVersion;
};

#endif // UPDATER_H
