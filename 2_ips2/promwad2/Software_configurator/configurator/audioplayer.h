#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QMediaPlayer>
#include <QDir>
#include <QUrl>


class AudioPlayer
{
public:
    AudioPlayer();
    void togglePlayback();
    void playUrl(const QUrl &url);
    void openFile();
    QStringList supportedMimeTypes();
private:
    QMediaPlayer mediaPlayer;
};

#endif // AUDIOPLAYER_H
