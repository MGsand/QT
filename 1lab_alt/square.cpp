#include "square.h"

square::square(QObject *parent)
    : QObject{parent}
{

}

float square::square_real(double a){
    return a*a;
}

float square::priam(float a, float b){
    return a*b;
}

float square::parall(float a, float b){
    return a*b;
}

float square::romb(float a, float b){
    return 0.5*a*b;
}

float square::trapecia(float a, float b,float h){
    return (a+b)/2*h;
}

float square::Krug(float a){
    return 3.14*a*a;
}

float square::sector(float a, float b){
    return (3.14*a*a)/360*b;
}

float square::treug(float a, float b){
    return 0.5*a*b;
}
