#ifndef SPINDLECONTROLLER_H
#define SPINDLECONTROLLER_H
#include "bmath.h"
#include "pid.h"

enum CI
{
    Powerswitch,

    cilast
};

enum PIDV
{
    pidkp = 0,
    pidki,
    pidkd,
    pidlast
};





/*�������*/
class SpindleController
{
public:
    SpindleController();
    /*��ʼ��CI����*/
    bool initCIValue();
    /*��ʼ��PID����*/
    bool initPID();

    void loop();

private:
    int m_CI[cilast];
    double m_pid[pidlast];

};

#endif // SPINDLECONTROLLER_H
