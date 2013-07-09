#ifndef KERNEL_NET_DHCP_H
#define KERNEL_NET_DHCP_H

#include "lib/int.h"
#include "net/interface.h"

#define DHCP_PORT_CLIENT 68
#define DHCP_PORT_SERVER 67

void dhcp_start(net_interface_t *interface);
void dhcp_handle(net_interface_t *interface, void *packet, uint16_t len);

#endif
