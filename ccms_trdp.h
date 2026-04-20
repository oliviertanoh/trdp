#ifndef __CCMS__TRDP__
#define __CCMS__TRDP__
#include <cstdint>
#include <sys/select.h>  
#include <arpa/inet.h>
#include <vector>
#include "typeDef.h"

#define GOTO_ERROR_NON_SUCESS(sucess) if (!sucess) { goto error ;}

struct getInfo {
    TRDP_PD_INFO_T *trdp_info ;
    uint8_t *data ;
} ;

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
                            uint32_t p_topoCount,
                            const char* p_sourceIp,
                            const char* p_destIp,
                            const uint32_t p_interval,
                            const uint8_t *p_data,
                            const uint32_t p_dataSize
        ) ; 

        



        TRDP_ERR_T readConfiguration(const char* p_nameFile) ;

        TRDP_ERR_T sendData (const UINT8  *p_data, const uint32_t p_dataSize) ;
        TRDP_ERR_T getData(getInfo *p_info, uint32_t p_dataSize) ;

        TRDP_ERR_T processData() ;

    private :
        TRDP_APP_SESSION_T  m_appHandle;
        std::vector<TRDP_SUB_T> m_subHandle ;
        std::vector<TRDP_PUB_T> m_pubHandle ;

};


#endif /* __CCMS__TRDP__ */