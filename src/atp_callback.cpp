/*
*   Calvin Neo
*   Copyright (C) 2017  Calvin Neo <calvinneo@calvinneo.com>
*   https://github.com/CalvinNeo/ATP
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License along
*   with this program; if not, write to the Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "atp_impl.h"
#include "udp_util.h"

void init_callbacks(ATPSocket * socket){
    socket->callbacks[ATP_CALL_ON_ERROR] = nullptr; 
    socket->callbacks[ATP_CALL_ON_STATE_CHANGE] = nullptr;
    socket->callbacks[ATP_CALL_GET_READ_BUFFER_SIZE] = nullptr;
    socket->callbacks[ATP_CALL_GET_RANDOM] = nullptr;
    socket->callbacks[ATP_CALL_LOG] = nullptr;
    socket->callbacks[ATP_CALL_SOCKET] = nullptr;
    socket->callbacks[ATP_CALL_CONNECT] = nullptr;
    socket->callbacks[ATP_CALL_BEFORE_ACCEPT] = [](atp_callback_arguments * args){
        // send ATP_PROC_REJECT to reject
        return ATP_PROC_OK;
    };
    socket->callbacks[ATP_CALL_ON_ACCEPT] = nullptr;
    socket->callbacks[ATP_CALL_BIND] = [](atp_callback_arguments * args){
        atp_socket * socket = args->socket;
        const struct sockaddr * sa = args->addr;
        socklen_t len = args->addr_len;
        int n = ::bind(socket->sockfd, sa, len);
        if(n < 0){
            return ATP_PROC_ERROR;
        }else{
            return ATP_PROC_OK;
        }
    };
    socket->callbacks[ATP_CALL_SENDTO] = [](atp_callback_arguments * args){
        atp_socket * socket = args->socket;
        const struct sockaddr * sa = args->addr;
        ATPPacket * pkt = (ATPPacket *)args->data;
        int n = sendto(socket->sockfd, args->data, args->length, 0, sa, args->addr_len);
        #if defined (ATP_LOG_AT_DEBUG) && defined(ATP_LOG_UDP)
            // const sockaddr_in * sk = (const sockaddr_in *)sa;
            ATPAddrHandle handle(sa);
            #if defined (ATP_LOG_AT_DEBUG) && defined(ATP_LOG_UDP)
                log_debug(socket, "Call sendto :%s, UDP Send %u bytes.", handle.to_string(), args->addr_len);
            #endif
        #endif
        if(n != args->length){
            return ATP_PROC_ERROR;
        }else{
            return ATP_PROC_OK;
        }
    };
    socket->callbacks[ATP_CALL_ON_RECV] = nullptr;
    socket->callbacks[ATP_CALL_ON_PEERCLOSE] = [](atp_callback_arguments * args){
        atp_socket * socket = args->socket;
        #if defined (ATP_LOG_AT_DEBUG) && defined(ATP_LOG_UDP)
            log_debug(socket, "Finish myself immediately when recv FIN from peer.");
        #endif
        return socket->close();
    };
    socket->callbacks[ATP_CALL_ON_DESTROY] = [](atp_callback_arguments * args){
        atp_socket * socket = args->socket;
        #if defined (ATP_LOG_AT_DEBUG) && defined(ATP_LOG_UDP)
            log_debug(socket, "Destroy socket.");
        #endif
        return ATP_PROC_FINISH;
    };
}

