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

#ifndef AMIXST_MIXERCTRL_H
#define AMIXST_MIXERCTRL_H

#include <QWidget>
#include "mixer.h"

namespace AMixST {

namespace Ui {
class MixerCtrl;
}

class MixerCtrl : public QWidget
{
    Q_OBJECT

public:
    explicit MixerCtrl(QWidget *parent = 0, Mixer* mixer = 0);
    ~MixerCtrl();
    void Refresh();
    bool SetDevice(const QString& dev = "default", const QString& chan = "Master", const int devid = 0);

    Mixer* mix;

private:
    Ui::MixerCtrl *ui;
    bool ownsMixer;

private slots:
    void SliderChanged(int val);
    void ToggleClicked();
    void VolumeChanged(int val);
};


} // namespace AMixST
#endif // AMIXST_MIXERCTRL_H
