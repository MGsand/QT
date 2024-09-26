#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>

class square : public QObject
{
    Q_OBJECT
public:
    explicit square(QObject *parent = nullptr);
   static float square_real(double a);
   static float priam(float a, float b);
   static float parall(float a, float b);
   static float romb(float a, float b);
   static float trapecia(float a, float b,float h);
   static float Krug(float a);
    static float sector(float a, float b);
   static float treug(float a, float b);
signals:

};

#endif // SQUARE_H
