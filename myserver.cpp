#include "accepter.h"
#include "queue.h"
#include "list.h"
#include "receiver.h"
#include "util.h"
#include "myserver.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>

// Runs the server continuously for all clients.
int myserver::runServer() {
    Queue<Message> queue;
    List<std::shared_ptr<sf::TcpSocket>> sockets;

    // TODO launch an accepter thread.
    Accepter accept(queue, sockets);
    std::thread launcher(accept);

    launcher.detach();

    std::thread UdpLooper(&myserver::runUDPLoop, this);
    UdpLooper.detach();

    while(1)
    {
        Message message = queue.pop();
        // std::cout << "Main read: \"" << s << "\"\n";
        // TODO send to all in sockets. Be careful to synchronise.
        // 1. Make a lambda that sends s to one socket

        auto send_to_one = [&message](std::shared_ptr<sf::TcpSocket> socket){
            // check for errors
            if (sf::Socket::Done != socket->send(message.first)) {
                std::cout << "User message was not sent";
                return;
            }
        };
        
        // call for each from sockets with the parameter send_to_one
        sockets.for_each(send_to_one);
    }
}

// Runs a constant UDP Loop to ensure connection.
void myserver::runUDPLoop() {

    sf::UdpSocket udpsocket;
    udpsocket.bind(PORT + 1);


    char buffer[1024];
    std::size_t received = 0;
    sf::IpAddress sender;
    unsigned short senderport;

    // Run loop endlessly.
    while (true) {
        // Sets up UDP receiver and checks if we have no problems receiving a client.
        sf::Socket::Status status = udpsocket.receive(buffer, sizeof(buffer), received, sender, senderport);

        if (status != sf::Socket::Done) {
            std::cout << "Problem receiving client";
            return;
        }

        // Generates an entrance message.
        std::cout << sender.toString() << " said: " << buffer << std::endl;

        std::string message = "Welcome " + sender.toString();
        status = udpsocket.send(message.c_str(), message.size() + 1, sender, senderport);

        // Detects if message was not sent by the client to the server.
        if (status != sf::Socket::Done) {
            std::cout << "Message was not sent";
            return;
        }
    }
}