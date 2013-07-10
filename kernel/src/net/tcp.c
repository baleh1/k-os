#include "lib/int.h"
#include "common/swap.h"
#include "net/layer.h"
#include "net/protocols.h"
#include "video/log.h"

void recv_tran_tcp(net_interface_t *interface, packet_t *packet, void *raw, uint16_t len) {
    tcp_header_t *tcp = packet->tran.tcp = raw;
    raw = tcp + 1;
    len -= sizeof(tcp_header_t);
    
    tcp++; //Silence gcc
}
