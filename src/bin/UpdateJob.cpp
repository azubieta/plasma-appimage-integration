// libraries
#include <KLocalizedString>
#include <KNotification>
#include <QTimer>

// local
#include "UpdateJob.h"
#include "UpdaterInterface.h"
#include "UpdaterTaskInterface.h"

UpdateJob::UpdateJob(const QString& target) : KJob(), target(target) {
    setCapabilities(Killable);
}

void UpdateJob::onBytesTotalChanged(int total) {
    setTotalAmount(Bytes, total);
}

void UpdateJob::onBytesReceivedChanged(int value) {
    setProcessedAmount(Bytes, value);
}

void UpdateJob::onStateChanged(int state) {
    switch (state) {
        case 10:
            infoMessage(this, i18nc("Job heading, like 'Copying'", "Reading update data"));
            break;
        case 20:
            infoMessage(this, i18nc("Job heading, like 'Copying'", "Looking for updates"));
            break;
        case 30:
            infoMessage(this, i18nc("Job heading, like 'Copying'", "Downloading update"));
            break;
            // final states
        case 21:
            setError(-2);
            setErrorText(i18n("No updates available"));
            emitResultDelayed();
            break;
        case 31:
        case 200:
        case -1:
            emitResultDelayed();
            break;

    }
}

bool UpdateJob::doKill() {
    taskInterface->cancel();

    return true;
}

void UpdateJob::start() {
    description(this, i18nc("Job heading, like 'Copying'", "Updating application"),
                qMakePair<QString, QString>(i18nc("The AppImage being updated", "Application"), target));

    if (error() == 0)
        connectUpdaterInterface();

    if (error() == 0)
        createUpdateTask();

    if (error() == 0)
        connectTaskInterface();

    if (error() == 0)
        startUpdateTask();


    if (error() != 0)
        emitResultDelayed();
}

void UpdateJob::connectUpdaterInterface() {
    updaterInterface = new OrgAppimageServices1UpdaterInterface("org.appimage.Services1.Updater",
                                                                "/org/appimage/Services1/Updater",
                                                                QDBusConnection::sessionBus(), this);

    if (!updaterInterface->isValid()) {
        setError(-1);
        setErrorText(i18n("Unable to connect to the AppImage Services"));
    }
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
        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::bytesTotalChanged,
                this, &UpdateJob::onBytesTotalChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::bytesReceivedChanged,
                this, &UpdateJob::onBytesReceivedChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::stateChanged,
                this, &UpdateJob::onStateChanged);

        connect(taskInterface, &OrgAppimageServices1UpdaterTaskInterface::error,
                this, &UpdateJob::onError);
    }
}

void UpdateJob::createUpdateTask() {
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


void UpdateJob::startUpdateTask() {
    auto reply = taskInterface->start();
    reply.waitForFinished();
    if (reply.isError()) {
        setError(DBUS_ERROR);
        setErrorText(i18n("Update failed: %0").arg(reply.error().message()));
    }
}

void UpdateJob::onError(int errorCode) {
    QString errorTitle = i18n("Update failed");

    setError(errorCode);
    setErrorText(errorTitle);
}

void UpdateJob::emitResultDelayed() {
    QTimer::singleShot(400, [this]() {
        emitResult();
    });
}
