#ifndef BMATH_H
#define BMATH_H
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>
#include <string>
#include <sstream>
#include <unistd.h>
#include <QStringList>
#include <math.h>
#include "cicomm.h"
using namespace std;

extern int showflag;

#ifdef WIN32
const string CIPATH = "./bieldate.dat";
#else
const string CIPATH = "/home/Lynuc/Users/Config/bieldate.dat";
#endif

#ifdef WIN32
const string PIDAPPATH = "./bielpidapdate.dat";
#else
const string PIDPATH = "/home/Lynuc/Users/Config/bielpidapdate.dat";
#endif



/*æ˘÷µ¬À≤®*/
class AverageFiltering
{
public:
    AverageFiltering()
    {
        AV = 10;
        junzhi = 0;
        SETLowPassFilter_AverageS(AV);
    }

    void SETLowPassFilter_AverageS(int num)
    {
        AV = num;
        Average_buf.clear();
        for(int i =0;i<num;i++)
        {
            Average_buf.push_back(0);
        }
        junzhi = 0;
    }

    double LowPassFilter_AverageS(double data)
    {
        double add=0;
        double  result;

        if(junzhi<AV)
        {
            Average_buf[junzhi] = data;
            junzhi++;
        }
        else
        {
            for(int i =0 ;i<AV ;i++)
            {
                Average_buf[i] = Average_buf[i+1];
            }
            Average_buf[AV-1] = data;
        }

        for(int i=0;i<junzhi+1;i++)
        {
            add += Average_buf[i];
        }
        result=add/double(junzhi+1);
        return result;
    }
private:
    /*æ˘÷µ¬À≤®*/
    int AV;
    vector<double> Average_buf;
    int junzhi;
};


/*µÕÕ®¬À≤®*/
class LowPassFilter
{
public:
    LowPassFilter()
    {
        FILTER_A = 0.1;
    }
    void setFilter(double d)
    {
        FILTER_A = d;
    }
    double onFilter(double newdata)
    {
        Value = (int)((float)newdata * FILTER_A + (1.0 - FILTER_A) * (float)Value);
        return Value;
    }
private:
    /*µÕÕ®¬À≤®*/
    double FILTER_A;
    double Value;
};


template<class T>
class mvecter
{
public:
    inline mvecter &operator<<(T t);
    T operator[](int num);
    void clear();
    T at(int num);

private:
    vector<T> elems;
};

template<class T>
mvecter<T> &mvecter<T>::operator<<(T t)
{
    elems.push_back(t);
    return *this;
}
template<class T>
T mvecter<T>::operator[](int num)
{
    if(num>elems.size())
    {
        return elems[0];
    }
    return elems[num];
}
template<class T>
void mvecter<T>::clear()
{
    elems.clear();
}
template<class T>
T mvecter<T>::at(int num)
{
    return elems[num];
}

class funtiongather
{
public:

    static bool getCIValue(vector<int > &cvint,string strp = CIPATH);
    static bool getPIDAPValue(vector<double > &cvint,string strp = PIDAPPATH);
    static bool saveCIValue(vector<int > cvint,string strp = CIPATH);
    static bool savePIDAPValue(vector<double > cvint,string strp = PIDAPPATH);
private:
    mvecter<string> ciannotation;
    mvecter<string> pidannotation;
    funtiongather();
    static funtiongather *getins();
    static funtiongather *fun;

};

class CCICALL
{
public:
    virtual ~CCICALL();
    static void OnDestory();
    static void OnSetCI(int addr,double v);
    static void OnGetCI(int addr,double &v);
    static void OnSetCIAT(int addr,double v);
    static void OnGetCIAT(int addr,double &v);
    static int ongetcitoint(int addr);
protected:
    CCICALL();
    static CCICALL * GetIns();
    int m_macro_addr;

};


namespace MPID {

class PID
{
public:
    PID(double kp = 0, double ki = 0, double kd = 0);
    void setParams(double kp, double ki, double kd);
    void setValue(double value);
    double iteration(double measured);
    /*≥ı ºªØ*/
    void initialize();
    /*¿€º”≤π≥•¡ø*/
    void TotalCompensation(double d);
    /*…Ë÷√≤π≥•∑∂Œß*/
    void setTotalCompensation(double d);
private:
    double m_kp, m_ki, m_kd;
    double m_setValue = 0, m_error = 0, m_sum = 0;
    double m_tc;
    double m_range;
};
}




#endif // BMATH_H
