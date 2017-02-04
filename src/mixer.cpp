
#include "mixer.h"
#include "alsa/asoundlib.h"
#include <QMutex>

namespace AMixST {

class AlsaHandle
{
public:
    snd_mixer_t*                mix;
    snd_mixer_elem_t*           elt;
    snd_mixer_selem_id_t*       sid;
    snd_mixer_selem_channel_id_t ch;
    long minvol, maxvol;
    int cid;

    QString  devname;
    QString channame;
    int     devindex;
    bool     capture;

    QMutex lock;

    AlsaHandle(const QString& dev = "default", const QString& chan = "Master", const int devindex = 0, bool capture = false)
        : mix(0), elt(0), sid(0)
        , ch(SND_MIXER_SCHN_UNKNOWN)
        , minvol(0), maxvol(1)
        , devname(dev), channame(chan)
        , devindex(devindex), capture(capture)
    {
        Obtain();
    }

    ~AlsaHandle() { Release(); }

    bool Valid();
    void Obtain();
    void Release(bool needsLock = true);
};

bool AlsaHandle::Valid()
{
    return mix != 0;
}

void AlsaHandle::Obtain()
{
    if(mix != 0)
        Release();

    lock.lock();

    snd_mixer_selem_id_malloc(&sid);
    if(!sid)
    {
        lock.unlock();
        return;
    }

    snd_mixer_selem_id_set_index(sid, devindex);
    snd_mixer_selem_id_set_name(sid, channame.toUtf8().data());

    if(snd_mixer_open(&mix, 0) < 0)
        goto err;
    if(snd_mixer_attach(mix, devname.toUtf8().data()) < 0)
        goto err;
    if(snd_mixer_selem_register(mix, 0, 0) < 0)
        goto err;
    if(snd_mixer_load(mix) < 0)
        goto err;

    elt = snd_mixer_find_selem(mix, sid);
    if(!elt)
        goto err;

    if(capture)
        snd_mixer_selem_get_capture_volume_range(elt, &minvol, &maxvol);
    else
        snd_mixer_selem_get_playback_volume_range(elt, &minvol, &maxvol);

    int i;
    for(i = 0; i < SND_MIXER_SCHN_LAST; i++)
    {
        if(capture && snd_mixer_selem_has_capture_channel(elt, (snd_mixer_selem_channel_id_t)i))
        {
            cid = i;
            break;
        }
        else if(!capture && snd_mixer_selem_has_playback_channel(elt, (snd_mixer_selem_channel_id_t)i))
        {
            cid = i;
            break;
        }
    }

    if(i >= SND_MIXER_SCHN_LAST)
        goto err;

    lock.unlock();
    return;
err:
    Release(false);
    lock.unlock();
}

void AlsaHandle::Release(bool needsLock)
{
    if(needsLock)
        lock.lock();
    if(mix != 0)
        snd_mixer_close(mix);
    if(sid != 0)
        snd_mixer_selem_id_free(sid);

    mix = 0;
    elt = 0;
    sid = 0;
    minvol = 0;
    maxvol = 0;
    cid = 0;

    if(needsLock)
        lock.unlock();
}

Mixer::Mixer(QObject *parent, QString device, QString channel, int dev_index, bool capture)
    : QObject(parent)
    , volume(0)
    , muted(false)
    , valid(false)
    , p(0)
{
    p = new AlsaHandle(device, channel, dev_index, capture);
    Tick();

    connect(&timer, &QTimer::timeout, this, &Mixer::Tick);
    timer.start(200);
}

Mixer::~Mixer()
{
    if(p) delete p;
}

void Mixer::Tick()
{
    if(!p->Valid())
    {
        p->Release();
        p->Obtain();
        if(!p->Valid())
        {
            valid = false;
            return;
        }
    }

    int mute;
    valid = true;
    p->lock.lock();
    long oldVol = volume;
    bool oldMute = muted;
    {
        snd_mixer_selem_channel_id_t ch = (snd_mixer_selem_channel_id_t)p->cid;
        if((p->capture && snd_mixer_selem_get_capture_volume(p->elt, ch, &volume) < 0)
        || (!p->capture && snd_mixer_selem_get_playback_volume(p->elt, ch, &volume) < 0))
        {
            volume = p->minvol;
            valid = false;
            goto end;
        }

        muted = false;
        if(p->capture && snd_mixer_selem_has_capture_switch(p->elt))
        {
            if(snd_mixer_selem_get_capture_switch(p->elt, ch, &mute) >= 0)
            {
                muted = (mute == 0);
            }
        }
        else if(!p->capture && snd_mixer_selem_has_playback_switch(p->elt))
        {
            if(snd_mixer_selem_get_playback_switch(p->elt, ch, &mute) >= 0)
            {
                muted = (mute == 0);
            }
        }
    }
end:
    p->lock.unlock();

    if(valid && (volume != oldVol))
        emit onVolumeChanged(volume);
    if(valid && (muted != oldMute))
        emit onMuteChanged();
}

bool Mixer::Valid() const
{
    return valid;
}

bool Mixer::Muted() const
{
    return muted;
}

long Mixer::Volume() const
{
    return volume;
}

long Mixer::MinVolume() const
{
    return p->minvol;
}

long Mixer::MaxVolume() const
{
    return p->maxvol;
}

bool Mixer::SetVolume(long vol, int ch)
{
    bool ret = true;
    if(!valid)
        return false;

    if(ch == CHAN_CURRENT)
        ch = p->cid;

    if(vol > p->maxvol) vol = p->maxvol;
    if(vol < p->minvol) vol = p->minvol;

    p->lock.lock();
    if(p->capture && (ch == CHAN_ALL))
    {
        if(snd_mixer_selem_set_capture_volume_all(p->elt, vol) < 0)
            ret = false;
        else
        {
            volume = vol;
            emit onVolumeChanged(volume);
        }
    }
    else if(p->capture)
    {
        if(snd_mixer_selem_set_capture_volume(p->elt, (snd_mixer_selem_channel_id_t)ch, vol) < 0)
            ret = false;
        else
        {
            volume = vol;
            emit onVolumeChanged(volume);
        }
    }
    else if(!p->capture && (ch == CHAN_ALL))
    {
        if(snd_mixer_selem_set_playback_volume_all(p->elt, vol) < 0)
            ret = false;
        else
        {
            volume = vol;
            emit onVolumeChanged(volume);
        }
    }
    else if(!p->capture)
    {
        if(snd_mixer_selem_set_playback_volume(p->elt, (snd_mixer_selem_channel_id_t)ch, vol) < 0)
            ret = false;
        else
        {
            volume = vol;
            emit onVolumeChanged(volume);
        }
    }

    p->lock.unlock();
    return ret;
}

bool Mixer::ToggleMute(int ch)
{
    bool ret = true;
    int set = muted ? 1 : 0;
    if(!valid)
        return false;

    if(ch == CHAN_CURRENT)
        ch = p->cid;

    p->lock.lock();
    if(p->capture && snd_mixer_selem_has_capture_switch(p->elt))
    {
        if(ch != CHAN_ALL)
        {
            if(snd_mixer_selem_set_capture_switch(p->elt, (snd_mixer_selem_channel_id_t)ch, set) < 0)
                ret = false;
        }
        else if(snd_mixer_selem_set_capture_switch_all(p->elt, set) < 0)
            ret = false;
    }
    else if(!p->capture && snd_mixer_selem_has_playback_switch(p->elt))
    {
        if(ch != CHAN_ALL)
        {
            if(snd_mixer_selem_set_playback_switch(p->elt, (snd_mixer_selem_channel_id_t)ch, set) < 0)
                ret = false;
        }
        else if(snd_mixer_selem_set_playback_switch_all(p->elt, set) < 0)
            ret = false;
    }
    p->lock.unlock();
    return ret;
}

} // namespace AMixST
