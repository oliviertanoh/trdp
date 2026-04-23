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

TRDP_ERR_T TrdpManager::subscribe_publish_all_device(const uint8_t *p_subscribeData, const uint8_t *p_publishData){

    TRDP_ERR_T err = TRDP_UNKNOWN_ERR ;

    for (uint32_t i = 0 ; i < m_device.interface.nbTelegramParam ; i++){

        switch (m_device.interface.telegramParam[i].type)
        {
        case 1: // Publish
            err = publish(m_device.interface.telegramParam[i].comId,
                            *m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000,
                            p_publishData,
                            sizeof(p_publishData)) ;
             if (err != TRDP_NO_ERR){
                std::cout << "error during publish" << std::endl ;
                goto error ;
            }
            std::cout<< " Publish to Com ID : " << m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        case 2: // Subscribe
            err = subscribe(m_device.interface.telegramParam[i].comId,
                            *m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000);
            if (err != TRDP_NO_ERR){
                std::cout << "error during subscribe" << std::endl ;
                goto error ;
            }
            std::cout<< "Subscribe to Com ID : " << m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        case 3:
            err = publish(m_device.interface.telegramParam[i].comId,
                            *m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000,
                            p_subscribeData,
                            sizeof(p_subscribeData)) ;
            
            if (err != TRDP_NO_ERR){
                std::cout << "error during publish_ : " << err << std::endl ;
                goto error ;
            }

            err = subscribe(m_device.interface.telegramParam[i].comId,
                            *m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000);

            if (err != TRDP_NO_ERR){
                std::cout << "error during subscribe _ : " << err << std::endl ;
                goto error ;
            }

            std::cout<< "Subscribe and Publish to Com ID : " << m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        
        default:
                std::cout<< "Unable to subscribe or publish to any devices..." << std::endl ;
            break;
        }
    }

    err = TRDP_NO_ERR ;

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



error : 
    return err ;

}



void TrdpManager::displayConfiguration(){

    // 4. Affiche les données
    std::cout << "=== Configuration chargée ===" << std::endl;
    for (uint32_t i = 0 ; i < m_device.device.numInterfaceConfig ; i ++ ){
        std::cout << "----------------- [ Interface - " << i + 1 << " ] -----------------" << std::endl;
        std::cout << "name : " << m_device.device.interfaceConfig[i].ifName << std::endl ;
        std::cout << "ip : "
                << ((m_device.device.interfaceConfig[i].hostIp >> 24) & 0xFF) << "."
                << ((m_device.device.interfaceConfig[i].hostIp >> 16) & 0xFF) << "."
                << ((m_device.device.interfaceConfig[i].hostIp >> 8)  & 0xFF) << "."
                << ( m_device.device.interfaceConfig[i].hostIp        & 0xFF) << std::endl;
    }

    for (uint32_t i = 0 ; i < m_device.interface.nbTelegramParam ; i++){

        std::cout << "------------ [Telegram - " << i + 1 << "]" << std::endl;
        std::cout<< "Com ID : " << m_device.interface.telegramParam[i].comId << std::endl;
        switch (m_device.interface.telegramParam[i].type)
        {
        case 0:
            std::cout<< "Communication Type : " << "default, direction is not defined" << std::endl;
            break;
        case 1:
            std::cout<< "Communication Type : " << "published" << std::endl;
            break;
        case 2:
            std::cout<< "Communication Type : " << "Subscribe" << std::endl;
            break;
        case 3:
            std::cout<< "Communication Type : " << "Published and Subscribed" << std::endl;
            break;
        
        default:
            break;
        }
        
        if (m_device.interface.telegramParam[i].srcCnt != 0 ){
            for (uint32_t j = 0 ; j < m_device.interface.telegramParam[j].srcCnt ; j++){
                std::cout << "ip (source - "<< j + 1 << ") : " 
                            << *m_device.interface.telegramParam[j].pSrc->pUriHost1 << std::endl; ;
            }
        }

        if (m_device.interface.telegramParam[i].destCnt != 0 ){
            for (uint32_t j = 0 ; j < m_device.interface.telegramParam[j].srcCnt ; j++){
                std::cout << "ip (destination - "<< j + 1 << ") : " << *m_device.interface.telegramParam[j].pDest->pUriHost ;
            }
        }
        std::cout << std::endl ;
    }

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