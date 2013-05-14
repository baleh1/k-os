#include "module.h"
#include "console.h"

static multiboot_module_t *mods;
static uint32_t count;

uint32_t module_count() {
    return count;
}

multiboot_module_t * module_get(uint32_t num) {
    return &mods[num];
}

void module_init(multiboot_info_t *mbd) {
    mods = mbd->mods;
    
    for(uint32_t i = 0; i < mbd->mods_count; i++) {
        kprintf("    - Module #%u at (0x%08X - 0x%08X) %s\n", i, mods[i].mod_start, mods[i].mod_end, mods[i].cmdline);
    }
}

