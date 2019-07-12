
// libraries
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

// local
#include "AppImageExtractor.h"
#include "InspectorInterface.h"

using namespace KFileMetaData;

AppImageExtractor::AppImageExtractor(QObject* parent)
    : ExtractorPlugin(parent),
      inspectorInterface(new OrgAppimageServices1InspectorInterface("org.appimage.Services1.Inspector",
                                                                    "/org/appimage/Services1/Inspector",
                                                                    QDBusConnection::sessionBus(), this)) {

    if (!inspectorInterface->isValid())
        qWarning() << "Unable to connect to the AppImage Updater Service";
}

QStringList AppImageExtractor::mimetypes() const {
    return QStringList{
        QStringLiteral("application/x-iso9660-appimage"),
        QStringLiteral("application/vnd.appimage"),
    };
}

void KFileMetaData::AppImageExtractor::extract(ExtractionResult* result) {
    const auto appImageFilePath = result->inputUrl().toUtf8();
    auto reply = inspectorInterface->getApplicationInfo(appImageFilePath);

    reply.waitForFinished();

    if (reply.isError())
        qWarning() << "Unable to fetch AppImage information " << reply.error().message();

    else {
        QString response = reply.value();
        QJsonDocument document = QJsonDocument::fromJson(response.toLocal8Bit());
        QJsonObject root = document.object();

        QJsonValue nameValue = root.value("name");
        result->add(Property::Title, nameValue.toVariant());

        QJsonValue summary = root.value("summary");
        result->add(Property::Description, summary.toString());

        QJsonValue license = root.value("license");
        result->add(Property::License, license.toString());

        QJsonValue links = root.value("links");
        QJsonObject linksOjbect = links.toObject();;
        for (const auto& link: linksOjbect.keys()) {
            QJsonValue linkValue = linksOjbect.value(link);
            result->add(Property::OriginUrl, linkValue.toString());
        }
    }
}
