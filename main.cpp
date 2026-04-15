#include "ccms_trdp.cpp"
#include <unistd.h>






int main(void)
{
   
    TrdpManager manager ;

    TRDP_ERR_T err ;

    getInfo p_info ;
    p_info.trdp_info = new TRDP_PD_INFO_T();
    p_info.data = new uint8_t[256];
    

    //init session
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


    //publish data
    err = manager.subscribe(
        1000,
        "10.0.0.54",
        "0.0.0.0",  
        5000000
    ) ;
    
     if (err != TRDP_NO_ERR){
        std::cout << "error during publish data : " << err << std::endl ;
        goto error ;
    }

    while (1){
        manager.processData();
        err = manager.getData(&p_info, 256);

        if (err == TRDP_NO_ERR){
            std::cout << "data received : " ;
            for(int i = 0; i < 4; i++){
                std::cout << std::hex << (int)p_info.data[i] << " ";
            }
            std::cout << "data receive but not print" << std::endl;
        }
        else if (err != TRDP_NODATA_ERR){
            std::cout << "error : " << err << std::endl;
            break;
        }

        usleep(10000);
    }
    
    //close session
    manager.terminateSession() ;
   

    return 0;

error :
    return -1 ;
}