#ifndef __CCMS__TRDP__
#define __CCMS__TRDP__

#include <cstdint>

class TrdpManager{

    public : 

        bool initSession ();
        void terminateSession();


    private : 
        uint8_t m_dataReceive ;
        uint8_t m_dataSend ;

};



#endif /* __CCMS__TRDP__ */