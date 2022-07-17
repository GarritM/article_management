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
#include "file_functions.h"

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
/*set sock to listening*/
void listen_socket(socket_type *sock){
    if(listen(*sock, 5) == -1){ //second parameter is the maximum amount of connection-requests that are queued up
        exit_error("unable to set sock to listening");
    }else{
        printf("sock is listening\n");
    }
}
/*accept connection-requests*/
void accept_socket(socket_type *sock, socket_type *new_socket){
    struct sockaddr_in client;
    // descriptor of connection, stored with two datastructures of the same size and similar structure: sockaddr_in and sock_addr;
    // but sockaddr_in is more comfortable to handle while using ip connections
    int len = sizeof(client);
    *new_socket = accept(*sock, (struct sockaddr*)&client, &len);
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
void TCP_send(socket_type *sock, char *data, size_t size){
#ifdef _WIN32
    if(send(*sock, data, size, 0) == SOCKET_ERROR){      //TODO: implement correction code;
#else
        if(send(*sock, data, size, 0) < 0){
#endif
        exit_error("error during sending the data occured");
    }
}

/*receive data via TCP*/
void TCP_receive(socket_type *sock, char *data, size_t size){
#ifdef _WIN32
    int len;
#else
    unsigned int len;
#endif
    len = recv(*sock, data, size, 0); // last argument "0" means recv behaves like "read()"
#ifdef _WIN32
    if(len>0 || len != SOCKET_ERROR){
#else
        if(len>-1){
#endif
        data[len] = '\0';
        }
    else{
        exit_error("error during receiving");
    }
}

int client_process();
int server_process();
int sent_db_via_tcp();
void server_answer();
int recv_db_via_tcp();

/*run server*/
int init_server(database_type *database) {
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

    int cont_server = 1;
    while (cont_server == 1) {
        accept_socket(&sock1, &sock2);
        server_process(&sock2, buffer, database);
#ifdef _WIN32
        closesocket(sock2);
#else
        close(sock2);
#endif
        printf("do u want to answer another client-request (y/n): ");
        cont_server = ask_for_answer();
    }
    closesocket(sock1);
#ifndef _WIN32
    close(sock1);
#endif
    return 0;
}

/*initialize client*/
int init_client(database_type *database){
    #ifdef _WIN32
    init_winsock();
    #endif

    static socket_type sock_client;
    char *string_addr;
    char *buffer = (char *) malloc(BUF);

    /*establishing connection*/
    sock_client = create_socket();
    #ifdef _WIN32
    atexit(cleanup); //when process is closed, the function cleanup is called
    #endif
    connect_socket(&sock_client, "127.0.0.1", 15000); // "127.0.0.1" is the loopback-address TODO: target-address should be choosable
    //TODO: Port 15000 is IANA registered. Use a Port between 49152-65535 (but also make sure it isn't already used by the host)

    /*when connection is established*/
    client_process(&sock_client, buffer, database);

    /*when disconnect*/
        #ifdef _WIN32
    closesocket(sock_client);
    #else
    close(sock_client);
    #endif
    return 0;
}

int server_process(socket_type *sock2,char* buffer, database_type *database){
    while (1) {
        //TODO: implement way to close server?
        memset(buffer, 0, BUF);
        TCP_receive(sock2, buffer, BUF - 1); // -1 because index_size needed here (but why not implemented directly into the function?
        if (strcmp(buffer, "0") == 0) {
            printf("client is disconnecting\n");
            break;
        } else {
            if(strcmp(buffer, "1") == 0){
                printf("client want to upload db\n");
                TCP_send(sock2, "1", BUF-1);
                recv_db_via_tcp(sock2, database, BUF-1);
                return 1;
            }else if(strcmp(buffer, "2") == 0){
                //TODO: upload recv db;
                printf("db received\n");
                return 2;
            }
        }
    }
}
int client_process(socket_type *sock_client, char* buffer, database_type *database){
    char data[BUF];
    while(1) {
        int chosen_opt_net_sub = sub_menu_network_client();
        if (chosen_opt_net_sub == 0) {

        /*request to end connection*/
            TCP_send(sock_client, "0", BUF - 1);
            break;
        }
        /*request to upload database*/
        if (chosen_opt_net_sub == 1) {
            TCP_send(sock_client, "1", BUF - 1);
            printf("request to upload database\n");
            TCP_receive(sock_client, data, BUF - 1);//check if server wants to receive a database (1 means yes) //TODO: check serverside
            if (strcmp(data, "1") == 0) {
                printf("Server is ready to receive the database\n");
                sent_db_via_tcp(sock_client, database, BUF - 1);
            } else {
            }
        /*request to download database*/
        }else if(chosen_opt_net_sub == 2){

        }
    }
    return 0;
}
int sub_menu_network_client(){
    int option_number = -1;
    do{
        printf("Choose one of the following options:\n"
               "[1] upload database to server\n"
               "[2] request database from server\n"
               "[0] end connection\n");
        ask_for_number(&option_number);
        if(option_number == 1){
            return 1;
        }else if(option_number == 2){
            return 2;
        }else if(option_number == 0){
            return 0;
        }
    }while (option_number < 0 || option_number > 2);
}
void server_answer(char* recv_buffer){
    if(strcmp(recv_buffer, "0") == 0){
        printf("request accepted\n");
    }else if(strcmp(recv_buffer, "-1") == 0){
        printf("disconnect from server\n");
    }else{
        printf("something went wrong here");
    }
}


int sent_db_via_tcp(socket_type *sock, database_type *db, size_t size){
    if(sizeof(database_information_type)+5 > (size)){ //5 extra bytes needed for "encode_database_info"
        return -1;//TODO: communicate server to intterupt process
    }else{
        char data[BUF];
        encode_database_info(data, db->file_information);
        TCP_send(sock, data, strlen(data));
        for(int i = 0; i < db->file_information->size; i++){
            memset(data, 0, BUF);
            TCP_receive(sock, data, BUF-1);
            if (strcmp(data, "received") == 0) {
                memset(data, 0, strlen(data));
                encode_article_data(data, db->article_array[i]);
                data[strcspn(data, "\n")] = '\0';
                TCP_send(sock, data, strlen(data));
                printf("sent article no.: %i\n", i);
            }else{
                break;
            }
        }
        TCP_send(sock, "end", size);
        return 0;
    }
}
int recv_db_via_tcp(socket_type *sock, database_type *database, size_t size) {
    char data[BUF];
    TCP_receive(sock, data, BUF - 1);
    if (decode_file_info(data, database) == 0) {
        database->article_array = create_article_array(database->file_information->size);
        for (int i = 0; i < database->file_information->size; i++) {
            memset(data, 0,BUF-1);
            TCP_send(sock,"received", BUF-1);
            memset(data, 0, BUF-1);
            TCP_receive(sock, data, BUF-1);
            decode_article_data(data, &database->article_array[i]);
            printf("received and decoded article %i\n", i);
        }
        TCP_receive(sock, data, 3);
        if(strcmp(data, "end")== 0){
            return 0;
        }else{
            printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"
                   "sth. went wrong while receiving the database\n"
                   "the database might be corrupted\n"
                   "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            return -1;
        }
    } else {
        printf("encoding of file_information failed\n");
        return -1;
    }
}
