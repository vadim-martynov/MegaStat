#ifndef CIDENTRY_H
#define CIDENTRY_H

#include <QObject>

class CIdEntry
{
public:
    CIdEntry();

    quint16 type;
    QString id;
    QString name;

};

#endif // CIDENTRY_H
