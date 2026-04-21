#include "ccms_trdp.cpp"
#include <unistd.h>





int main(void)
{
   
    TrdpManager manager ;

    TRDP_ERR_T err ;
    char buffer [64] ;
    uint32_t counter = 0 ;
    const uint8_t *p_data = (uint8_t*)buffer ;
    
    
    

    //init session
    err = manager.initSession() ;
    if (err != TRDP_NO_ERR){
        std::cout << "error during init session : " << err << std::endl ;
        goto error ;
    }
    std::cout << "initialization successful ! " << std::endl ;


    //open session
    err = manager.openSession("172.29.173.22") ;
     if (err != TRDP_NO_ERR){
        std::cout << "error during open session : " << err << std::endl ;
        goto error ;
    }

    //publish data
    err = manager.publish(
        1000,
        0,
        "172.29.173.22",
        "10.0.0.54",
        10000,
        p_data,
        sizeof(buffer)
    ) ;
    
     if (err != TRDP_NO_ERR){
        std::cout << "error during publish data : " << err << std::endl ;
        goto error ;
    }

    while (1){

        sprintf(buffer, "wsl msg - %d \n", counter);

        manager.processData();

        manager.sendData(p_data, sizeof(buffer));

        if (err != TRDP_NO_ERR){
            std::cout << "error during sending data : " << err << std::endl ;
            break ;
        }

        std::cout << buffer ;
        counter ++ ;
        usleep(1000000); 
    }
    
    //close session
    manager.terminateSession() ;
   

    return 0;

error :
    return -1 ;
}