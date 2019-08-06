// libraries
#include <QtWidgets/QApplication>
#include <utility>

// local
#include "InspectorInterface.h"
#include "TargetDataLoader.h"

TargetDataLoader::TargetDataLoader(QString target) : target(std::move(target)) {}

void TargetDataLoader::loadTargetDataIntoApplication() {
    auto inspectorInterface = new OrgAppimageServices1InspectorInterface("org.appimage.Services1.Inspector",
                                                                         "/org/appimage/Services1/Inspector",
                                                                         QDBusConnection::sessionBus(), this);
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
