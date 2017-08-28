/*
    @Author: Maik Basso <maik@maikbasso.com.br> 
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
 
using namespace std;
 
class TCPClient{
private:
    int sock;
    struct sockaddr_in server;
     
public:
    TCPClient();
    bool conn(string address , int port);
    bool sendData(string data);
    string receiveData(int size);
};
 
TCPClient::TCPClient(){
    this->sock = -1;
}

//connect to server
bool TCPClient::conn(string address , int port){
    //create socket if it is not already created
    if(this->sock == -1){
        //Create socket
        this->sock = socket(AF_INET, SOCK_STREAM, 0);
        if (this->sock == -1){
            perror("Could not create socket!");
        }
    }
    //setup address structure
    if(inet_addr(address.c_str()) == -1){
        struct hostent *he;
        struct in_addr **addr_list;
         
        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL){
            cout<<"Failed to resolve hostname\n";
	    return false;
        }
         
        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;
 
        for(int i = 0; addr_list[i] != NULL; i++){
            server.sin_addr = *addr_list[i];             
            break;
        }
    }
    else{
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }
     
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
     
    //Connect to remote server
    if (connect(this->sock, (struct sockaddr *)&server, sizeof(server)) < 0){
        return false;
    }
     
    cout << "Connected!";
    return true;
}
 
//Send data to the connected host
bool TCPClient::sendData(string data){
    if(send(this->sock , data.c_str() , strlen( data.c_str() ) , MSG_EOR) < 0){
        return false;
    }
    cout << "Send data: " << data.c_str() << endl;
    return true;
}
 
//Receive data from the connected host
string TCPClient::receiveData(int size=512){
    char buffer[size];
    string reply;

    if(recv(this->sock , buffer , sizeof(buffer) , 0) < 0){
        puts("receive failed");
    }

    reply = buffer;
    return reply;
}