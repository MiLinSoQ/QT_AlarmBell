#ifndef TASKTHREAD_H
#define TASKTHREAD_H

#include "Task.h"
#include <QThread>
#include <common.h>

#include <QDateTime>

class TaskThread : public QThread
{
    Q_OBJECT
public:
    explicit TaskThread(QObject *parent = nullptr);

    QString name;
    inline void SetTasks(QList<Task*> tasks) { this->tasks = tasks; }

public slots:
    void exit(void);

signals:

private:
    bool running;
    QList<Task*> tasks;

    void LoopTask(void);

protected:

    void run(void) override;

};

#endif // TASKTHREAD_H
