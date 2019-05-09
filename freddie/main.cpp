#include <map>
#include <mach/mach.h>
#include <servers/bootstrap.h>

#include "server.h"
#include "carol/user.h"

std::map<mach_port_t, freddie_handle_t> handles;
mach_port_t port_set;

freddie_handle_t freddie_lookup_handle(mach_port_t port) {
    const auto it = handles.find(port);
    return it == handles.cend() ? nullptr : it->second;
}

mach_port_t freddie_handle_get_port(freddie_handle_t handle) {
    return handle->port;
}

mach_port_t make_port() {
    mach_port_t port;
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
    mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
    mach_port_insert_member(mach_task_self(), port, port_set);
    return port;
}

kern_return_t freddie_make_handle(mach_port_t freddie, mach_port_t opaque_port,
                                  carol_passport_t passport, freddie_handle_t *handle)
{
    if (carol_seal(passport) != KERN_SUCCESS) {
        return KERN_FAILURE;
    }

    freddie_handle_t h = new freddie_handle();
    h->port = make_port();
    h->opaque_port = opaque_port;
    h->passport = passport;
    handles[h->port] = h;
    *handle = h;
    return KERN_SUCCESS;
}

kern_return_t freddie_read_handle(mach_port_t freddie, freddie_handle_t handle,
                                  mach_port_t *opaque_port, carol_passport_t *passport)
{
    if (handle == nullptr) {
        return KERN_INVALID_ARGUMENT;
    }
    *opaque_port = handle->opaque_port;
    *passport = handle->passport;
    return KERN_SUCCESS;
}

int main(int argc, const char* argv[]) {
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET, &port_set);
    mach_port_t service_port = make_port();
    bootstrap_register(bootstrap_port, "io.github.bugaevc.Freddie", service_port);
    mach_msg_server(freddie_server, freddie_subsystem.maxsize, port_set, 0);
}
