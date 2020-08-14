#include "SpindleController.h"

SpindleController::SpindleController()
{



}

bool SpindleController::initCIValue()
{
    return  true;
}

bool SpindleController::initPID()
{
    vector<double > tempid;

    bool bl = funtiongather::getPIDValue(tempid);
    if(!bl)
    {
        tempid.clear();
        tempid.push_back(0.5);
        tempid.push_back(1.5);
        tempid.push_back(0);

        funtiongather::savePIDValue(tempid);
    }
    else
    {
        for(int i =0;i<(int)tempid.size();i++)
        {
            m_pid[i] = tempid[i];
        }
    }


    return  bl;
}

void SpindleController::loop()
{

}
