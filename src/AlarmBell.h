#ifndef ALARMBELL_H
#define ALARMBELL_H

#define TASK_CONFIG_FILE "tFile"
#include <common.h>

#include <QWidget>
#include <Task.h>

#include <TaskThread.h>

#include <QGridLayout>

#include <QTableView>
#include <QStandardItemModel>

#include <QPushButton>
#include <QTimeEdit>
#include <QTextEdit>
#include <QFileDialog>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QLabel>
#include <RemoveButton.h>


QT_BEGIN_NAMESPACE
namespace Ui { class AlarmBell; }
QT_END_NAMESPACE

class AlarmBell : public QWidget
{
    Q_OBJECT

public:
    AlarmBell(QWidget *parent = nullptr);
    ~AlarmBell();

public slots:
    void AddBellTask(bool);
    void PickFile();
    void RemoveButtonClicked(RemoveButton*);

private:
    Ui::AlarmBell *ui;

    // QThread *tThread_p;

    QFile tFile;
    QString tFilePath;
    QJsonDocument jDoc;

    TaskThread *taskThread_p;
    QList<Task*> tasks;

    QGridLayout *mainLayout_p;

    QTableView *taskList_p;
    QStandardItemModel *model_p;

    QFileDialog fileDialog;
    QPushButton *filePicker_p;
    QPushButton *addButton_p;
    QTimeEdit *hourEdit_p;
    QTimeEdit *minuteEdit_p;
    QTextEdit *textEdit_p;

    void InitView();
    void InitTaskList();
    void InitBottomView();

    void UpdateTaskList();
    void UpdateThread();

    void InitTasks();
    void ParseTaskJson(QString);

    void WriteDataTotFile(Task*);
};
#endif // ALARMBELL_H
