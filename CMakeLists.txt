cmake_minimum_required(VERSION 3.19)
project(virt_camera LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets)

qt_standard_project_setup()

qt_add_executable(virt_camera
    WIN32 MACOSX_BUNDLE
    main.cpp
    window.h window.cpp
    GLWidget.h GLWidget.cpp
    model.txt
    mesh.h
    math3d.h math3d.cpp
    camera.h camera.cpp
    mesh.frag mesh.vert
    input.h
    renderer.h renderer.cpp
    meshLoader.h
    triangle.h triangle.cpp
    plane.h plane.cpp
    bspnode.h bspnode.cpp
    bsp.h bsp.cpp
)

configure_file(${CMAKE_SOURCE_DIR}/model.txt
               ${CMAKE_BINARY_DIR}/model.txt
               COPYONLY)

 configure_file(${CMAKE_SOURCE_DIR}/mesh.frag
               ${CMAKE_BINARY_DIR}/mesh.frag
               COPYONLY)

 configure_file(${CMAKE_SOURCE_DIR}/mesh.vert
              ${CMAKE_BINARY_DIR}/mesh.vert
              COPYONLY)

target_link_libraries(virt_camera
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Gui
        Qt6::OpenGLWidgets
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
