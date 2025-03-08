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

#include <QCoreApplication>
#include <string>
#include <vector>

#include "Utils/Utility.h"
#include "DB/DatabaseManager.h"
#include "MemoryManager/MemoryManager.h"


struct row
{
    int moduleID;
    uintptr_t moduleOffset;
    std::vector<uintptr_t> offsets;
};

std::vector<row> fullRows()
{
    DatabaseManager database("7.38lobby.sqlite");
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

void ScanForTimeAddress(MemoryManager& memmng, const std::vector<row>& rows, char* timeInGame)
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

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Logger& logger = Logger::getInstance();


    std::vector<row> rows = fullRows();

    char timeInGame[7];
    MemoryManager memmng(timeInGame);

    if (!rows.empty())
    {
        bool isAddressCorrect = false;
        while (true) {
            if (!isAddressCorrect) {
                ScanForTimeAddress(memmng, rows, timeInGame);
            }
            if (memmng.ReadMemory()) {
                if (Utility::isValidTimeFormat(timeInGame)) {
                    //std::string currentTime = std::string(timeInGame); // todo: check for 1:15

                    std::cout << "Time = " << timeInGame << std::endl;
                    isAddressCorrect = true;
                }
                else
                {
                    isAddressCorrect = false;
                    continue;
                }
            }
            else
            {
                isAddressCorrect = false;
                continue;
            }
            Sleep(1000);
        }
    }
    else
    {
        LOG("Vector is empty!");
        return 0;
    }



    system("pause");

    return 0;
}
