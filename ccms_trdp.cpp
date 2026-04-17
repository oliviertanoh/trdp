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

    TRDP_IP_ADDR_T ownIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(ipAddr));

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

    TRDP_IP_ADDR_T sourceIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_sourceIp));
    TRDP_IP_ADDR_T destIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_destIp));

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

    TRDP_IP_ADDR_T sourceIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_sourceIp));
    TRDP_IP_ADDR_T destIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_destIp));

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


TRDP_ERR_T TrdpManager::readConfiguration(const char* p_nameFile){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;
    TRDP_XML_DOC_HANDLE_T p_HandleDoc = {NULL} ;
    TRDP_IF_CONFIG_T* pIfConfig = NULL ;
    TRDP_COM_PAR_T *pComPar = NULL;
    TRDP_MEM_CONFIG_T   memConfig;
    TRDP_DBG_CONFIG_T   dbgConfig;
    char* p_interfaceName = NULL ;
    TRDP_PD_CONFIG_T p_pdConfig  ;
    TRDP_MD_CONFIG_T p_mdConfig ;
    uint32_t numIfConfig = 0;
    uint32_t numComPar = 0;
    uint32_t           numExchgPar = 0;
    TRDP_EXCHG_PAR_T *pExchgPar  = NULL;

    // ecrire dans un pointeur null ?

    err = tau_prepareXmlDoc(p_nameFile, &p_HandleDoc) ;

    if (err != TRDP_NO_ERR){
        goto error ;
    }


    err = tau_readXmlDeviceConfig(
        &p_HandleDoc,
        &memConfig,
        &dbgConfig,
        &numComPar,
        &pComPar,
        &numIfConfig,
        &pIfConfig
    );

    if (err != TRDP_NO_ERR){
        goto error ;
    }

    if (numIfConfig > 0)
    {
        p_interfaceName = pIfConfig->ifName ;
    }
    
    err = tau_readXmlInterfaceConfig(
        &p_HandleDoc,
        p_interfaceName,
        NULL,
        &p_pdConfig,
        &p_mdConfig,
        &numExchgPar,
        &pExchgPar
    );

    if (err != TRDP_NO_ERR){
        goto error ;
    }


    // 4. Affiche les données
    std::cout << "=== Configuration chargée ===" << std::endl;
    std::cout << "Nombre interfaces  : " << numIfConfig << std::endl;
    std::cout << "Nombre com-params  : " << numComPar   << std::endl ;


    for (UINT32 i = 0; i < numIfConfig; i++)
    {
        std::cout << "--- Interface " << i << " ---" << std::endl;
        std::cout << "  name      : " << pIfConfig[i].ifName    << std::endl;
        std::cout << "  networkId : " << (int)pIfConfig[i].networkId << std::endl;
        std::cout << "  hostIp    : " << (pIfConfig[i].hostIp >> 24 & 0xFF) << "."
                                      << (pIfConfig[i].hostIp >> 16 & 0xFF) << "."
                                      << (pIfConfig[i].hostIp >> 8  & 0xFF) << "."
                                      << (pIfConfig[i].hostIp       & 0xFF) << std::endl;
    }

    std::cout << "--- PD Config ---" << std::endl;
    std::cout << "  timeout   : " << p_pdConfig.timeout   << " µs" << std::endl;
    std::cout << "  port      : " << p_pdConfig.port      << std::endl;

    std::cout << "--- MD Config ---" << std::endl;
    std::cout << "  replyTimeout   : " << p_mdConfig.replyTimeout   << " µs" << std::endl;
    std::cout << "  confirmTimeout : " << p_mdConfig.confirmTimeout << " µs" << std::endl;
    std::cout << "  udpPort        : " << p_mdConfig.udpPort        << std::endl;



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

TRDP_ERR_T TrdpManager::getData(getInfo *p_info, uint32_t p_dataSize){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlp_get(
        m_appHandle,
        m_subHandle,
        p_info->trdp_info,
        p_info->data,
        &p_dataSize
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