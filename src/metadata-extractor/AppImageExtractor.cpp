
// libraries
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <KFileMetaData/Properties>
#include <KF5/kconfig_version.h>

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

        QString nameValue = root.value("name").toString();
        if (!nameValue.isEmpty())
            result->add(Property::Title, nameValue);

//         Property::Description and Property::License are not supported until KF5 5.53.0
#if (QT_VERSION_CHECK(KCONFIG_VERSION_MAJOR, KCONFIG_VERSION_MINOR, KCONFIG_VERSION_PATCH) >= QT_VERSION_CHECK(5, 53, 0))
        QString summary = root.value("summary").toString();
        if (!summary.isEmpty())
            result->add(Property::Description, summary);

        QString license = root.value("license").toString();
        if (!license.isEmpty())
            result->add(Property::License, license);
#endif

        QJsonValue links = root.value("links");
        QJsonObject linksOjbect = links.toObject();;
        for (const auto& link: linksOjbect.keys()) {
            QJsonValue linkValue = linksOjbect.value(link);
            result->add(Property::OriginUrl, linkValue.toString());
        }
    }
}
