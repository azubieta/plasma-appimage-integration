// libraries
#include <QIcon>
#include <QAction>
#include <KPluginFactory>
#include <KPluginLoader>
#include <KLocalizedString>
#include <KNotification>


// local
#include "AppImageFileItemActions.h"
#include "LauncherInterface.h"
#include "UpdaterInterface.h"

// KDE plugin setup
K_PLUGIN_FACTORY(AppImageFileItemActionsFactory, registerPlugin<AppImageFileItemActions>();)

Q_LOGGING_CATEGORY(APPIMAGE_FILEITEMACTIONS, "appimage.fileitemactions")

AppImageFileItemActions::AppImageFileItemActions(QObject* parent, const QVariantList&)
    : KAbstractFileItemActionPlugin(parent),
      launcherInterface(new OrgAppimageServices1LauncherInterface("org.appimage.Services1.Launcher",
                                                                  "/org/appimage/Services1/Launcher",
                                                                  QDBusConnection::sessionBus(), this)),

      updaterInterface(new OrgAppimageServices1UpdaterInterface("org.appimage.Services1.Updater",
                                                                "/org/appimage/Services1/Updater",
                                                                QDBusConnection::sessionBus(), this)) {


}

QList<QAction*> AppImageFileItemActions::actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) {
    QList<QAction*> actions;

    if (launcherInterface->isValid()) {
        const QList<QUrl> urlList = fileItemInfos.urlList();
        if (urlList.size() == 1) {
            if (launcherInterface->isRegistered(urlList.first().toString()))
                actions += createRemoveFromMenuAction(fileItemInfos, parentWidget);
            else
                actions += createAddToMenuAction(fileItemInfos, parentWidget);
        } else {
            actions += createAddToMenuAction(fileItemInfos, parentWidget);
            actions += createRemoveFromMenuAction(fileItemInfos, parentWidget);
        }
    } else {
        qWarning() << "Discarding AppImage Add/Remove to Menu actions. Unable to connect to the AppImage Launcher Service";
    }

    if (launcherInterface->isValid()) {
        QAction* updateAction = new QAction(QIcon::fromTheme("update-none"), "Update", parentWidget);
        updateAction->setProperty("urls", QVariant::fromValue(fileItemInfos.urlList()));
        updateAction->setProperty("parentWidget", QVariant::fromValue(parentWidget));
        connect(updateAction, &QAction::triggered, this, &AppImageFileItemActions::update);
        actions += updateAction;
    } else {
        qWarning() << "Discarding AppImage Update action. Unable to connect to the AppImage Launcher Updater";
    }

    actions += createInstallAction(fileItemInfos, parentWidget);

    return actions;
}

QAction* AppImageFileItemActions::createRemoveFromMenuAction(const KFileItemListProperties& fileItemInfos,
                                                             QWidget* parentWidget) const {

    QAction* removeFromMenuAction = new QAction(QIcon::fromTheme("list-remove"), "Remove from Launcher",
                                                parentWidget);
    removeFromMenuAction->setProperty("urls", QVariant::fromValue(fileItemInfos.urlList()));
    removeFromMenuAction->setProperty("parentWidget", QVariant::fromValue(parentWidget));
    connect(removeFromMenuAction, &QAction::triggered, this, &AppImageFileItemActions::removeFromMenu);
    return removeFromMenuAction;
}

QAction* AppImageFileItemActions::createAddToMenuAction(const KFileItemListProperties& fileItemInfos,
                                                        QWidget* parentWidget) const {
    QAction* addToMenuAction = new QAction(QIcon::fromTheme("list-add"), "Add to Launcher", parentWidget);
    addToMenuAction->setProperty("urls", QVariant::fromValue(fileItemInfos.urlList()));
    addToMenuAction->setProperty("parentWidget", QVariant::fromValue(parentWidget));
    connect(addToMenuAction, &QAction::triggered, this, &AppImageFileItemActions::addToMenu);
    return addToMenuAction;
}

QAction *AppImageFileItemActions::createInstallAction(const KFileItemListProperties &fileItemInfos,
                                                      QWidget *parentWidget) const {
    QAction* installAction = new QAction(QIcon::fromTheme("install"), "Install", parentWidget);
    installAction->setProperty("urls", QVariant::fromValue(fileItemInfos.urlList()));
    installAction->setProperty("parentWidget", QVariant::fromValue(parentWidget));
    connect(installAction, &QAction::triggered, this, &AppImageFileItemActions::install);
    return installAction;
}

void AppImageFileItemActions::addToMenu() {
    const QList<QUrl> urls = sender()->property("urls").value<QList<QUrl>>();
    QWidget* parentWidget = sender()->property("parentWidget").value<QWidget*>();

    QList<QDBusPendingReply<bool>> replies;
    for (const QUrl& url : urls)
        replies += launcherInterface->registerApp(url.toString());

    QString errorTitle = i18n("Add to launcher failed");
    for (QDBusPendingReply<bool>& reply: replies) {
        reply.waitForFinished();

        if (reply.isError())
            showErrorMessage(errorTitle, reply.error().message(), parentWidget);
        else {
            // notify failed operation
            if (!reply.value()) {
                QString url = urls.at(replies.indexOf(reply)).toString();
                showErrorMessage(errorTitle, i18n("\"%0\"\nthe file seems broken").arg(url), parentWidget);
            }
        }
    }
}

void AppImageFileItemActions::removeFromMenu() {
    const QList<QUrl> urls = sender()->property("urls").value<QList<QUrl>>();
    for (const QUrl& url : urls)
        launcherInterface->unregisterApp(url.toString());
}

void AppImageFileItemActions::update() {
    const QList<QUrl> urls = sender()->property("urls").value<QList<QUrl>>();
    QString program = "plasma-appimage-integration";

    for (const QUrl& url : urls) {
        QStringList arguments;
        arguments << "update" << url.toLocalFile();

        QProcess::startDetached(program, arguments);
    }
}

void AppImageFileItemActions::install() {
    const QList<QUrl> urls = sender()->property("urls").value<QList<QUrl>>();
    QString program = "plasma-appimage-integration";

    for (const QUrl& url : urls) {
        QStringList arguments;
        arguments << "install" << url.toLocalFile();

        QProcess::startDetached(program, arguments);
    }
}

void AppImageFileItemActions::showErrorMessage(const QString& title, const QString& message, QWidget* parentWidget) {
    KNotification* notify = new KNotification(QStringLiteral("notification"), parentWidget,
                                              KNotification::CloseOnTimeout | KNotification::DefaultEvent);
    notify->setTitle(title);
    notify->setText(message);
    notify->setIconName("dialog-warning");
    notify->sendEvent();
}

#include "AppImageFileItemActions.moc"
