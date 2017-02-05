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

#include "mixerctrl.h"
#include "ui_mixerctrl.h"
#include "amixst.h"
#include <QSlider>
#include <QPushButton>

namespace AMixST {

MixerCtrl::MixerCtrl(QWidget *parent, Mixer* mixer)
    : QWidget(parent)
    , mix(0)
    , ui(new Ui::MixerCtrl)
{
    ui->setupUi(this);

    if(mixer == 0)
    {
        ownsMixer = true;
        mix = new Mixer;
    }
    else
    {
        ownsMixer = false;
        mix = mixer;
    }

    connect(ui->volume, &QSlider::sliderMoved, this, &MixerCtrl::SliderChanged);
    connect(ui->toggle, &QAbstractButton::clicked, this, &MixerCtrl::ToggleClicked);
    connect(mix, &Mixer::onVolumeChanged, this, &MixerCtrl::VolumeChanged);
    connect(mix, &Mixer::onMuteChanged, this, &MixerCtrl::Refresh);

    Refresh();
}

MixerCtrl::~MixerCtrl()
{
    if(ownsMixer && (mix != 0))
        delete mix;
    delete ui;
}

void MixerCtrl::Refresh()
{
    if(mix->Valid())
    {
        ui->volume->setEnabled(true);
        ui->toggle->setEnabled(true);

        ui->volume->setMinimum(mix->MinVolume());
        ui->volume->setMaximum(mix->MaxVolume());

        if(ui->volume->value() != mix->Volume())
        {
            ui->volume->setValue(mix->Volume());
            long vol = mix->Volume() - mix->MinVolume();
            int p = (int)((100 * vol) / (mix->MaxVolume() - mix->MinVolume()));
            ui->volume->setToolTip(QString::number(p) + "%");
        }

        if(mix->Muted())
            ui->toggle->setIcon(app->icons[1]);
        else
        {
            long q = (mix->MaxVolume() - mix->MinVolume()) / 4;
            int i = (int)((mix->Volume() - mix->MinVolume()) / q);
            int num = qBound<int>(2, i + 2, 5);
            ui->toggle->setIcon(app->icons[num]);
        }
        ui->toggle->setToolTip(mix->Muted() ? "Click to unmute." : "Click to mute.");
    }
    else
    {
        ui->volume->setMinimum(0);
        ui->volume->setMaximum(0);
        ui->volume->setValue(0);
        ui->volume->setToolTip("n/a");

        ui->toggle->setIcon(app->icons[1]);
        ui->toggle->setToolTip("");

        ui->volume->setEnabled(false);
        ui->toggle->setEnabled(false);
    }
}

void MixerCtrl::SliderChanged(int val)
{
    if(!mix->Valid())
        return;
    if(ui->volume->value() != mix->Volume())
        mix->SetVolume(val);
}

void MixerCtrl::ToggleClicked()
{
    if(!mix->Valid())
        return;
    mix->ToggleMute();
}

void MixerCtrl::VolumeChanged(int val)
{
    Q_UNUSED(val);
    Refresh();
}

} // namespace AMixST
