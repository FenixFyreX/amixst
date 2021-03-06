/* amixst - AlsaMixer Sound Tool
 * Copyright (c) 2017 Chaz Domerese
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
