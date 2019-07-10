add_definitions(-DTRANSLATION_DOMAIN="appimage-fileitemactions")

add_library(appimage-fileitemactions MODULE AppImageFileItemActions.cpp)

target_link_libraries(appimage-fileitemactions appimageservices-interfaces KF5::KIOWidgets KF5::I18n KF5::Notifications)
target_include_directories(appimage-fileitemactions PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS appimage-fileitemactions DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES org.appimage.fileitemactions.desktop DESTINATION ${SERVICES_INSTALL_DIR})
