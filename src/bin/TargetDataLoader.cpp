// libraries
#include <QIcon>
#include <QtWidgets/QApplication>

// local
#include "InspectorInterface.h"
#include "TargetDataLoader.h"

TargetDataLoader::TargetDataLoader(QString target, QObject* parent) : QObject(parent), target(std::move(target)) {}

void TargetDataLoader::loadTargetDataIntoApplication() {
    auto inspectorInterface = new OrgAppimageServices1InspectorInterface("org.appimage.Services1.Inspector",
                                                                         "/org/appimage/Services1/Inspector",
                                                                         QDBusConnection::sessionBus(), this);
    loadApplicationIcon(inspectorInterface);
    loadApplicationName(inspectorInterface);
}

void TargetDataLoader::loadApplicationName(OrgAppimageServices1InspectorInterface* inspectorInterface) const {
    auto reply = inspectorInterface->getApplicationInfo(target);
    if (reply.isError())
        qWarning() << "Unable to fetch AppImage information " << reply.error().message();

    else {
        QString response = reply.value();
        QJsonDocument document = QJsonDocument::fromJson(response.toLocal8Bit());
        QJsonObject root = document.object();

        QString nameValue = root.value("name").toString();
        if (!nameValue.isEmpty())
            QApplication::setApplicationName(nameValue);
    }
}

void TargetDataLoader::loadApplicationIcon(OrgAppimageServices1InspectorInterface* inspectorInterface) {
    QTemporaryFile temporaryIconFile("XXXXXX");
    if (temporaryIconFile.open()) {
        auto reply = inspectorInterface->extractApplicationIcon(target, temporaryIconFile.fileName());
        if (reply.isError())
            qWarning() << "Unable to fetch AppImage icon " << reply.error().message();

        QString themeIconName = "application-vnd.appimage";
        QImage applicationIcon(temporaryIconFile.fileName());
        if (!applicationIcon.isNull()) {
            // scale the icon to a valid icon size
            applicationIcon = applicationIcon.scaled(512, 512, Qt::KeepAspectRatio);

            // prepare target dir
            QString dirPath = QDir::homePath() + "/.local/share/icons/hicolor/%1x%1/apps/";
            dirPath = dirPath.arg(applicationIcon.height());
            QDir::home().mkpath(dirPath);

            // prepare file name
            QFileInfo temporaryIconFileInfo(temporaryIconFile);
            QString fileName = "plasma-appimage-integration_%2.png";
            fileName = fileName.arg(temporaryIconFileInfo.baseName());

            // save the icon to the right place on the hicolor theme to make it accessible from the kjob dialog
            temporaryAppIconPath = dirPath + fileName;
            if (applicationIcon.save(temporaryAppIconPath, "PNG")) {
                // use the temporary created icon
                themeIconName = "plasma-appimage-integration_" + temporaryIconFileInfo.baseName();
            } else {
                qWarning() << "unable so save icon to " << temporaryAppIconPath;
            }
        }

        // double check that the icon was properly registered in the theme before using it
        if (QIcon::hasThemeIcon(themeIconName)) {
            QApplication::setWindowIcon(QIcon::fromTheme(themeIconName));
        } else {
            qDebug() << "Icon not found in theme " << themeIconName;
            QApplication::setWindowIcon(QIcon::fromTheme("application-vnd.appimage"));
        }
    }
}

TargetDataLoader::~TargetDataLoader() {
    if (QFile::exists(temporaryAppIconPath))
        QFile::remove(temporaryAppIconPath);
}
