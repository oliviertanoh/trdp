#include "ccms_trdp.h"
#include <iostream>



TRDP_ERR_T TrdpManager::initSession(){

    TRDP_APP_SESSION_T appHandle;
    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;
    const TRDP_MARSHALL_CONFIG_T* b ;


    err = tlc_init(NULL, NULL, NULL);
    if (err != TRDP_NO_ERR)
    {
        std::cout <<"tlc_init failed :" << err ;
        goto error ;
    }

error :
    return err ;
}

TRDP_ERR_T TrdpManager::openSession(const char* ipAddr){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    TRDP_IP_ADDR_T ownIp = (TRDP_IP_ADDR_T) inet_addr(ipAddr);

    err = tlc_openSession(
        &m_appHandle,
        ownIp,
        0,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (err != TRDP_NO_ERR){
        goto error ;
    }

error :
    return err ;
}

TRDP_ERR_T TrdpManager::subscribe(const uint32_t p_comid, 
                            const char* p_sourceIp,
                            const char* p_destIp,
                            const uint32_t p_timeout) {

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    TRDP_IP_ADDR_T sourceIp = (TRDP_IP_ADDR_T) inet_addr(p_sourceIp);
    TRDP_IP_ADDR_T destIp = (TRDP_IP_ADDR_T) inet_addr(p_destIp);

    err = tlp_subscribe(
        m_appHandle,
        &m_subHandle,
        NULL,
        NULL,
        0,
        p_comid,
        0,
        0,
        sourceIp,
        0,
        destIp,
        0,
        NULL,
        p_timeout,
        TRDP_TO_DEFAULT
    ) ;

    if (err != TRDP_NO_ERR){
        goto error ;
    }

error : 
    return err ;
}

TRDP_ERR_T TrdpManager::publish(const uint32_t p_comid,
                        uint32_t p_topoCount,
                        const char* p_sourceIp,
                        const char* p_destIp,
                        const uint32_t p_interval,
                        const uint8_t *p_data,
                        const uint32_t p_dataSize){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    TRDP_IP_ADDR_T sourceIp = (TRDP_IP_ADDR_T) inet_addr(p_sourceIp);
    TRDP_IP_ADDR_T destIp = (TRDP_IP_ADDR_T) inet_addr(p_destIp);

    err = tlp_publish(
        m_appHandle,
        &m_pubHandle,
        NULL,
        NULL,
        0,
        p_comid,
        0,
        0,
        sourceIp,
        destIp,
        p_interval,
        0,
        0,
        NULL,
        p_data,
        p_dataSize
    );

 if (err != TRDP_NO_ERR){
        goto error ;
    }

error : 
    return err ;
}


TRDP_ERR_T TrdpManager::sendData (const UINT8  *p_data, const uint32_t p_dataSize){
    
    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlp_put(
        m_appHandle,
        m_pubHandle,
        p_data,
        p_dataSize
    );

 if (err != TRDP_NO_ERR){
        goto error ;
    }

error : 
    return err ;
}

TRDP_ERR_T TrdpManager::processData()
{
    return tlc_process(m_appHandle, NULL, NULL);
}


TRDP_ERR_T TrdpManager::closeSession(){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlc_closeSession(m_appHandle) ;

    if (!err){
        goto error ;
    }

error : 
    return err ;
}


void TrdpManager::terminateSession(){
    tlc_terminate();
}