//
// Created by garri on 30.06.2022.
//

//TODO: for windows first
//  client
//      initialize winsock (if win)
//      init socket
//      init connection
//      send-function
//          copyrequest
//          synchronize
//          deleterequest
//          entryrequest
//          changerequest
//      close socket
//      clear winsock (if win)
//  server
//      initialize winsock (if win)
//      init socket
//      bind
//      listen
//      accept
//      recv
//          synchronize
//          deleterequest
//          entryrequest
//          changerequest
//      close socket
//      clear winsock (if win)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h> //seems to be required for some Windows-Versions
#include <io.h>

#define BUF 1024

/*evaluates error, returns error message*/
int exit_error(char *error_message){
    fprintf(stderr, "%s: %d\n", error_message, WSAGetLastError());
    return -1;
}

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
/*create a tcp, IPv4 Socket*/
SOCKET create_socket(){
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); //AF_INET corresponds to IPv4, 0 is the default transmission protocol for the socket type
    if(sock <0){
        exit_error("socket creation failed");
    }else{
        printf("socket is created\n");
    }
    return sock;
}
/*bind socket to serveraddress*/
void bind_socket(SOCKET *sock, unsigned long address, unsigned short port){
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server)); //sets memory of server to zeros
    server.sin_family = AF_INET; //specifies type of server to IPv4
    server.sin_addr.s_addr = htonl(address); //address given (and expected to be given) in host-byte-order is transformed to network-byte-order with "htonl()"
    server.sin_port = htons(port); //host to network-byte order conversion
    if(bind(*sock, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR){
        exit_error("unable to bind socket");
    }else{
        printf("socket is bind\n");
    }
}
/*set socket to listening*/
void listen_socket(SOCKET *socket){
    if(listen(*socket, 5) == -1){ //second parameter is the maximum amount of connection-requests that are queued up
        exit_error("unable to set socket to listening");
    }else{
        printf("socket is listening\n");
    }
}
/*accept connection-requests*/
void accept_socket(SOCKET *socket, SOCKET *new_socket){
    struct sockaddr_in client;
    // descriptor of connection, stored with two datastructures of the same size and similar structure: sockaddr_in and sock_addr;
    // but sockaddr_in is more comfortable to handle while using ip connections
    unsigned int len = sizeof(client);
    *new_socket = accept(*socket, (struct sockaddr*)&client, &len); // i dont understand why "&" in "&len"
    if(*new_socket == INVALID_SOCKET){
        exit_error("unable to accept");
    }else{
        printf("connection request received and accepted\n");
    }
}
/*establish a connection*/
void connect_socket(SOCKET *socket, char *serv_addr, unsigned short port) {
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
void TCP_send(SOCKET *socket, char *data, size_t size){
    if(send(*socket, data, size, 0) == SOCKET_ERROR){             //TODO: implement correction code;
                                                                //      what happens when datas are sent in multiple packages?
        exit_error("error during sending the data occured");
    }
}
/*receive data via TCP*/
void TCP_receive(SOCKET *socket, char *data, size_t size){
    int len = recv(*socket, data, size, 0); // "0" means recv behaves like "read()"
    if(len>0 || len != SOCKET_ERROR){
        data[len] = '\0';
        }
    else{
        exit_error("error during receiving");
    }
}
/*"free" Winsock*/
void cleanup(void){
    WSACleanup();
    printf("Winsock cleanup finished\n");
}

//TODO debug: first receive/send output is weird

/*initialize server*/
int init_server() {
    init_winsock();

    static SOCKET sock1, sock2;
    char *buffer = (char *) malloc(BUF);
    
    atexit(cleanup); //when process is closed, the function cleanup is called

    sock1 = create_socket();
    bind_socket(&sock1, INADDR_ANY, 15000); //TODO: port assignment should be somehow automated
    listen_socket(&sock1);
    accept_socket(&sock1, &sock2);
    do {
        printf("send message: ");
        fgets(buffer, BUF, stdin);
        TCP_send(&sock2, buffer, strlen(buffer));
        TCP_receive(&sock2, buffer, BUF - 1); // -1 so that its still zero-terminated I guess
        printf("message received: %s\n", buffer);
    } while (strcmp(buffer, "quit\n") != 0);
    closesocket(sock2);
    closesocket(sock1);
    return 0;
}

/*initialize client*/
int init_client(){
    init_winsock();

    static SOCKET sock_client;
    char *buffer = (char *) malloc(BUF);

    sock_client = create_socket();
    atexit(cleanup); //when process is closed, the function cleanup is called
    connect_socket(&sock_client, "127.0.0.1", 15000); // "127.0.0.1" is the loopback-address TODO: target-address should be scanned
    do{
        buffer[0] = '\0'; //TODO: understand why
        TCP_receive(&sock_client, buffer, BUF-1);
        printf("message received: %s\n", buffer);
        printf("send message: ");
        fgets(buffer,BUF,stdin);
        TCP_send(&sock_client, buffer, strlen(buffer));
        }while(strcmp(buffer, "quit\n") != 0);
    closesocket(sock_client);
    return 0;
}