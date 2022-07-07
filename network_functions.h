//
// Created by garri on 30.06.2022.
//

#ifndef ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
#define ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
#include <winsock2.h>
#ifdef _WIN32
#define socket_type SOCKET
#else
#define socket int
#endif
/*evaluates error, returns error message*/
int exit_error(char *error_message);
#ifdef _WIN32
int init_winsock();
#endif
socket_type create_socket();
void bind_socket(SOCKET *sock, unsigned long address, unsigned short port);
void listen_socket(SOCKET *socket);
void accept_socket(SOCKET *socket, SOCKET *new_socket);
void connect_socket(SOCKET *socket, char *serv_addr, unsigned short port);
void TCP_send(SOCKET *socket, char *data, size_t size);
void TCP_receive(SOCKET *socket, char *data, size_t size);
void cleanup(void);

int init_server();
int init_client();

#endif //ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
