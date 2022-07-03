//
// Created by garri on 30.06.2022.
//

#include "network_functions.h"
#include <winsock2.h> //maybe an extra library needs to be included? je ne sais pas

//TODO:
//  client
//      initialize winsock (if win)
//      init socket
//      init connection
//      send-function
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
