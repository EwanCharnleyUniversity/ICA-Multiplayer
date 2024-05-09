#include "queue.h"
#include "receiver.h"
#include "myserver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "myserver.h"



int main() {

    sf::UdpSocket udpSocket;
    std::string broading = "broadcast";
    udpSocket.send(broading.c_str(), broading.size() + 1, sf::IpAddress::Broadcast, PORT);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Creates a broadcaster and runServer
    // set the udp to be non blocking
    udpSocket.setBlocking(false);

    // attempt receive a message from the server
    sf::IpAddress serverAddress;
    unsigned short port;
    std::size_t byteReceived;
    char buffer[255];

    // We call to receive the server.
    sf::Socket::Status udpStatus = udpSocket.receive(buffer,sizeof(buffer),byteReceived,serverAddress,port);
    if (udpStatus != sf::Socket::Done) {
        serverAddress = "localhost";
        // Create a new server.

        myserver newServer;

        std::thread(&myserver::runServer, &newServer).detach();

        // Wait for server
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }



    // Normal socket stuff for generating a client.
    std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
    sf::Socket::Status status = socket->connect(serverAddress, PORT); // connect to server

    if (status != sf::Socket::Done) {
        return 1;
    }
    std::cout << "Connected\n";
    Queue<std::string> queue;

    // Create our receiver
    Receiver receive(socket, queue);

    // Starting our thread
    std::thread(&Receiver::recv_loop, &receive).detach();


    std::string s = "";
    while (1) {
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        // TODO send messages to the server

        if (sf::Socket::Done != socket->send(s.c_str(), s.size() + 1)) {
            std::cout << "User message was not sent";
            return 0;
        }

    }
    return 0;
}
