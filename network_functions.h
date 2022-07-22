//
// Created by garri on 30.06.2022.
//

#ifndef ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
#define ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
#include <pthread.h>

#ifdef _WIN32
#include <winsock2.h>
#define socket_type SOCKET
#else
#define socket_type int
#endif


/*evaluates error, returns error message*/
int exit_error(char *error_message);
#ifdef _WIN32
int init_winsock();
#endif
socket_type create_socket();
void bind_socket(socket_type *sock, unsigned long address, unsigned short port);
void listen_socket(socket_type *sock);
void accept_socket(socket_type *sock, socket_type *new_socket);
void connect_socket(socket_type *socket, unsigned short port);
void TCP_send(socket_type *sock, char *data, size_t size);
void TCP_receive(socket_type *sock, char *data, size_t size);
void cleanup(void);

void* init_server();
int server_process();
int init_client();

#endif //ARTICLE_MANAGEMENT_1_2_NETWORK_FUNCTIONS_H
