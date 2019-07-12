add_library(kfilemetadata-appimageextractor MODULE AppImageExtractor.cpp)
target_link_libraries(kfilemetadata-appimageextractor
    KF5::FileMetaData
    KF5::ConfigCore
    Qt5::Xml
    Qt5::Gui
    Qt5::DBus
    appimageservices-interfaces
    )

set_target_properties(kfilemetadata-appimageextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
install(TARGETS kfilemetadata-appimageextractor DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)
