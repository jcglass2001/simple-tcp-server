#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MSG_BUFFER 1024

int main(int argc, char* argv[])
{
    if(argc != 2){
        std::cerr << "[ERROR] Program takes one argument: Port Number" << std::endl;
        exit(-1);
    }

    int port = atoi(argv[1]);
    char buffer[MSG_BUFFER] = {0};

    // 1. Create the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "[ERROR] Failed to create socket..." << std::endl;
        exit(-1);
    }
    std::cout << "[INFO] Socket created sucessfully...\n";

    // 2. Define address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket to local machine
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "[ERROR] Failed to bind socket to local machine..." << std::endl;
        close(serverSocket);
        exit(-1);
    }
    std::cout << "[INFO] Bind successful...\n";

    // 4. Listen for connections
    if (listen(serverSocket, 5) < 0) {
        std::cerr << "[ERROR] Failed to receive connections..." << std::endl;
        close(serverSocket);
        exit(-1);
    }
    std::cout << "[INFO] Connection request received...\n";

    // 5. Accept client connections
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "[ERROR] Failed to connect to client..." << std::endl;
        close(serverSocket);
        exit(-1);
    }
    std::cout << "[INFO] Client connected...\n";

    // 6. Receive data from client
    while(true)
    {
        memset(buffer,0,MSG_BUFFER);
        if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0) {
            std::cerr << "[ERROR] Failed to receive data from client..." << std::endl;
            close(clientSocket);
            close(serverSocket);
            exit(-1);
        } else if (strcmp(buffer,"exit") == 0) {
            std::cout << "[INFO] Client requesed to end connection...\n";
            break;
        }
        std::cout << "(Client): " << buffer << std::endl;
    }

    // 7. Close socket
    close(serverSocket);
}