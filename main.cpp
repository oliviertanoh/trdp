#include "ccms_trdp.cpp"
#include <unistd.h>





int main(void)
{
   
    TrdpManager manager ;

    TRDP_ERR_T err ;
    const uint8_t data [] = {0x01, 0x02, 0x03, 0x04} ;
    const uint8_t *p_data = data ;
    
    
    

    //init session
    err = manager.initSession() ;
    if (err != TRDP_NO_ERR){
        std::cout << "error during init session : " << err << std::endl ;
        goto error ;
    }
    std::cout << "initialization successful ! " << std::endl ;


    //open session
    err = manager.openSession("0.0.0.0") ;
     if (err != TRDP_NO_ERR){
        std::cout << "error during open session : " << err << std::endl ;
        goto error ;
    }

    //publish data
    err = manager.publish(
        1000,
        0,
        "0.0.0.0",
        "239.255.0.1",
        10000,
        p_data,
        sizeof(data)
    ) ;
    
     if (err != TRDP_NO_ERR){
        std::cout << "error during publish data : " << err << std::endl ;
        goto error ;
    }

    while (1){
        manager.processData();
        manager.sendData(data, sizeof(data));
        if (err != TRDP_NO_ERR){
            std::cout << "error during sending data : " << err << std::endl ;
            break ;
        }
        std::cout << "data sent.." << std::endl ;
        usleep(1000000); 
    }
    
    //close session
    manager.terminateSession() ;
   

    return 0;

error :
    return -1 ;
}