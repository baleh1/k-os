#include "lib/int.h"
#include "common/list.h"
#include "atomic/spinlock.h"
#include "net/types.h"
#include "net/dhcp.h"
#include "video/log.h"

static LIST_HEAD(interfaces);
static SPINLOCK_INIT(interface_lock);

void register_net_interface(net_interface_t *interface) {
    uint32_t flags;
    spin_lock_irqsave(&interface_lock, &flags);

    interface->ip = IP_NONE;

    interface->rx_total = 0;
    interface->tx_total = 0;

    interface->state = IF_DHCP;

    list_add(&interface->list, &interfaces);

    spin_unlock_irqstore(&interface_lock, flags);

    logf("net - interface registerd with MAC %X:%X:%X:%X:%X:%X",
        interface->mac.addr[0],
        interface->mac.addr[1],
        interface->mac.addr[2],
        interface->mac.addr[3],
        interface->mac.addr[4],
        interface->mac.addr[5]
    );

    dhcp_start(interface);
}

void unregister_net_interface(net_interface_t *interface) {
    uint32_t flags;
    spin_lock_irqsave(&interface_lock, &flags);

    list_rm(&interface->list);

    spin_unlock_irqstore(&interface_lock, flags);
}
