// libraries
#include <QIcon>
#include <QAction>
#include <QDBusPendingReply>
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

    if (!launcherInterface->isValid())
        qWarning() << "Unable to connect to the AppImage Launcher Service";

    if (!updaterInterface->isValid())
        qWarning() << "Unable to connect to the AppImage Updater Service";
}

QList<QAction*> AppImageFileItemActions::actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) {
    QList<QAction*> actions;


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

    QAction* updateAction = new QAction(QIcon::fromTheme("update-none"), "Update", parentWidget);
    updateAction->setProperty("urls", QVariant::fromValue(fileItemInfos.urlList()));
    updateAction->setProperty("parentWidget", QVariant::fromValue(parentWidget));
    connect(updateAction, &QAction::triggered, this, &AppImageFileItemActions::update);
    actions += updateAction;

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

void AppImageFileItemActions::showErrorMessage(const QString& title, const QString& message, QWidget* parentWidget) {
    KNotification* notify = new KNotification(QStringLiteral("notification"), parentWidget,
                                              KNotification::CloseOnTimeout | KNotification::DefaultEvent);
    notify->setTitle(title);
    notify->setText(message);
    notify->setIconName("dialog-warning");
    notify->sendEvent();
}

#include "AppImageFileItemActions.moc"
