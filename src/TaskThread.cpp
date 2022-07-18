#include "TaskThread.h"

TaskThread::TaskThread(QObject *parent) : QThread(parent)
{
    running = false;
}

void TaskThread::LoopTask()
{
    int startTime = QDateTime::currentSecsSinceEpoch();
    while (running)
    {
        if (this->tasks.isEmpty()) continue;
        if (QDateTime::currentSecsSinceEpoch() < (startTime + 1)) continue;

        for (Task *task_p : this->tasks)
        {
            if (task_p == nullptr || task_p->IsEmpty()) continue;

            if (task_p->IsTimeup())
            {
                if (!task_p->IsPlaying() && !task_p->IsActive())
                {
                    M_DEBUG << "Timeup: " << task_p->GetTime();
                    task_p->PlayAudio();
                    task_p->SetActive(true);
                }
            }
        }

        startTime = QDateTime::currentSecsSinceEpoch();
        M_DEBUG << startTime;
    }
}

void TaskThread::exit()
{
    running = false;
    QThread::exit(0);
}

void TaskThread::run()
{
    running = true;
    LoopTask();
}
