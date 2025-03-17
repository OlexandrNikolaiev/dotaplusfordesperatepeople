#ifndef MEMORYWORKER_H
#define MEMORYWORKER_H

#include <QObject>
#include <QThread>

#include "MemoryManager.h"
#include "row.h"
#include "../TimeNotifier/timenotifier.h"

class MemoryWorker : public QObject {
    Q_OBJECT
public:
    MemoryWorker(const std::vector<row>& rows, MemoryManager* memmng, char* timeInGame, TimeNotifier* timemng);
    std::atomic<bool> _isRunning;

public slots:
    void process();

private:
    void ScanForTimeAddress(MemoryManager& memmng, const std::vector<row>& rows, char* timeInGame);
    std::vector<row> _rows;
    MemoryManager* _memmng;
    char* _timeInGame;
    TimeNotifier* _timemng;

};


#endif // MEMORYWORKER_H
