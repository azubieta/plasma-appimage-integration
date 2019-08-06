#pragma once

// libraries
#include <QtCore/QString>
#include <QtCore/QObject>

class OrgAppimageServices1InspectorInterface;

class TargetDataLoader : public QObject {
Q_OBJECT
public:
    explicit TargetDataLoader(QString target, QObject* parent = nullptr);

    ~TargetDataLoader() override;

    void loadTargetDataIntoApplication();

private:
    QString target;
    QString temporaryAppIconPath;

    void loadApplicationIcon(OrgAppimageServices1InspectorInterface* inspectorInterface);

    void loadApplicationName(OrgAppimageServices1InspectorInterface* inspectorInterface) const;
};