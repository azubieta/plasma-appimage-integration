#pragma once

// libraries
#include <KJob>
#include <QUrl>

class QDBusPendingCallWatcher;

class OrgAppimageServices1LauncherInterface;

class RemoveJob : public KJob {
public:
    RemoveJob(const QString& target, QObject* parent = nullptr);

    void start() override;

protected slots:

    void callFinishedSlot(QDBusPendingCallWatcher* watcher);

private:
    QString target;
    OrgAppimageServices1LauncherInterface* launcherInterface;
};
