// libraries
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <KI18n/KLocalizedString>
#include <utility>

// local
#include "InstallJob.h"

InstallJob::InstallJob(QString  target, QObject* parent) : KJob(parent), target(std::move(target)) {}

void InstallJob::start() {
    process.setProgram("pkexec");
    process.setArguments({"appimage-services", "install", target});
    qDebug() << "calling pkexec appimage-services install " << target;

    connect(&process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
    description(this, i18n("Installing application"),
                qMakePair<QString, QString>(i18nc("The AppImage being installed", "Application"), target));

    process.start();
}

void InstallJob::onProcessFinished(int exitCode) {
    qDebug() << exitCode;
    qDebug() << process.errorString();
    if (exitCode != 0) {
        setError(exitCode);
        setErrorText(process.readAllStandardError());
    }

    // notify result delayed
    QTimer::singleShot(1000, this, &InstallJob::emitResult);
}
