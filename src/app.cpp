#include "amixst.h"
#include <QFileInfo>
#include <QDir>
#include <QStyle>
#include <QStyleFactory>

namespace AMixST {

static QString icoNames[6] = {
    "tray", "mute", "vol1",
    "vol2", "vol3", "vol4"
};

App::App(int& argc, char **argv)
    : QApplication(argc, argv)
    , dialog(0)
    , sysTray()
    , conf(GetConfPath(), QSettings::IniFormat)
{
    LoadResources();
    sysTray.setIcon(icons[0]);

    dialog = new TrayDialog();

    connect(&sysTray, &SysIcon::onMove, this, []() { if(app->dialog) app->dialog->Relocate(); });
    connect(&sysTray, &QSystemTrayIcon::activated, this, &App::OnTrayActivated);

    sysTray.show();
}

App::~App()
{
    delete dialog;
}

void App::OnTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason != QSystemTrayIcon::ActivationReason::Trigger)
        return;
    if(dialog->isHidden())//&& mixerDialog->isHidden())
        dialog->show();
    else
    {
        dialog->hide();
        //mixerDialog->hide();
    }
}

void App::LoadResources()
{
    QString icoPath;

    for(int i = 0; i < 6; i++)
    {
        icoPath = conf.value("icon/" + icoNames[i], "").toString();
        if(icoPath.isEmpty())
            icoPath = ":/icons/" + icoNames[i] + ".png";
        else
            icoPath = PathSanitize(icoPath);
        icons[i] = QIcon(icoPath);
    }

    if(conf.contains("app/style"))
        setStyle(conf.value("app/style", "qt").toString());
}

QString App::GetConfPath()
{
    QString cfg = PathJoin(QDir::homePath(), ".config/amixst/conf");
    if(FileExists(cfg))
        return cfg;

    return "";
}

QString App::PathSanitize(const QString &path)
{
    if(path[0] == '~')
        return QDir::cleanPath(PathJoin(QDir::homePath(), path.mid(1)));
    return path;
}

bool App::FileExists(const QString& path)
{
    QFileInfo info(PathSanitize(path));
    if(info.exists() && info.isFile())
    {
        return true;
    }

    return false;
}

QString App::PathJoin(const QString &a, const QString &b)
{
    QString c;
    c.append(a);

    if(!a.endsWith('/') && !b.startsWith('/'))
        c.append('/');

    c.append((a.endsWith('/') && b.startsWith('/')) ? b.mid(1) : b);
    return c;
}

} // namespace AMixST
