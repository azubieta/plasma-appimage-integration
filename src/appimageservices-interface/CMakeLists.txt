set_source_files_properties(
    org.appimage.Services.Launcher.xml
    org.appimage.Services.Updater.xml
    org.appimage.Services.Updater.Task.xml
    org.appimage.Services.Inspector.xml
    PROPERTIES NO_NAMESPACE ON
)

qt5_add_dbus_interface(appimageservices-interfaces-sources org.appimage.Services.Launcher.xml LauncherInterface)
qt5_add_dbus_interface(appimageservices-interfaces-sources org.appimage.Services.Updater.xml UpdaterInterface)
qt5_add_dbus_interface(appimageservices-interfaces-sources org.appimage.Services.Updater.Task.xml UpdaterTaskInterface)
qt5_add_dbus_interface(appimageservices-interfaces-sources org.appimage.Services.Inspector.xml InspectorInterface)

add_library(appimageservices-interfaces STATIC ${appimageservices-interfaces-sources})

target_link_libraries(appimageservices-interfaces PUBLIC Qt5::Core)
target_include_directories(appimageservices-interfaces PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
