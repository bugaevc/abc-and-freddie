#include <stdio.h>
#include <mach/mach.h>
#include <servers/bootstrap.h>

#include "server.h"
#include "carol/user.h"
#include "freddie/user.h"

mach_port_t carol, freddie;

kern_return_t bob_prove(mach_port_t bob, freddie_handle_t handle) {
    mach_port_t opaque_port;
    carol_passport_t passport;

    kern_return_t kr = freddie_read_handle(freddie, handle, &opaque_port, &passport);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "freddie_read_handle() = %s\n", mach_error_string(kr));
        return KERN_FAILURE;
    }

    int64_t id;

    kr = carol_get_passport_id(carol, passport, &id);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "carol_get_passport_id() = %s\n", mach_error_string(kr));
        return KERN_FAILURE;
    }

    printf("Success, ID is %lld\n", id);
    return KERN_SUCCESS;
}

int main() {
    kern_return_t kr;

    kr = bootstrap_look_up(bootstrap_port, "io.github.bugaevc.Carol", &carol);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to look up Carol: %s\n", mach_error_string(kr));
        return 1;
    }
    bootstrap_look_up(bootstrap_port, "io.github.bugaevc.Freddie", &freddie);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to look up Freddie: %s\n", mach_error_string(kr));
        return 1;
    }

    mach_port_t service_port;
    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &service_port);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to allocate the service port: %s\n", mach_error_string(kr));
        return 1;
    }
    kr = mach_port_insert_right(mach_task_self(), service_port, service_port, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to insert a send right: %s\n", mach_error_string(kr));
        return 1;
    }
    kr = bootstrap_register(bootstrap_port, "io.github.bugaevc.Bob", service_port);
    if (kr != KERN_SUCCESS) {
        fprintf(stderr, "Failed to claim the bootstrap name: %s\n", mach_error_string(kr));
        return 1;
    }

    fprintf(stderr, "Listening...\n");
    mach_msg_server(bob_server, bob_subsystem.maxsize + 100, service_port, 0);
}
