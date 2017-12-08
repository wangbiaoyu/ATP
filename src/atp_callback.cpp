#include "atp_callback.h"
#include "atp_impl.h"
#include "udp_util.h"

int atp_default_on_accept(atp_callback_arguments * args){
    return 0;
}
int atp_default_on_error(atp_callback_arguments * args){
    return 0;
}
int atp_default_on_read(atp_callback_arguments * args){
    return 0;
}
int atp_default_on_state_change(atp_callback_arguments * args){
    return 0;
}
int atp_default_get_read_buffer_size(atp_callback_arguments * args){
    return 0;
}
int atp_default_get_random(atp_callback_arguments * args){
    return 0;
}
int atp_default_sendto(atp_callback_arguments * args){
    atp_socket * socket = args->socket;
    const struct sockaddr * sa = args->addr;
    int res = sendto(socket->sockfd, args->data, args->length, 0, sa, args->addr_len);
    #if defined (ATP_LOG) && ATP_LOG >= LOGLEVEL_DEBUG
        // const sockaddr_in * sk = (const sockaddr_in *)sa;
        ATPAddrHandle handle(sa);
        log_debug(socket, "Call sendto port:%s .", handle.hash_code());
    #endif
    return res;
}
int atp_default_connect(atp_callback_arguments * args){
    // atp_socket * socket = args->socket;
    // const struct sockaddr * sa = args->addr;
    // return ::connect(socket->sockfd, sa, args->addr_len);
    return 0;
}
int atp_default_bind(atp_callback_arguments * args){
    atp_socket * socket = args->socket;
    const struct sockaddr * sa = args->addr;
    socklen_t len = args->addr_len;
    return ::bind(socket->sockfd, sa, len);
}
int atp_default_log(atp_callback_arguments * args){
    return 0;
}
int atp_default_log_normal(atp_callback_arguments * args){
    return 0;
}
int atp_default_log_debug(atp_callback_arguments * args){
    return 0;
}
int atp_default_opt_sndbuf(atp_callback_arguments * args){
    return 0;
}
int atp_default_opt_rcvbuf(atp_callback_arguments * args){
    return 0;
}