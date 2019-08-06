// libraries
#include <KLocalizedString>

// local
#include "RemoveJob.h"
#include "LauncherInterface.h"

RemoveJob::RemoveJob(const QString& target, QObject* parent)
        : KJob(parent), target(target),
          launcherInterface(new OrgAppimageServices1LauncherInterface("org.appimage.Services1.Launcher",
                                                                      "/org/appimage/Services1/Launcher",
                                                                      QDBusConnection::sessionBus(), this)) {}

void RemoveJob::start() {
    description(this, i18n("Removing launcher entry"),
                qMakePair<QString, QString>(i18nc("Target AppImage", "Application"), target));

    auto reply = launcherInterface->unregisterApp(target);
    if (reply.isError()) {
        setError(-1);
        setErrorText(reply.error().message());
    }

    QDBusPendingCallWatcher* watcher = new QDBusPendingCallWatcher(reply, this);

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this, &RemoveJob::callFinishedSlot);
}

void RemoveJob::callFinishedSlot(QDBusPendingCallWatcher* watcher) {
    if (watcher->isError()) {
        setError(-1);
        setErrorText(watcher->error().message());
    } else
        infoMessage(this, i18n("Entry removed"));

    // notify result delayed
    QTimer::singleShot(1000, this, &RemoveJob::emitResult);
}
