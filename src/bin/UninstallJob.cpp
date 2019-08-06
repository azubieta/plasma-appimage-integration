// libraries
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <KI18n/KLocalizedString>
#include <utility>

// local
#include "UninstallJob.h"

UninstallJob::UninstallJob(QString  target, QObject* parent) : KJob(parent), target(std::move(target)) {}

void UninstallJob::start() {
    process.setProgram("pkexec");
    process.setArguments({"appimage-services", "uninstall", target});
    qDebug() << "calling pkexec appimage-services install " << target;

    connect(&process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
    description(this, i18n("Uninstalling application"),
                qMakePair<QString, QString>(i18nc("The AppImage being uninstalled", "Application"), target));

    process.start();
}

void UninstallJob::onProcessFinished(int exitCode) {
    qDebug() << exitCode;
    qDebug() << process.errorString();
    if (exitCode != 0) {
        setError(exitCode);
        setErrorText(process.readAllStandardError());
    } else {
        infoMessage(this, i18n("Application uninstalled"));
    }

    // notify result delayed
    QTimer::singleShot(1000, this, &UninstallJob::emitResult);
}
