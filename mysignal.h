#ifndef MYSIGNAL_H
#define MYSIGNAL_H
#include <QObject>

class Mysignal : public QObject
{
    Q_OBJECT

public:
    int type, id;
    double dis;
    Mysignal()
    {
        type = id = -1;
        dis = 0;
    }
    void setValue(int t, int v)
    {
        if (t != type || v != id)
        {
            type = t, id = v;
            emit valueChanged(type, id);
        }
    }
    void setValue(double d)
    {
        if (d != dis)
        {
            dis = d;
            emit valueChanged1(dis);
        }
    }

signals:
    void valueChanged(int type, int id);
    void valueChanged1(double dis);
};
extern Mysignal MS;
#endif // MYSIGNAL_H

