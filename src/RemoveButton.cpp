#include "RemoveButton.h"

RemoveButton::RemoveButton(QWidget* parent_p) : QPushButton(parent_p)
{
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(Clicked()));
}

RemoveButton::~RemoveButton()
{

}

void RemoveButton::Clicked()
{
    emit onClicked(this);
}
