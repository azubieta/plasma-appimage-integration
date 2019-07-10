qt5_add_dbus_interface(appimageservices-interfaces-sources  org.appimage.Services.Launcher.xml LauncherInterface )

add_library(appimageservices-interfaces OBJECT ${appimageservices-interfaces-sources})

target_link_libraries(appimageservices-interfaces PUBLIC Qt5::Core)
target_include_directories(appimageservices-interfaces PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
