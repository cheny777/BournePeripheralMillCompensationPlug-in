#include "SpindleController.h"

SpindleController::SpindleController()
{
    m_nc_state_last = -1;
    m_nc_state = -1;


}

bool SpindleController::initCIValue()
{
    vector<int> tempid;
    bool bl = funtiongather::getCIValue(tempid);
    if(!bl)
    {
        tempid.clear();
        for(int i =0;i<CI::cilast;i++)
        {
            tempid.push_back(600+i);
        }

        funtiongather::saveCIValue(tempid);
    }

    return  bl;
}

bool SpindleController::initPIDandP()
{
    vector<double> tempid;
    bool bl = funtiongather::getPIDAPValue(tempid);
    if(!bl)
    {
        tempid.clear();
        tempid.push_back(10);
        tempid.push_back(10);
        tempid.push_back(0.1);
        tempid.push_back(0.1);
        tempid.push_back(10);
        tempid.push_back(1);
        tempid.push_back(1);
        tempid.push_back(1);
        tempid.push_back(1);
        tempid.push_back(1);
        tempid.push_back(1);
        tempid.push_back(100);

        funtiongather::savePIDAPValue(tempid);
    }

    CCICALL::OnSetCI(m_CI[CI::Averagefiltering_1],tempid[0]);
    CCICALL::OnSetCI(m_CI[CI::Averagefiltering_2],tempid[1]);
    CCICALL::OnSetCI(m_CI[CI::lowfilterproportion_1],tempid[2]);
    CCICALL::OnSetCI(m_CI[CI::lowfilterproportion_2],tempid[3]);
    CCICALL::OnSetCI(m_CI[CI::compensationscope],tempid[4]);
    CCICALL::OnSetCI(m_CI[CI::pidkp_1],tempid[5]);
    CCICALL::OnSetCI(m_CI[CI::pidki_1],tempid[6]);
    CCICALL::OnSetCI(m_CI[CI::pidkd_1],tempid[7]);
    CCICALL::OnSetCI(m_CI[CI::pidkp_2],tempid[8]);
    CCICALL::OnSetCI(m_CI[CI::pidki_2],tempid[9]);
    CCICALL::OnSetCI(m_CI[CI::pidkd_2],tempid[10]);
    CCICALL::OnSetCI(m_CI[CI::compensationRate],tempid[11]);

    average[0].SETLowPassFilter_AverageS(tempid[0]);
    average[1].SETLowPassFilter_AverageS(tempid[1]);

    lowpass[0].setFilter(tempid[2]);
    lowpass[1].setFilter(tempid[3]);

    pid[0].setTotalCompensation(tempid[4]);
    pid[1].setTotalCompensation(tempid[4]);

    pid[0].setParams(tempid[5],tempid[6],tempid[7]);
    pid[1].setParams(tempid[8],tempid[9],tempid[10]);

    m_compensationRate = tempid[11];

    return  true;
}


void SpindleController::loop()
{
    while (1)
    {
        parameterparameter();
        oncontrol();
        usleep(10000);
    }
}

void SpindleController::oncontrol()
{
    /*获取运行状态*/
    m_nc_state = CCICALL::ongetcitoint(CI_ADDR_NC_STATE);
    /*加工状态改变*/
    bool is_start = false;
    bool is_stop = false;
    if(m_nc_state != m_nc_state_last)
    {
        if(m_nc_state ==1)
        {
            is_start = true;
        }
        else
        {
            is_stop = true;
        }
        m_nc_state_last = m_nc_state;
    }

    /*获取主轴负载*/
    double load[2];
    CCICALL::OnGetCI(m_CI[CI::axisload_1],load[0]);
    CCICALL::OnGetCI(m_CI[CI::axisload_2],load[1]);

    load[0] = onfilter(load[0],0);
    load[1] = onfilter(load[1],1);

    /*输出主轴负载*/
    CCICALL::OnSetCI(m_CI[CI::axisloadfilter_1],load[0]);
    CCICALL::OnSetCI(m_CI[CI::axisloadfilter_1],load[1]);

    if(is_start)
    {
        cout<<"start run"<<endl;

        /*判断是不是自动模式*/
        if(CCICALL::ongetcitoint(CI_MODE))
        {
            m_fc_state = CCICALL::ongetcitoint(m_CI[CI::Powerswitch]);
            if(m_fc_state != m_fc_state_last)
            {
                m_fc_state_last = m_fc_state;
                if(m_fc_state == 1)
                {
                    /*力控开启*/
                    pid->initialize();
                }
                if(m_fc_state == 2)
                {
                    /*力控清除*/
                }
            }
        }

        /*获取目标值*/
        double targetvalue[2];
        CCICALL::OnGetCI(m_CI[CI::targetLoad_1],targetvalue[0]);
        CCICALL::OnGetCI(m_CI[CI::targetLoad_2],targetvalue[1]);

        //cout<<"target:"<<targetvalue[0]<<"  "<<targetvalue[1]<<endl;
        /*目标值滤波*/
        targetvalue[0] = targetLoadLowpass(targetvalue[0]);
        targetvalue[1] = targetLoadLowpass(targetvalue[1]);
        /*pid计算补偿*/
        if(m_fc_state == 1)
        {
            double err[2];
            for(int i =0;i<2;i++)
            {
                pid[i].setValue(targetvalue[i]);
                err[i] = pid[i].iteration(load[i])/m_compensationRate;
                pid[i].setTotalCompensation(err[i]);
            }
            CCICALL::OnSetCI(m_CI[CI::compensation_1],err[0]);
            CCICALL::OnSetCI(m_CI[CI::compensation_2],err[1]);
        }
        /*清空补偿*/
        if(m_fc_state == 2)
        {
            double re[2];
            CCICALL::OnGetCI(m_CI[CI::compensation_1],re[0]);
            CCICALL::OnGetCI(m_CI[CI::compensation_2],re[1]);

            for(int i = 0;i<2;i++)
            {
                if(re[i]<0.02)
                {
                    CCICALL::OnSetCI(m_CI[CI::Powerswitch],0);
                }
                if(re[i]>=0)
                {
                    re[i]-=0.01;
                }
                else
                {
                    re[i]+=0.01;
                }
            }
            CCICALL::OnSetCI(m_CI[CI::compensation_1],0);
            CCICALL::OnSetCI(m_CI[CI::compensation_2],0);
        }

    }
    if(is_stop)
    {
        cout<<"stop run"<<endl;
        m_fc_state = CCICALL::ongetcitoint(m_CI[CI::Powerswitch]);

        if(m_fc_state == 2)
        {
            double re[2];
            CCICALL::OnGetCI(m_CI[CI::compensation_1],re[0]);
            CCICALL::OnGetCI(m_CI[CI::compensation_2],re[1]);

            for(int i = 0;i<2;i++)
            {
                if(re[i]<0.02)
                {
                    CCICALL::OnSetCI(m_CI[CI::Powerswitch],0);
                }
                if(re[i]>=0)
                {
                    re[i]-=0.01;
                }
                else
                {
                    re[i]+=0.01;
                }
            }
            CCICALL::OnSetCI(m_CI[CI::compensation_1],0);
            CCICALL::OnSetCI(m_CI[CI::compensation_2],0);
        }
    }

}

double SpindleController::onfilter(double d, int n)
{
    /*低通*/
    if(CCICALL::ongetcitoint(m_CI[CI::lowfilteringonoff]))
    {
        d = lowpass[n].onFilter(d);
    }
    /*均值*/
    if(CCICALL::ongetcitoint(m_CI[CI::Averagefilteringonoff]))
    {
        d = average[n].LowPassFilter_AverageS(d);
    }
    return d;
}

void SpindleController::parameterparameter()
{
    if(CCICALL::ongetcitoint(m_CI[CI::refreshdata]))
    {
        double tempid[pidlast];
        CCICALL::OnGetCI(m_CI[CI::Averagefiltering_1],tempid[0]);
        CCICALL::OnGetCI(m_CI[CI::Averagefiltering_2],tempid[1]);
        CCICALL::OnGetCI(m_CI[CI::lowfilterproportion_1],tempid[2]);
        CCICALL::OnGetCI(m_CI[CI::lowfilterproportion_2],tempid[3]);
        CCICALL::OnGetCI(m_CI[CI::compensationscope],tempid[4]);
        CCICALL::OnGetCI(m_CI[CI::pidkp_1],tempid[5]);
        CCICALL::OnGetCI(m_CI[CI::pidki_1],tempid[6]);
        CCICALL::OnGetCI(m_CI[CI::pidkd_1],tempid[7]);
        CCICALL::OnGetCI(m_CI[CI::pidkp_2],tempid[8]);
        CCICALL::OnGetCI(m_CI[CI::pidki_2],tempid[9]);
        CCICALL::OnGetCI(m_CI[CI::pidkd_2],tempid[10]);
        CCICALL::OnGetCI(m_CI[CI::compensationRate],tempid[11]);

        average[0].SETLowPassFilter_AverageS(tempid[0]);
        average[1].SETLowPassFilter_AverageS(tempid[1]);

        lowpass[0].setFilter(tempid[2]);
        lowpass[1].setFilter(tempid[3]);

        pid[0].setTotalCompensation(tempid[4]);
        pid[1].setTotalCompensation(tempid[4]);

        pid[0].setParams(tempid[5],tempid[6],tempid[7]);
        pid[1].setParams(tempid[8],tempid[9],tempid[10]);

        m_compensationRate = tempid[11];

        vector<double > tempidv;
        for(int i =0;i<pidlast;i++)
        {
            tempidv.push_back(tempid[i]);
        }
        funtiongather::savePIDAPValue(tempidv);

        CCICALL::OnSetCI(m_CI[CI::refreshdata],0);
    }
}

double SpindleController::targetLoadLowpass(double d)
{
    /*低通*/
    return targetlowpass.onFilter(d);
}


