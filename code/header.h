#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <pthread.h>
// #include <sqlite3.h>
// #include <algorithm>
#include "db.h"

Database db;

string getTimestamp()
{
    // Get the current time point
    auto currentTime = chrono::system_clock::now();

    // Convert the time point to a time_t object
    time_t currentTimeT = chrono::system_clock::to_time_t(currentTime);

    // Convert the time_t object to a string representation
    return ctime(&currentTimeT);
}
// thread function
void *handleClient(void *arg)
{
    // Cast the argument back to the original type
    int client_socket = *(int *)arg;
    cout << "\n->Client connected successfully" << endl;
                  
    // Welcome Message
    string welcome = "->Welcome to chat server\n->Login time: " + getTimestamp() + "\n->Type message: ";
    const char *message = welcome.c_str();

    // Send message
    send(client_socket, message, strlen(message), 0);

    while (true)
    {
        char recv[257] = {0};
        // read only 256 bytes
        ssize_t bytes_read = read(client_socket, recv, 256);
        if (bytes_read < 0)
        {
            cout << "Error Reading" << endl;
            bytes_read = -1;
        }
        string rcvMsg(recv);

        // Closing the socket
        if (rcvMsg.find("quit") != -1)
        {
            cout << "->Closing connection..." << endl;
            string msg = "->Connection closed.";
            send(client_socket, msg.c_str(), msg.length(), 0);
            close(client_socket);
            return NULL;
        }
        else
        {
            cout << "->Client: " << rcvMsg << endl;
            // inserting message to db
            db.insert(rcvMsg, getTimestamp());
            string funnyWords[] = {
                "banana", "noodle", "giggle", "silly", "whimsical", "hilarious",
                "guffaw", "wacky", "zany", "clown", "bozo", "goofball", "laughter",
                "chuckle", "joke", "funnybone", "gigglesnort", "yuk", "jolly", "droll"};
            int numWords = sizeof(funnyWords) / sizeof(funnyWords[0]);
            // getting 2 random words
            int id1 = rand() % numWords;
            int id2 = (id1 + 2) % numWords;
            rcvMsg.pop_back();
            // Append the two funny words
            string server_reply = "->Server: " + rcvMsg + " : " + funnyWords[id1] + " " + funnyWords[id2] + "\n->Type message: ";
            // sending modified message back to client
            send(client_socket, server_reply.c_str(), server_reply.length(), 0);
        }
    }

    return NULL;
}
