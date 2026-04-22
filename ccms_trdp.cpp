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
        &m_network.session,
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

    m_network.subscribe[p_comid] ;

    err = tlp_subscribe(
        m_network.session,
        &m_network.subscribe[p_comid],
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
                        const char* p_sourceIp,
                        const char* p_destIp,
                        const uint32_t p_interval,
                        const uint8_t *p_data,
                        const uint32_t p_dataSize){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    TRDP_IP_ADDR_T sourceIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_sourceIp));
    TRDP_IP_ADDR_T destIp = (TRDP_IP_ADDR_T) ntohl(inet_addr(p_destIp));

    m_network.publish[p_comid] ;

    err = tlp_publish(
        m_network.session,
        &m_network.publish[p_comid],
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
    
    TRDP_PD_CONFIG_T p_pdConfig  ;
    TRDP_MD_CONFIG_T p_mdConfig ;
    
    TRDP_MEM_CONFIG_T   memConfig;
    TRDP_DBG_CONFIG_T   dbgConfig;
    TRDP_EXCHG_PAR_T *pExchgPar  = NULL;
    TRDP_COM_PAR_T *pComPar = NULL;

    uint32_t numIfConfig = 0;
    uint32_t numComPar = 0;
    uint32_t numExchgPar = 0;

    char* p_interfaceName = NULL ;
    

    // ecrire dans un pointeur null ?

    err = tau_prepareXmlDoc(p_nameFile, &m_device.configFile) ;

    if (err != TRDP_NO_ERR){
        goto error ;
    }


    err = tau_readXmlDeviceConfig(
        &m_device.configFile, 
        &m_device.device.memoryConfig,
        &m_device.device.debugConfig,
        &m_device.device.numComParameter,
        &m_device.device.ComParameter,
        &m_device.device.numInterfaceConfig,
        &m_device.device.interfaceConfig
    );

    if (err != TRDP_NO_ERR){
        goto error ;
    }

    if (m_device.device.numInterfaceConfig > 0)
    {
        m_device.interface.interfaceName = m_device.device.interfaceConfig->ifName ;
    }
    
    err = tau_readXmlInterfaceConfig(
        &m_device.configFile,
        m_device.interface.interfaceName,
        NULL,
        &m_device.interface.processDatadConfig,
        &m_device.interface.messageDataConfig,
        &m_device.interface.nbTelegramParam,
        &m_device.interface.telegramParam
    );

    if (err != TRDP_NO_ERR){
        goto error ;
    }



    // for (UINT32 i = 0; i < numIfConfig; i++)
    // {
    //     std::cout << "--- Interface " << i << " ---" << std::endl;
    //     std::cout << "  name      : " << pIfConfig[i].ifName    << std::endl;
    //     std::cout << "  networkId : " << (int)pIfConfig[i].networkId << std::endl;
    //     std::cout << "  hostIp    : " << (pIfConfig[i].hostIp >> 24 & 0xFF) << "."
    //                                   << (pIfConfig[i].hostIp >> 16 & 0xFF) << "."
    //                                   << (pIfConfig[i].hostIp >> 8  & 0xFF) << "."
    //                                   << (pIfConfig[i].hostIp       & 0xFF) << std::endl;
    // }

    // std::cout << "--- PD Config ---" << std::endl;
    // std::cout << "  timeout   : " << p_pdConfig.timeout   << " µs" << std::endl;
    // std::cout << "  port      : " << p_pdConfig.port      << std::endl;

    // std::cout << "--- MD Config ---" << std::endl;
    // std::cout << "  replyTimeout   : " << p_mdConfig.replyTimeout   << " µs" << std::endl;
    // std::cout << "  confirmTimeout : " << p_mdConfig.confirmTimeout << " µs" << std::endl;
    // std::cout << "  udpPort        : " << p_mdConfig.udpPort        << std::endl;


    // std::cout << "--- Devices configuration ---" << std::endl;
    // std::cout << "  Port Com   : " << numComPar  << std::endl;

    // std::cout << "id :  " << pComPar->id << std::endl;
    // std::cout << "qos :  " << (int)pComPar->sendParam.qos << std::endl;
    // std::cout << "ttl :  " << (int)pComPar->sendParam.ttl << std::endl;
    // std::cout << "retries :  " << (int)pComPar->sendParam.retries << std::endl;


error : 
    return err ;

}


TRDP_ERR_T TrdpManager::sendData (uint32_t p_comid, const uint8_t  *p_data, const uint32_t p_dataSize){
    
    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlp_put(
        m_network.session,
        m_network.publish[p_comid],
        p_data,
        p_dataSize
    );

 if (err != TRDP_NO_ERR){
        goto error ;
    }

error : 
    return err ;
}

TRDP_ERR_T TrdpManager::getData(uint32_t p_comid, getInfo *p_info, uint32_t p_dataSize){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlp_get(
        m_network.session,
        m_network.subscribe[p_comid],
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
    return tlc_process(m_network.session, NULL, NULL);
}


TRDP_ERR_T TrdpManager::closeSession(){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR  ;

    err = tlc_closeSession(m_network.session) ;

    if (!err){
        goto error ;
    }

error : 
    return err ;
}


void TrdpManager::terminateSession(){
    tlc_terminate();
}