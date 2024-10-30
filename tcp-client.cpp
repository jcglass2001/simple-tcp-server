#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MSG_BUFFER 1024

int main()
{
    // 1. Create the socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "[ERROR] Failed to create socket..." << std::endl;
        exit(-1);
    }
    std::cout << "[INFO] Socket created successfully...\n";
    char buffer[MSG_BUFFER];

    // 2. Define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 3. Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "[ERROR] Failed to connect to server..." << std::endl;
        close(clientSocket);
        exit(-1);
    }
    std::cout << "[INFO] Successfully connected to server...\n";

    // 4. Send data to server
    while(true) 
    {
        std::string msg_input;
        std::cout << "> ";
        getline(std::cin,msg_input);
        memset(buffer,0,MSG_BUFFER);
        strcpy(buffer,msg_input.c_str());

        if (msg_input == "exit") {
            send(clientSocket, buffer, strlen(buffer), 0);
            break;
        }

        send(clientSocket, buffer, strlen(buffer), 0);
        memset(buffer, 0, MSG_BUFFER);

        std::cout << "[INFO] Waiting for response form server...\n";
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytesReceived < 0) {
            std::cerr << "[ERROR] Failed to receive message..." << std::endl;
            break;
        }
        std::cout << "(Server): " << buffer << std::endl;
    }
    // 5. Close the socket
    close(clientSocket);
    
    return 0;
}