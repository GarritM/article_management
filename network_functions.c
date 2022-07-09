//
// Created by garri on 30.06.2022.
//

//TODO implement:
//          synchronize-download
//          synchronize-upload

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "user_interface.h"

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h> //seems to be required for some Windows-Versions

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef _WIN32
#define socket_type SOCKET
#else
#define socket_type int
#endif

#define BUF 1024

/*evaluates error, returns error message*/
int exit_error(char *error_message){
#ifdef _WIN32
    fprintf(stderr, "%s: %d\n", error_message, WSAGetLastError());
#else
    fprintf(stderr, "%s: %s\n", error_message, strerror(errno));
#endif
    return -1;
}
#ifdef _WIN32
/*init winsock 2.0*/
int init_winsock(){
    WORD wVersion;
    WSADATA wsaData;
    wVersion = MAKEWORD(2,0);
    if(WSAStartup(wVersion, &wsaData) != 0){
        return exit_error("An error during the initialization of \"Winsock\" occurred");
    }else{
        printf("\"Winsock\" was initialized successfully\n");
    }
    return 0;
}
/*"free" Winsock*/
void cleanup(void){
    WSACleanup(); //dont understand thus dont like it, counterpart to "WSAStartup()"
    printf("Winsock cleanup finished\n");
}
#endif


/*create a tcp, IPv4 Socket*/
socket_type create_socket(){
    socket_type sock = socket(AF_INET, SOCK_STREAM, 0); //AF_INET corresponds to IPv4, 0 is the default transmission protocol for the socket type
    if(sock < 0){
        exit_error("socket creation failed");
    }else{
        printf("socket is created\n");
    }
#ifdef __UNIX
    const int y = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));
#endif
    return sock;
}

/*bind socket to serveraddress*/
void bind_socket(socket_type *sock, unsigned long address, unsigned short port){
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server)); //sets memory of server to zeros
    server.sin_family = AF_INET; //specifies type of server to IPv4
    server.sin_addr.s_addr = htonl(address); //address given (and expected to be given) in host-byte-order is transformed to network-byte-order with "htonl()"
    server.sin_port = htons(port); //host to network-byte order conversion
#ifdef _WIN32
    if(bind(*sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR){
#else
        if(bind(*sock, (struct sockaddr*) &server, sizeof(server)) < 0){ // probably no difference, but maybe bind() returns values <-1 with windows
#endif
        exit_error("unable to bind socket");
    }else{
        printf("socket is bind\n");
    }
}
/*set socket to listening*/
void listen_socket(socket_type *socket){
    if(listen(*socket, 5) == -1){ //second parameter is the maximum amount of connection-requests that are queued up
        exit_error("unable to set socket to listening");
    }else{
        printf("socket is listening\n");
    }
}
/*accept connection-requests*/
void accept_socket(socket_type *socket, socket_type *new_socket){
    struct sockaddr_in client;
    // descriptor of connection, stored with two datastructures of the same size and similar structure: sockaddr_in and sock_addr;
    // but sockaddr_in is more comfortable to handle while using ip connections
    unsigned int len = sizeof(client);
    *new_socket = accept(*socket, (struct sockaddr*)&client, &len); // i dont understand why "&" in "&len"
#ifdef _WIN32
    if(*new_socket == INVALID_SOCKET){
#else
    if(*new_socket == -1){
#endif
        exit_error("unable to accept");
    }else{
        printf("connection request received and accepted\n");
    }
}

/*establish a connection*/
void connect_socket(socket_type *socket, char *serv_addr, unsigned short port) {
    struct sockaddr_in server;
    struct hostent *host_info; // host descriptor
    unsigned long addr;
    memset(&server, 0, sizeof(server));
    if ((addr = inet_addr(serv_addr)) != INADDR_NONE) {
        memcpy((char *) &server.sin_addr, &addr, sizeof(addr));
    }else{
        host_info = gethostbyname(serv_addr);
        if(NULL == host_info) {
            exit_error("unknown server");
        }
            memcpy((char *) &server.sin_addr, host_info->h_addr, host_info->h_length); //h_addr == h_addr_list[0])
    }
    server.sin_family = AF_INET; //IPv4
    server.sin_port = htons(port); //host to network-byte order
    if(connect(*socket, (struct sockaddr*)&server, sizeof(server)) < 0){
        exit_error("unable to establish connection with server");
    }else{
        printf("connection created successfully\n");
    }
}
/*send data via TCP*/
void TCP_send(socket_type *socket, char *data, size_t size){
#ifdef _WIN32
    if(send(*socket, data, size, 0) == SOCKET_ERROR){      //TODO: implement correction code;
#else
        if(send(*socket, data, size, 0) < 0){
#endif
        exit_error("error during sending the data occured");
    }
}
/*receive data via TCP*/
void TCP_receive(socket_type *socket, char *data, size_t size){
#ifdef _WIN32
    int len;
#else
    unsigned int len;
#endif
    len = recv(*socket, data, size, 0); // "0" means recv behaves like "read()"
#ifdef _WIN32
    if(len>0 || len != SOCKET_ERROR){
#else
        if(len>0 || len != -1){
#endif
        data[len] = '\0';
        }
    else{
        exit_error("error during receiving");
    }
}

/*run server*/
int init_server() {
    #ifdef _WIN32
    init_winsock();
    #endif

    socket_type sock1, sock2;
    char *buffer = (char *) malloc(BUF);
#ifdef _WIN32
    atexit(cleanup); //when process is closed, the function cleanup is called
#endif
    sock1 = create_socket();
    bind_socket(&sock1, INADDR_ANY, 15000); //TODO: what happens when port 15000 is already used?
    listen_socket(&sock1);
    accept_socket(&sock1, &sock2);
    do {
        //TODO implement: synchronize, send_db,
        printf("send message: ");
        fflush(stdin);
        fgets(buffer, BUF, stdin);
        buffer[strcspn(buffer,"\n")] = 0;
        TCP_send(&sock2, buffer, strlen(buffer));
        TCP_receive(&sock2, buffer, BUF - 1); // -1 so that it's still zero-terminated I guess
        printf("message received: %s\n", buffer);
    } while (strcmp(buffer, "quit\n") != 0);
    #ifdef _WIN32
    closesocket(sock2);
    closesocket(sock1);
    #else
    close(sock2);
    close(sock1);
    #endif
    return 0;
}

/*initialize client*/
int init_client(){
    #ifdef _WIN32
    init_winsock();
    #endif

    static socket_type sock_client;
    char *string_addr;
    char *buffer = (char *) malloc(BUF);

    sock_client = create_socket();
    #ifdef _WIN32
    atexit(cleanup); //when process is closed, the function cleanup is called
    #endif
    connect_socket(&sock_client, "127.0.0.1", 15000); // "127.0.0.1" is the loopback-address TODO: target-address should be choosable
    //TODO: Port 15000 is IANA registered. Use a Port between 49152-65535 (but also make sure it isn't already used by the host)
        buffer[0] = '\0';
        TCP_receive(&sock_client, buffer, BUF-1);
        if(strcmp(buffer, "0") == 0){ //server sends a 0 to accept
            server_answer(buffer); //TODO WARNING: probably a buffer overflow
            do{
                sprintf(buffer, "%i", sub_menu_network_client());
                buffer[1] = '\0';
                TCP_send(&sock_client, buffer, strlen(buffer));
                buffer[0] = '\0';
                TCP_receive(&sock_client, buffer, BUF-1);
                server_answer(buffer);
            }while(strcmp(buffer, "-1") != 0);
        }
        #ifdef _WIN32
    closesocket(sock_client);
    #else
    close(sock_client);
    #endif
    return 0;
}