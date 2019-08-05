add_executable(
    plasma-appimage-integration
    main.cpp
    UpdateJob.cpp
    RemoveJob.cpp
    InstallJob.cpp
)

target_link_libraries(plasma-appimage-integration appimageservices-interfaces KF5::KIOWidgets KF5::I18n KF5::Notifications)

install(TARGETS plasma-appimage-integration DESTINATION bin)
