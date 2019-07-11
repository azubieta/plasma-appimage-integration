#pragma once

// libraries
#include <KJob>
#include <QUrl>

class OrgAppimageServices1UpdaterInterface;

class OrgAppimageServices1UpdaterTaskInterface;

class UpdateJob : public KJob {
Q_OBJECT
public:
    enum ErrorCodes {
        BAD_TARGET_ERROR = 1,
        DBUS_ERROR,
    };

    explicit UpdateJob(const QString& target);

    void start() override;

protected slots:

    void onProgressTotalChanged(int value, int total);

    void onProgressValueChanged(int value);

    void onStateChanged(int state);

    void onStatusMessageChanged(const QString& message);

    bool doKill() override;


private:
    QString target;
    QString taskId;
    OrgAppimageServices1UpdaterInterface* updaterInterface;
    OrgAppimageServices1UpdaterTaskInterface* taskInterface;

    void validateTarget();

    void connectUpdaterInterface();

    void startUpdateTasks();

    void connectTaskInterface();
};
