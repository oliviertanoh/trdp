#include "ccms_trdp.cpp"
#include <unistd.h>


    

int main(void)
{
   
    TrdpManager manager ;
    TRDP_ERR_T err ;

    const char* file = "device.xml" ;

    // Data to send
    char pubBuffer [64] = {'h', 'e', 'y','-','o','l','i','v','i','e','r'} ;
    const uint8_t *pubData = (const uint8_t *)pubBuffer ;

    // Get data
    getInfo p_info ;
    char subBuffer [64] ;
    const uint8_t *subData = (const uint8_t *)pubBuffer ;


    //init session
    err = manager.readConfiguration(file);

    if (err != TRDP_NO_ERR){
        std::cout << "error during reading conf : " << err << std::endl;
        goto error;
    }
    
    std::cout << std::endl << "Configuration loaded successfully... ! " << std::endl ;

    manager.displayConfiguration() ;

    std::cout << std::endl ;


    // Init Session
    err = manager.initSession() ;
    if (err != TRDP_NO_ERR){
        std::cout << "error during init session : " << err << std::endl ;
        goto error ;
    }
    std::cout << "Initialization successful... ! " << std::endl ;


    // Open session
    err = manager.openSession("10.0.0.54");
    if (err != TRDP_NO_ERR){
        std::cout << "error during open session : " << err << std::endl;
        goto error;
    }
    std::cout << "Session opened successfully... ! " << std::endl ;


    // Subscribe & Publish all device
    err = manager.subscribe_publish_all_device(subData, pubData);
    if (err != TRDP_NO_ERR){
        std::cout << "Error during subscription and publishing : " << err << std::endl;
        goto error;
    }
    std::cout << "subscription and publishing done successfully ! " << std::endl ;

    
    
    while (1)
    {
        //Publish data
        manager.processData() ;
        err = manager.sendData(2001, pubData, sizeof(pubData)) ;
        if (err != TRDP_NO_ERR){
            std::cout << "Data not sent..." << std ::endl ;
        }

        std::cout << "data sent !" << std::endl ;
        usleep(1000000);
    }
    

    //close session
    manager.terminateSession() ;

   

    return 0;

error :
    return -1 ;
}