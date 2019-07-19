// libraries
#include <QString>
#include <QImage>
#include <QStandardPaths>
#include <QMimeType>
#include <QLoggingCategory>
#include <QUrl>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QCryptographicHash>

// local
#include "AppImageThumbnailCreator.h"
#include "InspectorInterface.h"

Q_LOGGING_CATEGORY(LOG_APPIMAGE_THUMBS, "AppImageThumbnailer")

extern "C"
{
Q_DECL_EXPORT ThumbCreator* new_creator() {
    return new AppImageThumbnailCreator();
}
}

AppImageThumbnailCreator::AppImageThumbnailCreator(QObject* parent) : QObject(parent),
                                                                      inspectorInterface(
                                                                          new OrgAppimageServices1InspectorInterface(
                                                                              "org.appimage.Services1.Inspector",
                                                                              "/org/appimage/Services1/Inspector",
                                                                              QDBusConnection::sessionBus(), this)) {
}

AppImageThumbnailCreator::~AppImageThumbnailCreator() = default;


ThumbCreator::Flags AppImageThumbnailCreator::flags() const {
    return ThumbCreator::None;
}

bool AppImageThumbnailCreator::create(const QString& path, int w, int h, QImage& thumb) {
    if (inspectorInterface->isValid()) {
        QTemporaryFile temporaryFile;
        if (temporaryFile.open()) {
            auto reply = inspectorInterface->extractFile(path, ".DirIcon", temporaryFile.fileName());
            reply.waitForFinished();

            if (reply.isError()) {
                return false;
            } else {
                bool res = thumb.load(temporaryFile.fileName());
                thumb = thumb.scaled(w, h);
                return res;
            }

        } else {
            return false;
        }

    } else {
        qCDebug(LOG_APPIMAGE_THUMBS) << "Unable to connect to appimage services dbus interface";
        return false;
    }
}

bool AppImageThumbnailCreator::isAnAcceptedMimeType(const QString& path) const {
    QMimeType type = db.mimeTypeForFile(path);
    return type.isValid() &&
           (type.inherits("application/x-iso9660-appimage") || type.inherits("application/vnd.appimage"));
}

