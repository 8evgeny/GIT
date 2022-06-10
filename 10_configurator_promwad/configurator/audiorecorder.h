#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioRecorder>

class AudioRecorder : public QObject
{
    Q_OBJECT

    Q_PROPERTY ( bool recording READ recording NOTIFY recordingChanged )

public:
    AudioRecorder ( QObject * parent = nullptr );

    const bool &recording ( ) const;

    public
slots:
    void record ( );
    void stop ( );
public:
signals:
    void recordingChanged ( );

private:
    QAudioRecorder * q_audioRecorder;

    bool b_recording;
};

#endif // AUDIORECORDER_H
