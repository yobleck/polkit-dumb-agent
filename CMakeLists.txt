cmake_minimum_required(VERSION 3.6)

project(polkit-dumb-agent-style LANGUAGES CXX C)

find_package(Qt5 COMPONENTS Widgets DBus REQUIRED)
find_package(KF5Su REQUIRED)

# systemd for responder dbus
find_package(PkgConfig REQUIRED)
pkg_check_modules(systemd REQUIRED IMPORTED_TARGET libsystemd)

include(GNUInstallDirs)
set(LIBEXEC_PATH "${CMAKE_INSTALL_FULL_LIBDIR}/polkit-dumb-agent-style/")
add_definitions(-DLIBEXEC_DIR=\"${LIBEXEC_PATH}\")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(polkit-dumb-agent-style agent.cpp)

target_link_libraries(polkit-dumb-agent-style PRIVATE
    Qt5::DBus
    Qt5::Widgets
    KF5::Su
    )

add_executable(polkit-dumb-agent-responder-style sendreply.c)
target_link_libraries(polkit-dumb-agent-responder-style PRIVATE PkgConfig::systemd)

install(TARGETS polkit-dumb-agent-style DESTINATION bin)
install(TARGETS polkit-dumb-agent-responder-style
    DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/polkit-dumb-agent-style/"
    )

#install(FILES polkit-dumb-agent-style.desktop DESTINATION /etc/xdg/autostart/)
