#include "AlarmBell.h"
#include "ui_AlarmBell.h"

AlarmBell::AlarmBell(QWidget *parent) : QWidget(parent) , ui(new Ui::AlarmBell)
{
    taskThread_p = nullptr;

    ui->setupUi(this);

    InitTasks();

    InitView();

    UpdateThread();
}

AlarmBell::~AlarmBell()
{
    delete ui;

    tasks.clear();

    if (taskThread_p != nullptr)
    {
        taskThread_p->quit();
        taskThread_p->deleteLater();
    }
}

void AlarmBell::InitTasks()
{
    tasks.clear();

    tFilePath = QString("%1/%2").arg( qApp->applicationDirPath()).arg(TASK_CONFIG_FILE);
    tFile.setFileName(tFilePath);
    QString tContent = "";
    if (tFile.open(QIODevice::ReadWrite) | QIODevice::Text)
    {
        tContent = tFile.readAll();
        tFile.close();
    }
    else
    {
        M_DEBUG << "Open file feil.";
    }

    if (tContent.isEmpty() == false)
    {
        ParseTaskJson(tContent);
    }
}

void AlarmBell::ParseTaskJson(QString content)
{
    QJsonParseError jError;
    jDoc = QJsonDocument::fromJson(content.toUtf8(), &jError);

    if (jError.error != QJsonParseError::NoError)
    {
        M_DEBUG << "Json format error, code: " << jError.error;
        return;
    }

    QJsonObject jData = jDoc.object();
    if (jData.isEmpty())
    {
        return;
    }

    QJsonArray jArray = jData.value("data").toArray();
    QJsonObject jObj;
    for (int i = 0; i < jArray.count(); i ++)
    {
        jObj = jArray[i].toObject();
        if (jObj.isEmpty())
        {
            continue;
        }

        int id = jObj.value("id").toInt();
        QString time = jObj.value("time").toString();
        QString path = jObj.value("path").toString();
        if (time.isEmpty() || path.isEmpty()) continue;

        tasks.append(new Task(id, time, path));
    }
}

void AlarmBell::InitView()
{
    this->mainLayout_p = new QGridLayout(this);
    this->mainLayout_p->setMargin(4);

    InitTaskList();

    InitBottomView();

    setLayout(this->mainLayout_p);
}

void AlarmBell::InitTaskList()
{
    this->taskList_p = new QTableView(this);


    model_p = new QStandardItemModel(this->taskList_p);
    this->taskList_p->setModel(model_p);

    model_p->setColumnCount(3);
    model_p->setHeaderData(0, Qt::Orientation::Horizontal, "Time");
    model_p->setHeaderData(1, Qt::Orientation::Horizontal, "Path");
    model_p->setHeaderData(2, Qt::Orientation::Horizontal, "");
    this->mainLayout_p->addWidget(this->taskList_p, 0, 0, 1, 5);

    UpdateTaskList();
}

void AlarmBell::InitBottomView()
{
    //
    QLabel* hourLable_p = new QLabel(this);
    hourLable_p->setText("Hour");
    hourLable_p->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    this->mainLayout_p->addWidget(hourLable_p, 2, 0);

    QLabel* minuteLabel_p = new QLabel(this);
    minuteLabel_p->setText("Minute");
    minuteLabel_p->setAlignment(Qt::AlignRight |  Qt::AlignCenter);
    this->mainLayout_p->addWidget(minuteLabel_p, 2, 2);

    this->hourEdit_p = new QTimeEdit(this);
    this->hourEdit_p->setDisplayFormat("hh");
    this->hourEdit_p->setTime(QTime::currentTime());
    this->mainLayout_p->addWidget(this->hourEdit_p, 2, 1);


    this->minuteEdit_p = new QTimeEdit(this);
    this->minuteEdit_p->setDisplayFormat("mm");
    this->minuteEdit_p->setTime(QTime::currentTime());
    this->mainLayout_p->addWidget(this->minuteEdit_p, 2, 3);


    this->addButton_p = new QPushButton(this);
    this->addButton_p->setText("Add");
    QObject::connect(this->addButton_p, SIGNAL(clicked(bool)), this, SLOT(AddBellTask(bool)));
    this->mainLayout_p->addWidget(this->addButton_p, 2, 4);


    this->filePicker_p = new QPushButton(this);
    this->filePicker_p->setText("Pick");
    QObject::connect(this->filePicker_p, SIGNAL(clicked(bool)), this, SLOT(PickFile()));
    this->mainLayout_p->addWidget(this->filePicker_p, 1, 4);

    this->textEdit_p = new QTextEdit(this);
    this->textEdit_p->setFixedHeight(38);
    this->textEdit_p->setReadOnly(true);
    this->textEdit_p->setWordWrapMode(QTextOption::NoWrap);
    this->textEdit_p->adjustSize();
    this->mainLayout_p->addWidget(this->textEdit_p, 1, 0, 1, 4);

}

void AlarmBell::UpdateTaskList()
{
    if (model_p == nullptr || this->taskList_p == nullptr)
    {
        return;
    }

    model_p->clear();
    if (this->tasks.isEmpty() == false)
    {
        for (int i = 0; i < this->tasks.count(); i ++)
        {
            Task* task = this->tasks.at(i);

            model_p->setItem(i, 1, new QStandardItem(task->GetTime()));
            model_p->setItem(i, 2, new QStandardItem(task->GetPath()));

            RemoveButton *removeButton_p = new RemoveButton;
            QObject::connect(removeButton_p, SIGNAL(onClicked(RemoveButton*)), this, SLOT(RemoveButtonClicked(RemoveButton*)));
            removeButton_p->SetId(task->GetID());
            removeButton_p->setText("X");
            taskList_p->setIndexWidget(model_p->index(i, 0), removeButton_p);
        }
    }

    this->taskList_p->setColumnWidth(0, 80);
    this->taskList_p->setColumnWidth(1, 40);
    this->taskList_p->setColumnWidth(2, 300);
}

void AlarmBell::UpdateThread()
{
    if (taskThread_p != nullptr)
    {
        taskThread_p->exit();
        taskThread_p = nullptr;
    }

    taskThread_p = new TaskThread;
    taskThread_p->SetTasks(this->tasks);
    taskThread_p->start();
}

void AlarmBell::AddBellTask(bool)
{
    if (this->textEdit_p == nullptr || this->hourEdit_p == nullptr || this->minuteEdit_p == nullptr)
    {
        return;
    }

    QString targetPath = this->textEdit_p->toPlainText().toUtf8();
    if (targetPath.isEmpty() == true)
    {
        return;
    }

    int hour = hourEdit_p->time().hour();
    int minute = minuteEdit_p->time().minute();
    QString time = QString("%1:%2").arg(hour).arg(minute);

    // Write data to tFile
    WriteDataTotFile(new Task(time, targetPath));

    // Update
    UpdateTaskList();

    UpdateThread();
}

void AlarmBell::WriteDataTotFile(Task *task_p)
{
    if (task_p == nullptr || task_p->IsEmpty())
    {
        return;
    }

    this->tasks.append(task_p);

    QJsonObject jObj;
    jObj.insert("id", task_p->GetID());
    jObj.insert("time", task_p->GetTime());
    jObj.insert("path", task_p->GetPath());

    QJsonObject jObject = jDoc.object();
    QJsonArray jArray = jObject.value("data").toArray();

    jArray.append(jObj);
    jObject.insert("data", jArray);

    jDoc.setObject(jObject);
    M_DEBUG << jDoc.toJson();

    tFile.setFileName(tFilePath);
    if (tFile.open(QIODevice::ReadWrite | QFile::Text | QFile::Truncate))
    {
        tFile.write(jDoc.toJson());
        tFile.close();
    }
}

void AlarmBell::PickFile()
{
    if (this->textEdit_p == nullptr)
    {
        return;
    }
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Audio only (*.mp3)"));
    this->textEdit_p->setText(filePath);
}

void AlarmBell::RemoveButtonClicked(RemoveButton *button)
{
    if (button == nullptr)
    {
        return;
    }

    M_DEBUG << "ID:" << button->GetId();

    int targetId = button->GetId();
    int removeTarget = -1;

    tFile.setFileName(tFilePath);
    if (tFile.open(QIODevice::ReadWrite | QFile::Text | QFile::Truncate))
    {

        QJsonObject jObject = jDoc.object();
        QJsonArray jArray = jObject.value("data").toArray();

        for (int i = 0; i < jArray.count(); i ++)
        {
            QJsonObject jData = jArray.at(i).toObject();
            if (jData.isEmpty()) continue;
            if (jData.value("id").toInt() == targetId)
            {
                removeTarget = i;
            }
        }

        jArray.removeAt(removeTarget);
        jObject.insert("data", jArray);

        jDoc.setObject(jObject);

        tFile.write(jDoc.toJson());
        tFile.close();
    }

    for (int i = 0; i < this->tasks.count(); i ++)
    {
        Task *task_p = this->tasks.at(i);

        if (task_p ==nullptr) continue;
        if (task_p->GetID() == targetId)
        {
            removeTarget = i;
        }
    }
    this->tasks.removeAt(removeTarget);

    UpdateTaskList();
}

