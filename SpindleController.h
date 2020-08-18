#ifndef SPINDLECONTROLLER_H
#define SPINDLECONTROLLER_H
#include "bmath.h"
#include "pid.h"

enum CI
{
    Powerswitch = 0, /*力控开关*/
    axisload_1, /*轴1负载*/
    axisload_2, /*轴2负载*/
    axisloadfilter_1, /*轴1滤波后负载*/
    axisloadfilter_2, /*轴2滤波后负载*/
    Averagefilteringonoff, /*均值滤波开关*/
    Averagefiltering_1, /*均值滤波个数轴1*/
    Averagefiltering_2, /*均值滤波个数轴2*/
    lowfilteringonoff, /*低通滤波开关*/
    lowfilterproportion_1, /*低通滤波比例轴1*/
    lowfilterproportion_2, /*低通滤波比例轴2*/
    targetLoad_1, /*目标负载1*/
    targetLoad_2, /*目标负载2*/
    compensation_1, /*轴1补偿*/
    compensation_2, /*轴2补偿*/
    compensationscope, /*补偿范围*/
    pidkp_1, /*pid宏变量*/
    pidki_1,
    pidkd_1,
    pidkp_2,
    pidki_2,
    pidkd_2,
    compensationRate,/*补偿速度*/
    refreshdata,/*刷新数据*/
    cilast
};

enum PIDAP
{
    PIDAPAveragefiltering_1, /*均值滤波个数轴1*/
    PIDAPAveragefiltering_2, /*均值滤波个数轴2*/
    PIDAPlowfilterproportion_1, /*低通滤波比例轴1*/
    PIDAPlowfilterproportion_2, /*低通滤波比例轴2*/
    PIDAPcompensationscope, /*补偿范围*/
    PIDAPpidkp_1, /*pid宏变量*/
    PIDAPpidki_1,
    PIDAPpidkd_1,
    PIDAPpidkp_2,
    PIDAPpidki_2,
    PIDAPpidkd_2,
    PIDAPcompensationRate, /*补偿速度*/
    pidlast
};


#define  CI_ADDR_INS_STATE 221
#define  CI_ADDR_INS_VALUE 201
#define  CI_ADDR_INS_TBSER 203

#define CI_MODE 50000
/*运行状态*/
const int  CI_ADDR_NC_STATE = 50292;
#define  CI_ADDR_FC_STATE  102
#define  CI_ADDR_FC_SERIA  103


#define  CI_ADDR_ANG	   30278
#define  CI_ADDR_F_CMD     30428
#define  CI_ADDR_INDEX     30478


/*主轴控制*/
class SpindleController
{
public:
    SpindleController();
    /*初始化CI参数*/
    bool initCIValue();
    /*初始化PID参数*/
    bool initPIDandP();
    /*循环*/
    void loop();

private:
    /*控制*/
    void oncontrol();
    /*滤波*/
    double onfilter(double d,int n);
    /*设置滤波参数*/
    void parameterparameter();
    /*目标负载低通滤波*/
    double targetLoadLowpass(double d);

    /*ci变量*/
    int m_CI[cilast];
    /*pid参数*/
    double m_pid[pidlast];
    /*系统状态*/
    int m_nc_state;
    /*上一个系统状态*/
    int m_nc_state_last;
    /*力控状态*/
    int m_fc_state;
    /*上一个力控状态*/
    int m_fc_state_last;
    /*pid*/
    MPID::PID pid[2];
    /*低通*/
    LowPassFilter lowpass[2];
    /*均值*/
    AverageFiltering average[2];
    /*目标值低通*/
    LowPassFilter targetlowpass;
    /*补偿速度*/
    int m_compensationRate;

};

#endif // SPINDLECONTROLLER_H
