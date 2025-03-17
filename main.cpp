// #include "Forms/mainwindow.h"

// #include <QApplication>
// #include <QLocale>
// #include <QTranslator>

// int main(int argc, char *argv[])
// {
//     QApplication a(argc, argv);

//     QTranslator translator;
//     const QStringList uiLanguages = QLocale::system().uiLanguages();
//     for (const QString &locale : uiLanguages) {
//         const QString baseName = "dotaplusfordesperatepeople_" + QLocale(locale).name();
//         if (translator.load(":/i18n/" + baseName)) {
//             a.installTranslator(&translator);
//             break;
//         }
//     }
//     MainWindow w;
//     w.show();
//     return a.exec();
// }

#include <QApplication>
#include <string>
#include <vector>

#include "Utils/Utility.h"
#include "DB/DatabaseManager.h"
#include "MemoryManager/MemoryManager.h"
#include "Updater/updater.h"
#include "TimeNotifier/timenotifier.h"

#include "OverlayManager/overlaymanager.h"
#include "MemoryManager/row.h"
#include "MemoryManager/memoryworker.h"



std::vector<row> fullRows()
{
    DatabaseManager database("database.sqlite");
    std::vector<row> rows;
    if (database.open()) {
        const char* sql = R"(SELECT moduleid, moduleoffset, offset1, offset2, offset3, offset4, offset5, offset6, offset7 FROM results)";
        if (database.prepareQuery(sql)) {
            while (database.step()) {
                int moduleID = database.getIntColumn(0);
                uintptr_t moduleOffset = database.getIntColumn(1);
                std::vector<uintptr_t> offsets;

                for (int i = 7; i > 0; i--) { // Offsets in the database are placed in reverse for unknown reasons.
                    uintptr_t offsetValue = database.getIntColumn(1 + i);
                    offsets.push_back(offsetValue);
                }

                rows.push_back({ moduleID, moduleOffset, std::move(offsets) });
            }
        }
    }
    LOG("Vector filled with " + std::to_string(rows.size()) + " elements");
    return rows;
}



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    Updater updater;
    updater.checkForUpdates();

    Logger& logger = Logger::getInstance();
    TimeNotifier* timemng = new TimeNotifier();

    std::vector<row> rows = fullRows();
    char timeInGame[7];
    MemoryManager memmng(timeInGame);

    if (!rows.empty())
    {
        MemoryWorker* worker = new MemoryWorker(rows, &memmng, timeInGame, timemng);
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        QObject::connect(thread, &QThread::started, worker, &MemoryWorker::process);
        QObject::connect(worker, &QObject::destroyed, thread, &QThread::quit);
        QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        QObject::connect(&app, &QApplication::aboutToQuit, worker, [worker]() {
            worker->_isRunning = false;
        });

        thread->start();
        qDebug() << "Thread started";
    }
    else {
        LOG("Vector is empty!");
        return 0;
    }

    return app.exec();
}

