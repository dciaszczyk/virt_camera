cmake_minimum_required(VERSION 3.19)
project(virt_camera LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

qt_standard_project_setup()

qt_add_executable(virt_camera
    WIN32 MACOSX_BUNDLE
    main.cpp
    window.cpp
    window.h
    window.ui
    RhiWidget.h RhiWidget.cpp
    camera.h camera.cpp
    mesh.h mesh.cpp
)

target_link_libraries(virt_camera
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Gui
)

include(GNUInstallDirs)

install(TARGETS virt_camera
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET virt_camera
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
