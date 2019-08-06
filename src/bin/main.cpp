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
#include "RegisterJob.h"
#include "RemoveJob.h"
#include "InstallJob.h"
#include "UninstallJob.h"
#include "TargetDataLoader.h"


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

    QString target;
    KJob* job = nullptr;
    if (command == "update") {
        target = parseTarget(parser);
        job = new UpdateJob(target);
    }

    if (command == "remove") {
        target = parseTarget(parser);
        job = new RemoveJob(target);
    }

    if (command == "register") {
        target = parseTarget(parser);
        job = new RegisterJob(target);
    }

    if (command == "install") {
        target = parseTarget(parser);
        job = new InstallJob(target);
    }

    if (command == "uninstall") {
        target = parseTarget(parser);
        job = new UninstallJob(target);
    }

    if (!target.isEmpty()) {
        auto targetDataLoader = new TargetDataLoader(target, &app);
        targetDataLoader->loadTargetDataIntoApplication();
    }

    if (job != nullptr) {
        KIO::getJobTracker()->registerJob(job);
        QMetaObject::invokeMethod(job, "start");
    }

    return QApplication::exec();
}


