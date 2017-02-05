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

#ifndef MIXER_H
#define MIXER_H

#include <QObject>
#include <QTimer>

namespace AMixST {

class AlsaHandle;

#define CHAN_ALL     -2
#define CHAN_CURRENT -1

// F = front, B = back, L = left, R = right, S = side, C = center
#define CHAN_FL   0
#define CHAN_FR   1
#define CHAN_BL   2
#define CHAN_BR   3
#define CHAN_FC   4
#define CHAN_WOOF 5
#define CHAN_SL   6
#define CHAN_SR   7
#define CHAN_BC   8
#define CHAN_MONO CHAN_FL

class Mixer : public QObject
{
    Q_OBJECT
public:
    explicit Mixer(QObject *parent = 0, QString device = "default", QString channel = "Master", int dev_index = 0, bool capture = false);
    ~Mixer();
    void ReOpen(const QString& device, const QString& channel, const int dev_index = 0);
    bool Valid() const;
    bool Muted() const;
    long Volume() const;
    long MinVolume() const;
    long MaxVolume() const;

    bool SetVolume(long vol, int ch = CHAN_ALL);
    bool ToggleMute(int ch = CHAN_ALL);

signals:
    void onVolumeChanged(long vol);
    void onMuteChanged();

private:
    long   volume;
    bool    muted;
    bool    valid;
    QTimer  timer;
    AlsaHandle* p;

private slots:
    void Tick();
};

} // namespace AMixST

#endif // MIXER_H
