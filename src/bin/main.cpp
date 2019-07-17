// libraries
#include <QDebug>
#include <QApplication>
#include <QCommandLineParser>
#include <KNotification>
#include <KLocalizedString>

#include <KIO/JobTracker>
#include <KJobTrackerInterface>

// local
#include "UpdateJob.h"
#include "RemoveJob.h"


QString parseTarget(QCommandLineParser& parser) {
    const QStringList& positionalArguments = parser.positionalArguments();

    if (positionalArguments.size() >= 2) {
        QUrl url(positionalArguments.at(1));

        // Remove 'file://' prefix
        if (url.isLocalFile())
            return url.toLocalFile();
        else
            return url.toString();
    } else {
        qWarning() << "Missing target AppImage";
        parser.showHelp(1);
    }
}

void executeUpdateCommand(const QString& target) {
    KJob* job = new UpdateJob(target);

    KIO::getJobTracker()->registerJob(job);
    job->start();

    if (job->error() != 0)
        UpdateJob::notifyError(i18n("Update failed").arg(target), job->errorString());
}

void executeRemoveCommand(const QString& target) {
    KJob* job = new RemoveJob(target);

    KIO::getJobTracker()->registerJob(job);
    job->start();

    if (job->error() != 0)
        UpdateJob::notifyError(i18n("Remove failed").arg(target), job->errorString());

}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setApplicationName("plasma-appimage-integration");
    QApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("AppImage Integration utility for Plasma Desktop");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("command", QCoreApplication::translate("command", "Command to execute."));
    parser.addPositionalArgument("target", QCoreApplication::translate("target", "Target AppImage."));


    parser.process(app);
    if (parser.positionalArguments().empty())
        parser.showHelp(1);

    const QStringList& positionalArguments = parser.positionalArguments();

    const QString& command = positionalArguments.at(0);

    if (command == "update") {
        QString target = parseTarget(parser);
        executeUpdateCommand(target);
    }

    if (command == "remove") {
        QString target = parseTarget(parser);
        executeRemoveCommand(target);
    }

    return QApplication::exec();
}


