#include <map>
#include <mach/mach.h>
#include <servers/bootstrap.h>

#include "server.h"

std::map<mach_port_t, carol_client_t> clients;
std::map<mach_port_t, carol_passport_t> passports;
uint64_t next_id;

mach_port_t port_set;

carol_client_t carol_lookup_client(mach_port_t port) {
    const auto it = clients.find(port);
    return it == clients.cend() ? nullptr : it->second;
}

mach_port_t carol_client_get_port(carol_client_t client) {
    return client->port;
}

carol_passport_t carol_lookup_passport(mach_port_t port) {
    const auto it = passports.find(port);
    return it == passports.cend() ? nullptr : it->second;
}

mach_port_t carol_passport_get_port(carol_passport_t passport) {
    return passport->port;
}

mach_port_t make_port() {
    mach_port_t port;
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
    mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
    mach_port_insert_member(mach_task_self(), port, port_set);
    return port;
}

kern_return_t carol_check_in(mach_port_t carol, carol_client_t *client) {
    carol_client_t c = new carol_client();
    c->port = make_port();
    c->id = next_id++;
    clients[c->port] = c;
    *client = c;
    return KERN_SUCCESS;
}

kern_return_t carol_get_id(carol_client_t client, int64_t *id) {
    if (client == nullptr) {
        return KERN_INVALID_ARGUMENT;
    }
    *id = client->id;
    return KERN_SUCCESS;
}

kern_return_t carol_make_passport(carol_client_t client, carol_passport_t *passport) {
    if (client == nullptr) {
        return KERN_INVALID_ARGUMENT;
    }
    carol_passport_t p = new carol_passport();
    p->port = make_port();
    p->client = client;
    p->sealed = false;
    passports[p->port] = p;
    *passport = p;
    return KERN_SUCCESS;
}

kern_return_t carol_seal(carol_passport_t passport) {
    if (passport == nullptr) {
        return KERN_INVALID_ARGUMENT;
    }
    if (passport->sealed) {
        return KERN_FAILURE;
    }
    passport->sealed = true;
    return KERN_SUCCESS;
}

kern_return_t carol_get_passport_id(mach_port_t carol, carol_passport_t passport, int64_t *id) {
    if (passport == nullptr) { 
        return KERN_INVALID_ARGUMENT;
    }

    // if (!passport->sealed) {
    //     return KERN_FAILURE;
    // }

    *id = passport->client->id;
    return KERN_SUCCESS;
}

int main(int argc, const char* argv[]) {
    mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_PORT_SET, &port_set);
    mach_port_t service_port = make_port();
    bootstrap_register(bootstrap_port, "io.github.bugaevc.Carol", service_port);
    mach_msg_server(carol_server, carol_subsystem.maxsize, port_set, 0);
}
