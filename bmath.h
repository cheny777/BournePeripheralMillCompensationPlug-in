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
using namespace std;

extern int showflag;

#ifdef WIN32
const string CIPATH = "./bieldate.dat";
#else
const string CIPATH = "/home/Lynuc/Users/Config/bieldate.dat";
#endif

#ifdef WIN32
const string PIDPATH = "./bielpiddate.dat";
#else
const string PIDPATH = "/home/Lynuc/Users/Config/bielpiddate.dat";
#endif

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
    funtiongather();
    static bool getCIValue(vector<int > &cvint,string strp = CIPATH);
    static bool getPIDValue(vector<double > &cvint,string strp = PIDPATH);
    static bool saveCIValue(vector<int > cvint,string strp = CIPATH);
    static bool savePIDValue(vector<double > cvint,string strp = PIDPATH);
private:
    //static mvecter<string> ciannotation;
    //static mvecter<string> pidannotation;
};









#endif // BMATH_H
