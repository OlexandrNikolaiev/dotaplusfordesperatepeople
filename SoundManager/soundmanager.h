#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QSoundEffect>
#include <QTemporaryFile>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <memory>

class SoundManager {
public:
    static void playSound(const QString& filePath) {
        static std::unique_ptr<QSoundEffect> effect = std::make_unique<QSoundEffect>();

        QFile resourceFile(filePath);
        if (!resourceFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Не удалось открыть ресурс";
            return;
        }

        QTemporaryFile tempFile(QDir::tempPath() + "/XXXXXX.wav");
        if (!tempFile.open()) {
            qWarning() << "Не удалось создать временный файл";
            return;
        }

        QByteArray soundData = resourceFile.readAll();
        tempFile.write(soundData);
        tempFile.flush();

        tempFile.setAutoRemove(false);
        QString tempFilePath = tempFile.fileName();
        qDebug() << "Временный файл создан:" << tempFilePath;

        effect->setSource(QUrl::fromLocalFile(tempFilePath));
        //effect->setLoopCount(QSoundEffect::Infinite);
        effect->setVolume(0.25f);
        effect->play();
    }
};

#endif // SOUNDMANAGER_H
