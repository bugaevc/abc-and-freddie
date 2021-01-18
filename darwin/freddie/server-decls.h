#include <mach/mach_types.h>
#include "carol/user-types.h"

typedef struct freddie_handle {
    mach_port_t port;
    mach_port_t opaque_port;
    carol_passport_t passport;
} *freddie_handle_t;

freddie_handle_t freddie_lookup_handle(mach_port_t);
mach_port_t freddie_handle_get_port(freddie_handle_t);
