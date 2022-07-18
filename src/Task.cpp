#include "Task.h"

#include <qdatetime.h>

Task::Task(QObject *parent) : QObject(parent) { }

Task::Task( QString time, QString path) : Task(QDateTime::currentSecsSinceEpoch(), time, path) { }

Task::Task(int id, QString time, QString path)
{
    this->id = id;
    this->time = time;
    this->path = path;
    this->active = false;
    this->playing = false;
    this->audioName = this->path.split("/").last();

    play_p = new QMediaPlayer;
    QObject::connect(play_p, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(AudioMediaChanged(QMediaContent)));
    QObject::connect(play_p, SIGNAL(positionChanged(qint64)), this, SLOT(AudioPositionChanged(qint64)));
    QObject::connect(play_p, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(AudioStatusChanged(QMediaPlayer::State)));


    InitWidget();
}

int Task::GetHour()
{
    if (this->time.isEmpty()) return -1;
    return this->time.split(":")[0].toInt();
}

int Task::GetMinute()
{
    if (this->time.isEmpty()) return -1;
    return this->time.split(":")[1].toInt();
}

bool Task::IsTimeup()
{
    if (IsEmpty()) return false;
    int hour = QTime::currentTime().hour();
    int minute = QTime::currentTime().minute();
    return (GetHour() == hour && GetMinute() == minute);
}

void Task::PlayAudio()
{
    if (this->path.isEmpty())
    {
        return;
    }

    if (play_p == nullptr || playerWidget_p == nullptr)
    {
        return;
    }

    play_p->setMedia(QUrl::fromLocalFile(this->path));
    play_p->setVolume(100);
    if (play_p->state() == QMediaPlayer::State::StoppedState)
    {
        play_p->play();
    }
}

void Task::AudioMediaChanged(QMediaContent media)
{
    // M_DEBUG << play_p->duration();
}

void Task::AudioPositionChanged(qint64 pos)
{
    if (slider_p == nullptr || pos == 0)
    {
        return;
    }

    double currentPos = (double) pos;
    double totalProgress = (double) play_p->duration();
    slider_p->setValue(( currentPos / totalProgress ) * 100 );
}

void Task::AudioStatusChanged(QMediaPlayer::State status)
{
    if (QMediaPlayer::State::PlayingState == status)
    {
        this->playing = true;


        if (playerWidget_p == nullptr || slider_p == nullptr)
        {
            return;
        }
        playerWidget_p->show();
    }
    else
    {
        this->playing = false;
    }
}

void Task::AudioStop()
{
    if (play_p == nullptr)
    {
        return;
    }

    if (play_p->state() == QMediaPlayer::State::PlayingState)
    {
        play_p->stop();
    }
}

void Task::InitWidget()
{
    QGridLayout *layout = new QGridLayout;

    playerWidget_p = new QWidget();
    playerWidget_p->installEventFilter(this);
    playerWidget_p->setGeometry(0, 0, 400, 30);
    playerWidget_p->setWindowTitle(this->audioName);

    layout->setMargin(8);
    layout->setAlignment(Qt::AlignCenter);
    playerWidget_p->setLayout(layout);

    slider_p = new QSlider(playerWidget_p);
    slider_p->installEventFilter(this);
    slider_p->setStyleSheet("QSlider::handle:horizonta { background: green; }");
    slider_p->setOrientation(Qt::Horizontal);
    slider_p->resize(380, 25);
    slider_p->setRange(0, 100);

    layout->addWidget(slider_p);
}

bool Task::eventFilter(QObject *object, QEvent *event_p)
{
    if (playerWidget_p == object)
    {
        if (QEvent::Close == event_p->type())
        {
            AudioStop();
        }

        return false;
    }
    return QObject::eventFilter(object, event_p);
}


