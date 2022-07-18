#ifndef REMOVEBUTTON_H
#define REMOVEBUTTON_H

#include <QPushButton>

class RemoveButton : public QPushButton
{
    Q_OBJECT
public:
    explicit RemoveButton(QWidget *parent_p = nullptr);
    ~RemoveButton();

    inline void SetId(int id) { this->id = id; }
    inline int GetId(void) { return this->id; }

public slots:
    void Clicked();

signals:
    void onClicked(RemoveButton*);

private:
    int id;


};

#endif // REMOVEBUTTON_H
