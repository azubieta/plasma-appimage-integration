add_library(appimage-thumbnailer MODULE AppImageThumbnailCreator.cpp)

target_link_libraries(appimage-thumbnailer PRIVATE KF5::KIOWidgets appimageservices-interfaces)

install(TARGETS appimage-thumbnailer DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES org.appimage.appimage-thumbnailer.desktop DESTINATION ${SERVICES_INSTALL_DIR})
