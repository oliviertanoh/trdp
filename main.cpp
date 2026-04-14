#include "ccms_trdp.cpp"




int main(void)
{
   
    TrdpManager manager ;

    if (!manager.initSession()){
        std::cout << "error during init session" ;
    }

    std::cout << "initialization successful ! " << std::endl ;
    
    manager.terminateSession() ;
   

    return 0;
}