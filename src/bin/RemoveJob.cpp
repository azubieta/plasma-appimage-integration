// libraries
#include <KLocalizedString>
#include <QDBusPendingCallWatcher>

// local
#include "RemoveJob.h"
#include "LauncherInterface.h"

RemoveJob::RemoveJob(const QString& target, QObject* parent)
    : KJob(parent), target(target),
      launcherInterface(new OrgAppimageServices1LauncherInterface("org.appimage.Services1.Launcher",
                                                                  "/org/appimage/Services1/Launcher",
                                                                  QDBusConnection::sessionBus(), this)) {}

void RemoveJob::start() {
    auto reply = launcherInterface->unregisterApp(target);
    if (reply.isError()) {
        setError(-1);
        setErrorText(i18n("Remove failed: %0").arg(reply.error().message()));
    }

    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(reply, this);

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this, &RemoveJob::callFinishedSlot);
}

void RemoveJob::callFinishedSlot(QDBusPendingCallWatcher* watcher) {
    if (watcher->isError()) {
        setError(-1);
        setErrorText(i18n("Remove failed: %0").arg(watcher->error().message()));
    } else
        description(this, i18n("Application successfully removed"));

    // notify result delayed
    QTimer::singleShot(1000, this, &RemoveJob::emitResult);
}
