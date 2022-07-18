#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#define M_DEBUG qDebug().noquote() << __FUNCTION__ << ", " << __LINE__ << ":"

#endif // COMMON_H
