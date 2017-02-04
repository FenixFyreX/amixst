#ifndef APP_H
#define APP_H

#include <QSettings>
#include <QApplication>
#include "widget/sysicon.h"
#include "ui/traydialog.h"

namespace AMixST {

class App : public QApplication
{
    Q_OBJECT
public:
    explicit App(int& argc, char **argv);
    ~App();

    static bool FileExists(const QString& path);
    static QString PathJoin(const QString& a, const QString& b);
    static QString PathSanitize(const QString& path);

    TrayDialog*  dialog;
    SysIcon     sysTray;
    QSettings      conf;
    QIcon      icons[6];

private slots:
    void OnTrayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void LoadResources();
    QString GetConfPath();
};

} // namespace AMixST

#endif // APP_H
