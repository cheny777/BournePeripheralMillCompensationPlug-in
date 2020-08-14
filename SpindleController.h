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





/*主轴控制*/
class SpindleController
{
public:
    SpindleController();
    /*初始化CI参数*/
    bool initCIValue();
    /*初始化PID参数*/
    bool initPID();

    void loop();

private:
    int m_CI[cilast];
    double m_pid[pidlast];

};

#endif // SPINDLECONTROLLER_H
