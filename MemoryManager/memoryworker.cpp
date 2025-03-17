#include "memoryworker.h"

MemoryWorker::MemoryWorker(const std::vector<row> &rows, MemoryManager *memmng, char *timeInGame, TimeNotifier *timemng)
 : _rows(rows), _memmng(memmng), _timeInGame(timeInGame), _timemng(timemng) {}

void MemoryWorker::process()
{
    bool isAddressCorrect = false;
    while (true) {
        if (!isAddressCorrect) {
            ScanForTimeAddress(*_memmng, _rows, _timeInGame);
        }
        if (_memmng->ReadMemory()) {
            if (Utility::isValidTimeFormat(_timeInGame)) {
                _timemng->handleTimeEvent(_timeInGame);
                isAddressCorrect = true;
            } else {
                isAddressCorrect = false;
                continue;
            }
        } else {
            isAddressCorrect = false;
            continue;
        }
        QThread::sleep(1);
    }
}

void MemoryWorker::ScanForTimeAddress(MemoryManager &memmng, const std::vector<row> &rows, char *timeInGame)
{
    LOG("Scanning for valid time address");
    bool isAddressFound = false;
    while (!isAddressFound) {
        for (const auto& r : rows) {
            if (memmng.ReadPointerChain(r.moduleID, r.moduleOffset, r.offsets)) {
                if (memmng.ReadMemory()) {
                    if (Utility::isValidTimeFormat(timeInGame)) {
                        isAddressFound = true;
                        break;
                    }
                }
            }
        }
    }
}
