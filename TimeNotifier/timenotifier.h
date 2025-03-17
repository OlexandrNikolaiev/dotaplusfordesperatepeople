#ifndef TIMENOTIFIER_H
#define TIMENOTIFIER_H

#include <QString>
#include <QDebug>
#include <unordered_map>
#include <QObject>

#include "../OverlayManager/overlaymanager.h"
#include "../SoundManager/soundmanager.h"
#include "../SoundManager/soundPaths.h"

struct TimeEvent {
    QString soundFile;
    QString textOverlay;
};

class TimeNotifier : public QObject
{
    Q_OBJECT
public:
    TimeNotifier();
    ~TimeNotifier();

    // Сделаем метод слотом, чтобы можно было использовать queued connection.
public slots:
    void handleTimeEvent(char timeInGame[7]);

private:
    void playSound(const QString& file);
    void showTextOverlay(const QString& text);

    OverlayManager* overlay;
};

#endif // TIMENOTIFIER_H
