// libraries
#include <KLocalizedString>

// local
#include "RegisterJob.h"
#include "LauncherInterface.h"

RegisterJob::RegisterJob(const QString& target, QObject* parent)
        : KJob(parent), target(target),
          launcherInterface(new OrgAppimageServices1LauncherInterface("org.appimage.Services1.Launcher",
                                                                      "/org/appimage/Services1/Launcher",
                                                                      QDBusConnection::sessionBus(), this)) {}

void RegisterJob::start() {
    description(this, i18n("Creating launcher entry"),
                qMakePair<QString, QString>(i18nc("Target AppImage", "Application"), target));

    auto reply = launcherInterface->registerApp(target);
    if (reply.isError()) {
        setError(-1);
        setErrorText(reply.error().message());
    }

    auto* watcher = new QDBusPendingCallWatcher(reply, this);

    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, this, &RegisterJob::callFinishedSlot);
}

void RegisterJob::callFinishedSlot(QDBusPendingCallWatcher* watcher) {
    if (watcher->isError()) {
        setError(-1);
        setErrorText(watcher->error().message());
    }

    // notify result delayed
    QTimer::singleShot(1000, this, &RegisterJob::emitResult);
}
