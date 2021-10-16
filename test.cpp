#include <iostream>
#include <memory>

  
#include <string.h>   
#include <unistd.h>  
#include <netdb.h>  
#include <net/if.h>  
#include <arpa/inet.h>  
#include <sys/ioctl.h>  
#include <sys/types.h>  
#include <sys/socket.h>  

class Test{
public:
    std::shared_ptr<Test> getSharedPtr(void) {
        return std::shared_ptr<Test>(this);
    }
    ~Test() {
        std::cout << "Test is distructed" << std::endl;
    }
};

int get_local_ip(const char *eth_inf, char *ip)  
{  
    
}  


int main(int argc, char **argv) {
    // std::shared_ptr<Test> ptr1(new Test);
    // std::shared_ptr<Test> ptr2 = ptr1->getSharedPtr();
    char ip[16];
    get_local_ip("ens33", ip);
    std::cout << ip << std::endl; 
    

    return 0;
}