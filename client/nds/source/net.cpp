#include "./../../common/net.h"
#include "./../../common/utils.h"

#include <nds.h>
#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

Net* net;

class NetNDS: public Net {
    public:
        NetNDS(const char* _addr, int _port) {
            if(!Wifi_InitDefault(WFC_CONNECT)) {
                consolef("Unable to start wifi.\n");
                consolef("Please check your internet switch and ensure that you configured\n");
                consolef("a connection (DS connection if running on a 3DS)\n");
                dsExit(1);
            }
            addr = (char*)malloc(sizeof(char) * strlen(_addr) + 1);
            memcpy(addr, _addr, sizeof(char) * strlen(_addr) + 1);
            port = _port;
            struct hostent * myhost = gethostbyname(_addr);
            if(myhost == NULL) {
                consolef("Unable to resolve %s.\n", _addr);
                dsExit(1);
            }
            remote.sin_family = AF_INET;
            remote.sin_port = htons(_port);
            remote.sin_addr.s_addr= *( (unsigned long *)(myhost->h_addr_list[0]) );
        }

        ~NetNDS() {
            free(addr);
        }
};

void initNet(const char* server, int port) {
    consolef("Starting networking...\n");
    net = new NetNDS(server, port);
}

Net* getNet() {
    return net;
}

void closeNet() {
    delete net;
}