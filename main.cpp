#include "ccms_trdp.cpp"
#include <unistd.h>


    

int main(void)
{
   
    TrdpManager manager ;
    TRDP_ERR_T err ;

    const char* file = "device.xml" ;

    // Data to send
    char buffer [64] ;
    const uint8_t *data = (const uint8_t *)buffer ;

    // Get data
    getInfo p_info ;
    p_info.trdp_info = new TRDP_PD_INFO_T();
    p_info.data = new uint8_t[256];

    //init session
    
    err = manager.readConfiguration(file);

    if (err != TRDP_NO_ERR){
        std::cout << "error during reading conf : " << err << std::endl;
        goto error;
    }
    
    std::cout << std::endl << "Configuration loaded successfully... ! " << std::endl ;

    // 4. Affiche les données
    std::cout << "=== Configuration chargée ===" << std::endl;
    for (uint32_t i = 0 ; i < manager.m_device.device.numInterfaceConfig ; i ++ ){
        std::cout << "----------------- [ Interface - " << i + 1 << " ] -----------------" << std::endl;
        std::cout << "name : " << manager.m_device.device.interfaceConfig[i].ifName << std::endl ;
        std::cout << "ip : "
                << ((manager.m_device.device.interfaceConfig[i].hostIp >> 24) & 0xFF) << "."
                << ((manager.m_device.device.interfaceConfig[i].hostIp >> 16) & 0xFF) << "."
                << ((manager.m_device.device.interfaceConfig[i].hostIp >> 8)  & 0xFF) << "."
                << ( manager.m_device.device.interfaceConfig[i].hostIp        & 0xFF) << std::endl;
    }

    for (uint32_t i = 0 ; i < manager.m_device.interface.nbTelegramParam ; i++){

        std::cout << "------------ [Telegram - " << i + 1 << "]" << std::endl;
        std::cout<< "Com ID : " << manager.m_device.interface.telegramParam[i].comId << std::endl;
        switch (manager.m_device.interface.telegramParam[i].type)
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
        

        if (manager.m_device.interface.telegramParam[i].srcCnt != 0 ){
            for (uint32_t j = 0 ; j < manager.m_device.interface.telegramParam[j].srcCnt ; j++){
                std::cout << "ip (source - "<< j + 1 << ") : " 
                            << *manager.m_device.interface.telegramParam[j].pSrc->pUriHost1 << std::endl; ;
            }
        }

        if (manager.m_device.interface.telegramParam[i].destCnt != 0 ){
            for (uint32_t j = 0 ; j < manager.m_device.interface.telegramParam[j].srcCnt ; j++){
                std::cout << "ip (destination - "<< j + 1 << ") : " << *manager.m_device.interface.telegramParam[j].pDest->pUriHost ;
            }
        }
        std::cout << std::endl ;
    }

    std::cout << std::endl ;

    err = manager.initSession() ;
    if (err != TRDP_NO_ERR){
        std::cout << "error during init session : " << err << std::endl ;
        goto error ;
    }
    std::cout << "Initialization successful... ! " << std::endl ;

    err = manager.openSession("0.0.0.0");
    if (err != TRDP_NO_ERR){
        std::cout << "error during open session : " << err << std::endl;
        goto error;
    }

    std::cout << "Session opened successfully... ! " << std::endl ;

    for (uint32_t i = 0 ; i < manager.m_device.interface.nbTelegramParam ; i++){

        switch (manager.m_device.interface.telegramParam[i].type)
        {
        case 1: // Publish
            manager.publish(manager.m_device.interface.telegramParam[i].comId,
                            *manager.m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *manager.m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000,
                            data,
                            sizeof(data)) ;
             if (err != TRDP_NO_ERR){
                std::cout << "error during publish" << std::endl ;
                goto error ;
            }
            std::cout<< " Publish to Com ID : " << manager.m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        case 2: // Subscribe
            err = manager.subscribe(manager.m_device.interface.telegramParam[i].comId,
                            *manager.m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *manager.m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000);
            if (err != TRDP_NO_ERR){
                std::cout << "error during subscribe" << std::endl ;
                goto error ;
            }
            std::cout<< "Subscribe to Com ID : " << manager.m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        case 3:
            err = manager.publish(manager.m_device.interface.telegramParam[i].comId,
                            *manager.m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *manager.m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000,
                            data,
                            sizeof(data)) ;
            
            if (err != TRDP_NO_ERR){
                std::cout << "error during publish_ : " << err << std::endl ;
                goto error ;
            }

            err = manager.subscribe(manager.m_device.interface.telegramParam[i].comId,
                            *manager.m_device.interface.telegramParam[i].pSrc->pUriHost1,
                            *manager.m_device.interface.telegramParam[i].pDest->pUriHost,
                            10000);

            if (err != TRDP_NO_ERR){
                std::cout << "error during subscribe _ : " << err << std::endl ;
                goto error ;
            }

            std::cout<< "Subscribe and Publish to Com ID : " << manager.m_device.interface.telegramParam[i].comId << std::endl ;
            break;
        
        default:
                std::cout<< "Unable to subscribe or publish to any devices..." << std::endl ;
            break;
        }
    }

    while (1)
    {
        //Publish data
        err = manager.sendData(2001, data , sizeof(data)) ;
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