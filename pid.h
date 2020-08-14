#ifndef PID_H
#define PID_H


class CPID
{
public:
    CPID();
    virtual ~CPID();
    void init(double p,double i,double d);
    double onErr(double e);
protected:
    bool frist_tag;
    double i_base;
    double d_err;
    double old_err;
    double ts;
    double kp;
    double ki;
    double kd;

};

class CMoveObj
{
public:
    CMoveObj();
    virtual ~CMoveObj();
    void SetInitState(double p,double v);
    double OnStep(double f);
protected:
    double mass;
    double pos;
    double vel;
    double TS;
    double TS2;
};

#endif // PID_H
