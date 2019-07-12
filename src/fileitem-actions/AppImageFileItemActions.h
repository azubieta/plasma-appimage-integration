#pragma once

// libraries
#include <KFileItemListProperties>
#include <KAbstractFileItemActionPlugin>
#include <QLoggingCategory>
#include <QMap>

class QWidget;

class QAction;

class KFileItemListProperties;

class OrgAppimageServices1LauncherInterface;

class OrgAppimageServices1UpdaterInterface;

Q_DECLARE_LOGGING_CATEGORY(APPIMAGE_FILEITEMACTIONS)

class AppImageFileItemActions : public KAbstractFileItemActionPlugin {
Q_OBJECT
public:
    AppImageFileItemActions(QObject* parent, const QVariantList& args);

    QList<QAction*> actions(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) override;

private Q_SLOTS:

    void addToMenu();

    void removeFromMenu();

    void update();

private:
    OrgAppimageServices1LauncherInterface* launcherInterface;
    OrgAppimageServices1UpdaterInterface* updaterInterface;

    void showErrorMessage(const QString& title, const QString& message, QWidget* parentWidget);

    QAction* createAddToMenuAction(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) const;

    QAction* createRemoveFromMenuAction(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) const;
};
