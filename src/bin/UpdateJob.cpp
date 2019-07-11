// libraries
#include <QFile>
#include <QDebug>
#include <QDBusPendingReply>
#include <KLocalizedString>

// local
#include "UpdateJob.h"
#include "UpdaterInterface.h"
#include "UpdaterTaskInterface.h"

UpdateJob::UpdateJob(const QString& target) : KJob(), target(target) {
    setCapabilities(Killable);
}

void UpdateJob::onProgressTotalChanged(int value, int total) {
    setTotalAmount(Bytes, total);
}

void UpdateJob::onProgressValueChanged(int value) {
    setProcessedAmount(Bytes, value);
}

void UpdateJob::onStateChanged(int state) {
}

void UpdateJob::onStatusMessageChanged(const QString& message) {
    description(this, message);
}

bool UpdateJob::doKill() {
    taskInterface->cancel();

    return true;
}

void UpdateJob::start() {
    validateTarget();

    if (error() == 0)
        connectUpdaterInterface();

    if (error() == 0)
        startUpdateTasks();

    if (error() == 0)
        connectTaskInterface();

    if (error() != 0)
        emitResult();
}

void UpdateJob::connectTaskInterface() {
    taskInterface = new OrgAppimageServices1UpdaterTaskInterface(
        "org.appimage.Services1.Updater.Task", "/org/appimage/Services1/Updater/Tasks/" + taskId,
        QDBusConnection::sessionBus(), this);

    if (!taskInterface->isValid()) {
        setError(DBUS_ERROR);
        setErrorText(i18n("Unable to connect to AppImage Services Updater Task interface: %0")
                         .arg(taskInterface->lastError().message())
        );

    } else {
        description(this, i18n("Looking for updates"));

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::progressTotalChanged,
                this, &UpdateJob::onProgressTotalChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::progressValueChanged,
                this, &UpdateJob::onProgressValueChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::stateChanged,
                this, &UpdateJob::onStateChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::statusMessageChanged,
                this, &UpdateJob::onStatusMessageChanged);
    }
}

void UpdateJob::startUpdateTasks() {
    auto reply = updaterInterface->update(target);
    reply.waitForFinished();

    if (reply.isError()) {
        setError(DBUS_ERROR);
        setErrorText(i18n("Update failed: %0").arg(reply.error().message()));
    } else {
        taskId = reply.value();

        // notify failed operation
        if (taskId.isEmpty()) {
            setError(DBUS_ERROR);
            setErrorText(i18n("Update failed: %0").arg(target));
        }
    }
}

void UpdateJob::validateTarget() {
    QFile targetFile(target);
    if (!targetFile.exists(target)) {
        setError(BAD_TARGET_ERROR);
        setErrorText(i18n("Target file doesn't exists: %0").arg(target));
        return;
    }
}

void UpdateJob::connectUpdaterInterface() {
    updaterInterface = new OrgAppimageServices1UpdaterInterface("org.appimage.Services1.Updater",
                                                                "/org/appimage/Services1/Updater",
                                                                QDBusConnection::sessionBus(), this);

    if (!updaterInterface->isValid()) {
        setError(-1);
        setErrorText(i18n("Unable to connect to the AppImage Services deamon"));
    }
}

