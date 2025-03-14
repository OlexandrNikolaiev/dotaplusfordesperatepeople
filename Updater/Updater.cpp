#include "updater.h"

Updater::Updater(QObject *parent)
    : QObject(parent)
{
    _manager = new QNetworkAccessManager(this);
}

void Updater::checkForUpdates() {
    QNetworkRequest request((QUrl(_remoteVersionUrl)));
    QNetworkReply *reply = _manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Updater::onVersionJsonDownloaded);
}

void Updater::onVersionJsonDownloaded() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        return;

    if (reply->error() != QNetworkReply::NoError) {
        LOG(QString("Couldn't download version.json: ") + reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isObject()) {
        LOG(QString("Invalid json received"));
        reply->deleteLater();
        return;
    }

    _remoteVersion = jsonDoc.object();
    qDebug()<<_remoteVersion;
    QString remoteDatabaseName = _remoteVersion.value("database_name").toString();

    if (isUpdateNeeded(_remoteVersion)) { // todo ask for update
        LOG(QString("Update is available. Database " + remoteDatabaseName + " will be downloaded"));
        downloadDatabase(remoteDatabaseName);
    } else {
        LOG(QString("Database is up to date"));
    }
    reply->deleteLater();
}

void Updater::downloadDatabase(const QString &databaseName) {
    QString urlStr = _remoteRepoBaseUrl + "/" + databaseName;
    QUrl url(urlStr);
    QNetworkRequest request(url);
    QNetworkReply *reply = _manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Updater::onDatabaseDownloaded);
}

void Updater::onDatabaseDownloaded() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply)
        return;

    if (reply->error() != QNetworkReply::NoError) {
        LOG(QString("Database download error:" + reply->errorString()));
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonObject localVersion = loadLocalVersion();
    QString localDatabaseName = localVersion.value("database_name").toString();

    // If the old database file exists, delete it
    if (!localDatabaseName.isEmpty()) {
        QFile oldDbFile(localDatabaseName);
        if (oldDbFile.exists()) {
            if (!oldDbFile.remove()) {
                LOG(QString("Failed to delete the old database " + localDatabaseName));
                return;
            }
        }
    }

    // Save the new database with the name specified in the remote version.json
    QString newDatabaseName = _remoteVersion.value("database_name").toString();
    QFile newDbFile(newDatabaseName);
    if (!newDbFile.open(QIODevice::WriteOnly)) {
        LOG(QString("Failed to open the file for writing the new database:" + newDatabaseName));
        return;
    }
    newDbFile.write(data);
    newDbFile.close();
    LOG(QString("Database successfully updated:" + newDatabaseName));

    updateLocalVersion(_remoteVersion);
}

bool Updater::isUpdateNeeded(const QJsonObject &remoteVersion) {
    QJsonObject localVersion = loadLocalVersion();
    // If the local version.json file does not exist, an update is required
    if (localVersion.isEmpty())
        return true;

    QString localPatch = localVersion.value("dota2_patch").toString();
    qDebug() << localPatch;
    QString remotePatch = remoteVersion.value("dota2_patch").toString();
    qDebug() << remotePatch;
    return (localPatch != remotePatch);
}

QJsonObject Updater::loadLocalVersion() {
    QFile file(_localVersionFile);
    if (!file.exists()) {
        LOG(QString("Local version.json file not found."));
        return QJsonObject();
    }
    if (!file.open(QIODevice::ReadOnly)) {
        LOG(QString("Failed to open the local version.json file."));
        return QJsonObject();
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isObject()) {
        LOG(QString("Invalid JSON in the local version.json file."));
        return QJsonObject();
    }
    //qDebug() << "local = " << jsonDoc.object();
    return jsonDoc.object();
}

void Updater::updateLocalVersion(const QJsonObject &newVersion) {
    QFile file(_localVersionFile);
    if (!file.open(QIODevice::WriteOnly)) {
        LOG(QString("Failed to open the local version.json file for updating."));
        return;
    }
    QJsonDocument jsonDoc(newVersion);
    file.write(jsonDoc.toJson());
    file.close();
    LOG(QString("version.json file successfully updated."));
}

