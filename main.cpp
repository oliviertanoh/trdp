#include "ccms_trdp.cpp"
#include <unistd.h>






int main(void)
{
   
    TrdpManager manager ;
    TRDP_ERR_T err ;

    const char* file = "device.xml" ;

    //init session
    
    err = manager.readConfiguration(file);

    if (err != TRDP_NO_ERR){
        std::cout << "error during reading conf : " << err << std::endl;
        goto error;
    }

    err = manager.initSession() ;
    if (err != TRDP_NO_ERR){
        std::cout << "error during init session : " << err << std::endl ;
        goto error ;
    }
    std::cout << "initialization successful ! " << std::endl ;

    err = manager.openSession("0.0.0.0");
    if (err != TRDP_NO_ERR){
        std::cout << "error during open session : " << err << std::endl;
        goto error;
    }

    //close session
    manager.terminateSession() ;
   

    return 0;

error :
    return -1 ;
}