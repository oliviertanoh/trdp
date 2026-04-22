#include "tau_xml.h"
#include "trdp_if_light.h"
#include <map>


// Get information when data is receive
struct getInfo {
    TRDP_PD_INFO_T *trdp_info ;
    uint8_t *data ;
} ;


// Read the TRDP device configuration parameters out of the XML configuration file
struct xmlDeviceConfig {
    TRDP_MEM_CONFIG_T   memoryConfig; //Memory configuration
    TRDP_DBG_CONFIG_T   debugConfig;  //Debug printout configuration for application use
    TRDP_COM_PAR_T *ComParameter; //Pointer to array of com parameters
    TRDP_IF_CONFIG_T* interfaceConfig; //Pointer to an array of interface parameter sets
    uint32_t numComParameter;
    uint32_t numInterfaceConfig; //Number of configured interfaces
};


// Read the interface relevant telegram parameters
struct xmlInterfaceConfig {
    char* interfaceName ; //Interface name
    TRDP_PD_CONFIG_T processDatadConfig  ; //PD default configuration for the interface
    TRDP_MD_CONFIG_T messageDataConfig ; //MD default configuration for the interface
    uint32_t nbTelegramParam ; //Number of configured telegrams
    TRDP_EXCHG_PAR_T *telegramParam ; //Pointer to array of telegram configurations
};


// Device configuation
struct deviceConfig {
    TRDP_XML_DOC_HANDLE_T configFile;
    xmlInterfaceConfig interface;
    xmlDeviceConfig device;
};

// Network configuration
struct networkConfig {
    TRDP_APP_SESSION_T session;
    std::map<uint32_t, TRDP_SUB_T> subscribe;  // clé = comId
    std::map<uint32_t, TRDP_PUB_T> publish;
};
