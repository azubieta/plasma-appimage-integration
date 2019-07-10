#ifndef PLASMA5_APPIMAGE_EXTENSIONS_UPDATEAPPLICATIONACTION_H
#define PLASMA5_APPIMAGE_EXTENSIONS_UPDATEAPPLICATIONACTION_H

#include <KFileItemListProperties>
#include <KAbstractFileItemActionPlugin>
#include <QLoggingCategory>

class QAction;

class KFileItemListProperties;

class QWidget;

class OrgAppimageServices1LauncherInterface;
namespace org {
    namespace appimage {
        namespace Services1 {
            typedef ::OrgAppimageServices1LauncherInterface Launcher;
        }
    }
}

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
    org::appimage::Services1::Launcher* launcherInterface;

    void showErrorMessage(const QString& title, const QString& message, QWidget* parentWidget);

    QAction* createAddToMenuAction(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) const;

    QAction* createRemoveFromMenuAction(const KFileItemListProperties& fileItemInfos, QWidget* parentWidget) const;
};

#endif //PLASMA5_APPIMAGE_EXTENSIONS_UPDATEAPPLICATIONACTION_H
