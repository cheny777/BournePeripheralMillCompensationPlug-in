#include <QCoreApplication>
#include "SpindleController.h"
#include "bmath.h"


void AppLoop()
{
    SpindleController m_sc;

    if(!m_sc.initCIValue())
    {
        cout<<"open ci error!"<<endl;
        //return;
    }
    if(!m_sc.initPIDandP())
    {
        cout<<"open pid error!"<<endl;
        //return;
    }

    m_sc.loop();

}

int main(int argc, char *argv[])
{
    if(argc==2)
    {
        if(argv[1][0] == 's')
        {
            showflag = 1;
        }
        if(argv[1][0] == 'v')
        {
            cout<<"V1.0"<<endl;
        }
        if(argv[1][0] == 'h')
        {
            cout<<" s :show debug"<<endl;
            cout<<" h :show help"<<endl;
            cout<<" v :show version"<<endl;
            cout<<" s :show debug"<<endl;
        }
    }

    cout<<"start"<<endl;
    AppLoop();
    cout<<"successfil"<<endl;
    return 0;
}
