#ifndef __CCMS__TRDP__
#define __CCMS__TRDP__
#include <cstdint>
#include <sys/select.h>  
#include <arpa/inet.h>
#include "typeDef.h"

#define GOTO_ERROR_NON_SUCESS(sucess) if (!sucess) { goto error ;}

class TrdpManager{

    public : 

        TRDP_ERR_T initSession ();
        void terminateSession();

        TRDP_ERR_T openSession(const char* ipAddr);
        TRDP_ERR_T closeSession();

        TRDP_ERR_T subscribe (const uint32_t p_comid,
                        const char* p_sourceIp,
                        const char* p_destIp,
                        const uint32_t p_timeout) ;

        TRDP_ERR_T publish (const uint32_t p_comid,
                            const char* p_sourceIp,
                            const char* p_destIp,
                            const uint32_t p_interval,
                            const uint8_t *p_data,
                            const uint32_t p_dataSize
        ) ; 

        
        TRDP_ERR_T readConfiguration(const char* p_nameFile) ;

        TRDP_ERR_T sendData (uint32_t p_comid, const uint8_t  *p_data, const uint32_t p_dataSize) ;
        TRDP_ERR_T getData(uint32_t p_comid, getInfo *p_info, uint32_t p_dataSize) ;

        TRDP_ERR_T processData() ;

    



        deviceConfig m_device ;

    private :
        networkConfig m_network ;

};


#endif /* __CCMS__TRDP__ */