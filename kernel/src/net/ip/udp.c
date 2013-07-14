#include "lib/int.h"
#include "common/swap.h"
#include "mm/cache.h"
#include "net/types.h"
#include "net/layer.h"
#include "net/interface.h"
#include "net/dhcp.h"
#include "net/nbns.h"
#include "net/protocols.h"
#include "video/log.h"

#include "checksum.h"

void udp_build(packet_t *packet, ip_t dst_ip, uint16_t src_port, uint16_t dst_port) {
    udp_header_t *hdr = kmalloc(sizeof(udp_header_t));

    hdr->src_port = swap_uint16(src_port);
    hdr->dst_port = swap_uint16(dst_port);
    hdr->length = swap_uint16(sizeof(udp_header_t) + packet->payload_size);

    uint32_t sum = 0;
    sum += ((uint16_t *) &packet->interface->ip)[0];
    sum += ((uint16_t *) &packet->interface->ip)[1];
    sum += ((uint16_t *) &dst_ip)[0];
    sum += ((uint16_t *) &dst_ip)[1];
    sum += swap_uint16((uint16_t) IP_PROT_UDP);
    sum += swap_uint16(sizeof(udp_header_t) + packet->payload_size);

    sum += hdr->src_port;
    sum += hdr->dst_port;
    sum += hdr->length;

    uint32_t len = packet->payload_size / sizeof(uint8_t);
    uint16_t *ptr = (uint16_t *) packet->payload;
    for (; len > 1; len -= 2) {
        sum += *ptr++;
    }

    if(len) {
        sum += *((uint8_t *) ptr);
    }

    hdr->checksum = sum_to_checksum(sum);

    packet->tran.udp = hdr;
    packet->tran_size = sizeof(udp_header_t);

    ip_build(packet, IP_PROT_UDP, dst_ip);
}

void udp_recv(packet_t *packet, void *raw, uint16_t len) {
    udp_header_t *udp = packet->tran.udp = raw;
    raw += sizeof(udp_header_t);
    len -= sizeof(udp_header_t);

    udp->src_port = swap_uint16(udp->src_port);
    udp->dst_port = swap_uint16(udp->dst_port);

    if(packet->interface->state == IF_UP && udp->src_port == DHCP_PORT_SERVER && udp->dst_port == DHCP_PORT_CLIENT) {
        dhcp_handle(packet, raw, len);
    } else if(packet->interface->state == IF_READY && udp->dst_port == NBNS_PORT) {
        nbns_handle(packet, raw, len);
    }
}