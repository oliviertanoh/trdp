#include "ccms_trdp.h"
#include <iostream>
#include <sys/select.h>  
#include "trdp_if_light.h"


bool TrdpManager::initSession(){

    TRDP_APP_SESSION_T appHandle;
    TRDP_ERR_T         err;

    bool result = false ;

    err = tlc_init(NULL, NULL, NULL);
    if (err != TRDP_NO_ERR)
    {
        std::cout <<"tlc_init failed :" << err ;
        goto error ;
    }

    result = true ;

error :
    return result ;
}


void TrdpManager::terminateSession(){
    tlc_terminate();
}


