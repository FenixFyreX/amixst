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
