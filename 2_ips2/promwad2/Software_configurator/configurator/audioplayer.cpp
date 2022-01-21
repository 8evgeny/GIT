#include "audioplayer.h"
#include "audioplayer.h"


AudioPlayer::AudioPlayer()
{

}


QStringList AudioPlayer::supportedMimeTypes()
{
    QStringList result = QMediaPlayer::supportedMimeTypes();
    if (result.isEmpty())
        result.append(QStringLiteral("audio/mpeg"));
    return result;
}

void AudioPlayer::openFile()
{
//    QFileDialog fileDialog(this);
//    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
//    fileDialog.setWindowTitle(tr("Open File"));
//    fileDialog.setMimeTypeFilters(MusicPlayer::supportedMimeTypes());
//    fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).value(0, QDir::homePath()));
//    if (fileDialog.exec() == QDialog::Accepted)
//        playUrl(fileDialog.selectedUrls().constFirst());
}

void AudioPlayer::playUrl(const QUrl &url)
{
//    if (url.isLocalFile()) {
//        const QString filePath = url.toLocalFile();
//        setWindowFilePath(filePath);
//        infoLabel->setText(QDir::toNativeSeparators(filePath));
//        fileName = QFileInfo(filePath).fileName();
//    } else {
//        setWindowFilePath(QString());
//        infoLabel->setText(url.toString());
//        fileName.clear();
//    }
// mediaPlayer.setMedia(url);
    mediaPlayer.setMedia(QUrl::fromLocalFile("/home/user/work/prj.promwad.Fez-Telecom-Engineering/Software_configurator/build-configurator-Desktop_Qt_5_12_3_GCC_64bit-Debug/test.mp3"));
    mediaPlayer.play();
}

void AudioPlayer::togglePlayback()
{
    if (mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia)
        openFile();
    else if (mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}

