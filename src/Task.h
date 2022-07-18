#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <common.h>
#include <QMediaPlayer>
#include <QWidget>
#include <QEvent>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QSlider>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = nullptr);

    Task(QString, QString);
    Task(int, QString, QString);

    inline int GetID() { return this->id; }
    inline QString GetTime() { return this->time; }
    inline QString GetPath() { return this->path; }
    inline bool IsActive() { return this->active; }
    inline bool IsEmpty() { return (this->time.isEmpty() || this->path.isEmpty()); }
    inline bool IsPlaying(void) { return this->playing; }

    inline void SetActive(bool b) { this->active = b; }

    int GetHour(void);
    int GetMinute(void);
    bool IsTimeup(void);

    void PlayAudio(void);

public slots:
    void AudioMediaChanged(QMediaContent);
    void AudioPositionChanged(qint64);
    void AudioStatusChanged(QMediaPlayer::State);

    void AudioStop(void);


signals:

private:
    int id;
    QString time;
    QString path;
    QString audioName;
    bool active;

    QMediaPlayer *play_p;
    bool playing;

    QWidget *playerWidget_p;
    QSlider *slider_p;

    void InitWidget(void);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

};

#endif // TASK_H
