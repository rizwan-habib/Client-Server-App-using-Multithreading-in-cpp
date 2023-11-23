#include "header.h"
// #include "db.h"
int main()
{
    // create db
    db.create();
    // create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cout << "Could not create socket." << std::endl;
        return -1;
    }
    // we can reuse any port which is already in use
    int reuse = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("Failed to set socket options");
        return -1;
    }

    // specify the server address and port
    struct sockaddr_in server_address;
    int port = 12345; // setting the port number
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); // change IP address here
    server_address.sin_port = htons(port);

    // bind the socket to the server address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cout << "Failed to bind socket" << std::endl;
        return -1;
    }

    pthread_t threads[5];
    int counter = 0;
    for (int i = 0; i < 5; i++)
    {
        // listen for incoming connections
        if (listen(server_socket, 5) < 0)
        {
            std::cout << "Failed to listen" << std::endl;
            return -1;
        }

        // print the server status
        cout << "->Number of connected clients: " << counter++ << endl;
        cout << "->Server listening to port: " << port << " with ip-address 127.0.0.1" << endl;
        cout << "->Server time: " << getTimestamp()<<endl;
        
        // accept incoming connections
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t addrlen = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen);
        if (client_socket < 0)
        {
            std::cout << "Failed to accept connection" << std::endl;
            return -1;
        }
        
        pthread_create(&threads[i], NULL, handleClient, (void *)&client_socket);
        
    }

    // wait for all client threads to finish
    for (int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }
    db.closeConnection();
    close(server_socket);

    return 0;
}
