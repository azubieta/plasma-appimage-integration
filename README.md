# Plasma AppImage Integration

Provide a set of extensions for the Plasma desktop Environment to enhance the AppImage related user experience.

Features:
- "Add to Launcher",  "Remove from Launcher" and "Update" File Item Actions
- Update progress get notified using a KJob
- File Metadata Extractor. Includes the application name, links, license and description.
- Launch assistant to skip the give execution permission step before running an AppImage

**Notice that this software is under active development and it's not suitable for production yet.**


## Build
**Dependencies**
- appimage-services
- cmake
- gcc (>=4.8)
- KF5::XmlGui 
- KF5::I18n 
- KF5::KIO 
- KF5::TextWidgets
- KF5::Notifications
- KF5::FileMetaData
- Qt5::Core
- Qt5::Widgets
- Qt5::Network
- Qt5::DBus

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
