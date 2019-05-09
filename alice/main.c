#include <mach/mach.h>
#include <servers/bootstrap.h>
#include <stdio.h>
#include <unistd.h>

#include "carol/user.h"
#include "freddie/user.h"

int main() {
    mach_port_t carol;
    bootstrap_look_up(bootstrap_port, "io.github.bugaevc.Carol", &carol);

    carol_client_t client;
    carol_check_in(carol, &client);

    int64_t id;
    carol_get_id(client, &id);
    printf("My ID is %lld\n", id);

    carol_passport_t passport;
    carol_make_passport(client, &passport);
    // carol_seal(passport);

    mach_port_t freddie;
    bootstrap_look_up(bootstrap_port, "io.github.bugaevc.Freddie", &freddie);

    freddie_handle_t handle;
    freddie_make_handle(freddie, MACH_PORT_NULL, passport, &handle);

    mach_port_t bob;
    bootstrap_look_up(bootstrap_port, "io.github.bugaevc.Bob", &bob);

    bob_prove(bob, handle);
}
