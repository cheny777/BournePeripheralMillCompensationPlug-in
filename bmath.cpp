#include "bmath.h"

int showflag = 0;

funtiongather::funtiongather()
{

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
            cvint.push_back(tem);
        }
    }
    else
    {
        return  false;
    }
    return  true;
}

bool funtiongather::getPIDValue(vector<double> &cvint, string strp)
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

        }
    }
    else
    {
        return  false;
    }
    return  true;
}

bool funtiongather::savePIDValue(vector<double> cvint, string strp)
{
    ofstream outfile;
    mvecter<string> pidannotation;
    pidannotation<<"比例KP"<<"积分ki"<<"微分kd";
    outfile.open(strp.c_str(),ios::trunc|ios::out);
    if(outfile.is_open())
    {
        for(int i =0;i<(int )cvint.size();i++)
        {
            outfile<<cvint[i]<<','<<pidannotation[i]<<endl;
        }
    }
    else
    {
        return  false;
    }
    return  true;
}


