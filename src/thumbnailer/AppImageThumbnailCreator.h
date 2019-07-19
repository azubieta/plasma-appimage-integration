#pragma once

// libraries
#include <QObject>
#include <QMimeDatabase>
#include <QLoggingCategory>
#include <kio/thumbcreator.h>

Q_DECLARE_LOGGING_CATEGORY(LOG_APPIMAGE_THUMBS)

class OrgAppimageServices1InspectorInterface;

class AppImageThumbnailCreator : public QObject, public ThumbCreator {
Q_OBJECT
    QMimeDatabase db;
public:
    explicit AppImageThumbnailCreator(QObject* parent = 0);

    ~AppImageThumbnailCreator() override;

    bool create(const QString& path, int w, int h, QImage& thumb) override;

    Flags flags() const override;

    bool isAnAcceptedMimeType(const QString& path) const;

private:
    OrgAppimageServices1InspectorInterface *inspectorInterface;

};
