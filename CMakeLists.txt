cmake_minimum_required(VERSION 3.19)
project(dotaplusfordesperatepeople LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)

qt_standard_project_setup()

qt_add_executable(dotaplusfordesperatepeople
    WIN32 MACOSX_BUNDLE
    main.cpp
    DB/DatabaseManager.cpp
    DB/DatabaseManager.h
    Logger/Logger.cpp
    Logger/Logger.h
    MemoryManager/MemoryManager.cpp
    MemoryManager/MemoryManager.h
    ProcessManager/ProcessManager.cpp
    ProcessManager/ProcessManager.h
    Utils/Utility.cpp
    Utils/Utility.h
)

qt_add_translations(
    TARGETS dotaplusfordesperatepeople
    TS_FILES dotaplusfordesperatepeople_en_150.ts
)

target_link_libraries(dotaplusfordesperatepeople
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Sql
)

include(GNUInstallDirs)

install(TARGETS dotaplusfordesperatepeople
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET dotaplusfordesperatepeople
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
