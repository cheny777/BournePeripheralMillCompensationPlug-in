#include "bmath.h"

int showflag = 0;

funtiongather *funtiongather::fun = new funtiongather();


funtiongather::funtiongather()
{
    pidannotation<<"��ֵ�˲�������1"<<"��ֵ�˲�������2"<<"��ͨ�˲�������1"<<"��ͨ�˲�������1"<<"������Χ"
                <<"����KP_1"<<"����ki_1"<<"΢��kd_1"<<"����KP_2"<<"����ki_2"<<"΢��kd_2"
               <<"�����ٶ�";

    ciannotation<<"���غ꣬1����0��"<<"��1����"<<"��2����"<<"��1�˲�����"<<"��2�˲�����"
               <<"��ֵ�˲�����"<<"��ֵ�˲�������1"<<"��ֵ�˲�������2"<<"��ͨ�˲�����"<<"��ͨ�˲�������1"<<"��ͨ�˲�������2"<<"Ŀ�긺��1"<<"Ŀ�긺��2"<<"��1����"
              <<"��2����"<<"������Χ����֮��"<< "ͨ��1kp"<<"ͨ��1ki"<<"ͨ��1kd"<<"ͨ��2kp"<<"ͨ��2ki"
             <<"ͨ��2kd"<<"�����ٶ�"<<"ˢ������1��ˢ��0��ˢ�����";
}

funtiongather *funtiongather::getins()
{
    return  fun;
}

bool funtiongather::getCIValue(vector<int> &cvint, string strp)
{
    ifstream infile;
    infile.open(strp.c_str(),ios::in);
    if(infile.is_open())
    {
        string annotation;
        char comma;
        char buffer[256];
        int tem;
        while (!infile.eof())
        {
            infile.getline(buffer,256);
            istringstream istr(buffer);
            istr>>tem>>comma>>annotation;
            if(comma == ',')
            {
                cvint.push_back(tem);
            }
            comma = ' ';

        }
    }
    else
    {
        return  false;
    }
    return  true;
}

bool funtiongather::getPIDAPValue(vector<double> &cvint, string strp)
{
    ifstream infile;
    infile.open(strp.c_str(),ios::in);
    if(infile.is_open())
    {
        string annotation;
        char comma;
        char buffer[256];
        double tem;
        while (!infile.eof())
        {
            infile.getline(buffer,256);
            istringstream istr(buffer);
            istr>>tem>>comma>>annotation;
            if(comma == ',')
            {
                cvint.push_back(tem);
            }
            comma = ' ';
        }
    }
    else
    {
        return  false;
    }
    return  true;
}

bool funtiongather::saveCIValue(vector<int> cvint, string strp)
{
    ofstream outfile;
    outfile.open(strp.c_str(),ios::trunc|ios::out);
    if(outfile.is_open())
    {
        for(int i =0;i<(int )cvint.size();i++)
        {
            outfile<<cvint[i]<<','<<funtiongather::getins()->ciannotation[i]<<endl;
        }
    }
    else
    {
        return  false;
    }
    return  true;
}

bool funtiongather::savePIDAPValue(vector<double> cvint, string strp)
{
    ofstream outfile;
    outfile.open(strp.c_str(),ios::trunc|ios::out);
    if(outfile.is_open())
    {
        for(int i =0;i<(int )cvint.size();i++)
        {
            outfile<<cvint[i]<<','<<funtiongather::getins()->pidannotation[i]<<endl;
        }
    }
    else
    {
        return  false;
    }
    return  true;
}

static CCICALL * ciins = NULL;

CCICALL::CCICALL()
{
#ifdef _WIN32
#else
    m_macro_addr = CreateCIKernel();
#endif
}

CCICALL::~CCICALL()
{
#ifdef _WIN32
#else
    DestroyCIKernel(&m_macro_addr);
#endif
}

void CCICALL::OnDestory()
{
    if(ciins != NULL)
    {
        delete ciins;
        ciins = NULL;
    }
}

CCICALL * CCICALL::GetIns()
{
    if(ciins != NULL)
    {
        return ciins;
    }
    ciins = new CCICALL;
    return ciins;
}


void CCICALL::OnSetCI(int addr,double v)
{
    CCICALL * i = GetIns();
#ifdef WIN32
#else
    SetMacroVal(i->m_macro_addr,addr,v);
#endif

}

void CCICALL::OnGetCI(int addr,double &v)
{
    CCICALL * i = GetIns();
#ifdef WIN32
    v=0;
#else
    GetMacroVal(i->m_macro_addr,addr,v);
#endif
}

void CCICALL::OnSetCIAT(int addr,double v)
{
    CCICALL * i = GetIns();
#ifdef WIN32
#else
    SetAtMacroVal(i->m_macro_addr,addr,v);
#endif
}

void CCICALL::OnGetCIAT(int addr,double &v)
{
    CCICALL * i = GetIns();
#ifdef WIN32
    v=0;
#else
    GetAtMacroVal(i->m_macro_addr,addr,v);
#endif
}

int CCICALL::ongetcitoint(int addr)
{
    double v;
    CCICALL::OnGetCI(addr,v);
    return (int )(v+0.00001);
}
using namespace MPID;

PID::PID(double kp, double ki, double kd)
{
    setParams(kp, ki, kd);
    initialize();
    m_tc = 0;
    m_range = 10;
}

void PID::setParams(double kp, double ki, double kd)
{
    m_kp = kp;
    m_ki = ki;
    m_kd = kd;
}

void PID::setValue(double value)
{
    m_setValue = value;
}

double PID::iteration(double measured)
{
    double error = m_setValue - measured;
    double derror = error - m_error;

    m_sum += error;
    m_error = error;

    double r = m_kp * error + m_ki * m_sum + m_kd * derror;
    /*������Χ*/
    if(fabs(r+m_tc)>=m_range)
    {
        r = m_range - m_tc;
    }
    return r;
}

void PID::initialize()
{
    m_sum = 0;
    m_error = 0;
    m_tc = 0;
}

void PID::TotalCompensation(double d)
{
    m_tc += d;

}

void PID::setTotalCompensation(double d)
{
    m_range = d;
}


