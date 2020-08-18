#ifndef SPINDLECONTROLLER_H
#define SPINDLECONTROLLER_H
#include "bmath.h"
#include "pid.h"

enum CI
{
    Powerswitch = 0, /*���ؿ���*/
    axisload_1, /*��1����*/
    axisload_2, /*��2����*/
    axisloadfilter_1, /*��1�˲�����*/
    axisloadfilter_2, /*��2�˲�����*/
    Averagefilteringonoff, /*��ֵ�˲�����*/
    Averagefiltering_1, /*��ֵ�˲�������1*/
    Averagefiltering_2, /*��ֵ�˲�������2*/
    lowfilteringonoff, /*��ͨ�˲�����*/
    lowfilterproportion_1, /*��ͨ�˲�������1*/
    lowfilterproportion_2, /*��ͨ�˲�������2*/
    targetLoad_1, /*Ŀ�긺��1*/
    targetLoad_2, /*Ŀ�긺��2*/
    compensation_1, /*��1����*/
    compensation_2, /*��2����*/
    compensationscope, /*������Χ*/
    pidkp_1, /*pid�����*/
    pidki_1,
    pidkd_1,
    pidkp_2,
    pidki_2,
    pidkd_2,
    compensationRate,/*�����ٶ�*/
    refreshdata,/*ˢ������*/
    cilast
};

enum PIDAP
{
    PIDAPAveragefiltering_1, /*��ֵ�˲�������1*/
    PIDAPAveragefiltering_2, /*��ֵ�˲�������2*/
    PIDAPlowfilterproportion_1, /*��ͨ�˲�������1*/
    PIDAPlowfilterproportion_2, /*��ͨ�˲�������2*/
    PIDAPcompensationscope, /*������Χ*/
    PIDAPpidkp_1, /*pid�����*/
    PIDAPpidki_1,
    PIDAPpidkd_1,
    PIDAPpidkp_2,
    PIDAPpidki_2,
    PIDAPpidkd_2,
    PIDAPcompensationRate, /*�����ٶ�*/
    pidlast
};


#define  CI_ADDR_INS_STATE 221
#define  CI_ADDR_INS_VALUE 201
#define  CI_ADDR_INS_TBSER 203

#define CI_MODE 50000
/*����״̬*/
const int  CI_ADDR_NC_STATE = 50292;
#define  CI_ADDR_FC_STATE  102
#define  CI_ADDR_FC_SERIA  103


#define  CI_ADDR_ANG	   30278
#define  CI_ADDR_F_CMD     30428
#define  CI_ADDR_INDEX     30478


/*�������*/
class SpindleController
{
public:
    SpindleController();
    /*��ʼ��CI����*/
    bool initCIValue();
    /*��ʼ��PID����*/
    bool initPIDandP();
    /*ѭ��*/
    void loop();

private:
    /*����*/
    void oncontrol();
    /*�˲�*/
    double onfilter(double d,int n);
    /*�����˲�����*/
    void parameterparameter();
    /*Ŀ�긺�ص�ͨ�˲�*/
    double targetLoadLowpass(double d);

    /*ci����*/
    int m_CI[cilast];
    /*pid����*/
    double m_pid[pidlast];
    /*ϵͳ״̬*/
    int m_nc_state;
    /*��һ��ϵͳ״̬*/
    int m_nc_state_last;
    /*����״̬*/
    int m_fc_state;
    /*��һ������״̬*/
    int m_fc_state_last;
    /*pid*/
    MPID::PID pid[2];
    /*��ͨ*/
    LowPassFilter lowpass[2];
    /*��ֵ*/
    AverageFiltering average[2];
    /*Ŀ��ֵ��ͨ*/
    LowPassFilter targetlowpass;
    /*�����ٶ�*/
    int m_compensationRate;

};

#endif // SPINDLECONTROLLER_H
