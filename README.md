# Plasma AppImage Integration

Enhance the AppImage user experience on the plasma desktop environment. It's build on top of
AppImageServices and `libappimage` being compatible with other solutions like AppImageLauncher.
 

Features:
- Creation/Removal of launcher entries for application files
- System wide (all users) application installation
- Updates lookup and installation
- Application metada shown in the details view
- Launch assistant to skip the give execution permission step before running an AppImage

## Usage

After installing `plasma-appimage-integration` from sources you will also need to install AppImageServices. 
Instructions are provided in the **Build** section. If you installed it from the binaries provided, AppImageServices 
will be already installed.

As AppImageServices depends on DBus and it runs on user space you must ensure having such feature available on 
your system. Debian, Ubuntu and derivatives user will have to install `dbus-user-session` and restart the system
for it to be enable.

To use this software just open Dolphin (the kde file browser) and you will get AppImage files thumbnails, 
enhanced metadata and file item actions to easily mange your applications.  

## Build
**Dependencies**
- cmake (>=3.12)
- gcc (>=4.8)
- Qt5::Core
- Qt5::Widgets
- Qt5::Network
- Qt5::DBus
- KF5::XmlGui 
- KF5::I18n 
- KF5::KIO 
- KF5::TextWidgets
- KF5::Notifications
- KF5::FileMetaData

```bash
mkdir build; cd build
cmake -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` ..
make -j`nproc`
sudo make install

# Install AppImage Services
wget https://www.opencode.net/azubieta/AppImageService/-/jobs/artifacts/master/raw/appimage-services-x86_64.AppImage?job=build:AppImage -O appimage-services.AppImage
chmod +x appimage-services.AppImage
./appimage-services.AppImage self-install
```
