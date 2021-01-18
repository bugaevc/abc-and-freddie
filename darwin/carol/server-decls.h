#include <mach/mach_types.h>

typedef struct carol_client {
    mach_port_t port;
    uint64_t id;
} *carol_client_t;

carol_client_t carol_lookup_client(mach_port_t);
mach_port_t carol_client_get_port(carol_client_t);


typedef struct carol_passport {
    mach_port_t port;
    carol_client_t client;
    boolean_t sealed;
} *carol_passport_t;

carol_passport_t carol_lookup_passport(mach_port_t);
mach_port_t carol_passport_get_port(carol_passport_t);
