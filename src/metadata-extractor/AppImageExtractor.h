#pragma once

// libraries
#include <QObject>
#include <KFileMetaData/ExtractorPlugin>

class OrgAppimageServices1InspectorInterface;

namespace KFileMetaData {
    class AppImageExtractor : public ExtractorPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

    public:
        explicit AppImageExtractor(QObject* parent = nullptr);

    public:
        void extract(ExtractionResult* result) override;

        QStringList mimetypes() const override;

    private:
        OrgAppimageServices1InspectorInterface * inspectorInterface;
    };


}
