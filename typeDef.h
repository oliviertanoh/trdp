#include "tau_xml.h"
#include "trdp_if_light.h"


//read the TRDP device configuration parameters out of the XML configuration file
struct xmlDeviceConfig {
    TRDP_XML_DOC_HANDLE_T p_xmlFile; // Handle of the XML document prepared by tau_prepareXmlDoc
    TRDP_MEM_CONFIG_T   memoryConfig; //Memory configuration
    TRDP_DBG_CONFIG_T   debugConfig;  //Debug printout configuration for application use
    TRDP_COM_PAR_T *ComParameter; //Pointer to array of com parameters
    TRDP_IF_CONFIG_T* interfaceConfig; //Pointer to an array of interface parameter sets
    uint32_t numComParameter;
    uint32_t numInterfaceConfig; //Number of configured interfaces
};


// Read the interface relevant telegram parameters
struct xmlInterfaceConfig {
    TRDP_XML_DOC_HANDLE_T xmlFile; // Handle of the XML document prepared by tau_prepareXmlDoc
    char* interfaceName ; //Interface name
    TRDP_PD_CONFIG_T processDatadConfig  ; //PD default configuration for the interface
    TRDP_MD_CONFIG_T messageDataConfig ; //MD default configuration for the interface
    uint32_t numExchPararameter ; //Number of configured telegrams
    TRDP_EXCHG_PAR_T *pExchgPar ; //Pointer to array of telegram configurations
};


struct deviceConfig {
    xmlInterfaceConfig interface;
    xmlDeviceConfig device;
};
