#include "network.h"
#include "receiver.h"
#include "myserver.h"
#include "util.h"
#include <iostream>


// The Network, it is the fundamental architecture
Network::Network(Queue<Message>& q): queue(q) {
    // UDP message to find server
    sf::UdpSocket udpSocket;
    sf::Packet packet;
    sf::Socket::Status status;

    // Asks wether there is a server
    packet << "Are you a server?";
    unsigned short port = UPORT;
    std::cout << "Sending UDP message discovery: " << port << std::endl;

    // Checks and quits the entire program if it finds no Network to connect to.
    status = udpSocket.send(packet, sf::IpAddress::Broadcast, UPORT);
    if (status != sf::Socket::Done) {
        std::cout << "There has been a critical error in sending a request to the Network";
        exit(0);
    }
    
    // Send out a UDP Message.
    std::cout << "Sent UDP message\n";

    // Here we receive a UDP message from the server.
    packet.clear();
    sf::IpAddress address;
    udpSocket.setBlocking(false);
    bool serverResponded = false;

    // Start a clock and loop through a while statement for 2 seconds. During the while loop the Network will look for a server to connect to.
    // If the server has not responded after those two seconds, we create a new server. This means players loading in the game can play it without crashing.
    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 2.0f) {
        udpSocket.receive(packet, address, port);
        if (!packet.endOfPacket()) { // empty socket means we did not receive an answer
            serverResponded = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if (!serverResponded) {
        myserver server;
        std::thread(&myserver::runServer, &server).detach();
        address = sf::IpAddress::getLocalAddress();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    std::cout << "Recieved UDP message back from server\n";

    // Check if the client has a connection to the network, if not we close the game.
    socket = std::make_shared<sf::TcpSocket>();
    status = socket->connect(address,PORT);
    if (status != sf::Socket::Done) 
    {
        std::cout << "There has been a critical error in connecting with the network.";
        exit(0);
    }
    
    // CLient succeed message
    std::cout << "Client::main Connected\n";

    // Detatch the thread.
    std::shared_ptr<Receiver> r = std::make_shared<Receiver>(socket, queue);
    std::thread(&Receiver::recv_loop, r).detach();
}


Network::~Network() {}


// Registering and Sending
void Network::reg(const sf::Uint32 id) {
    // Register with the server first
    sf::Packet m_reg_packet;
    m_reg_packet << MsgReg(id, "Batman");

    // Checks registering, crashes if it is not done.
    sf::Socket::Status status = socket->send(m_reg_packet);
    if (status != sf::Socket::Done) {
        std::cout << "Failed to register the user.";
        exit(0);
    }

    std::cout << "Send register\n";
}

// All of the following are simply repeats of the same send function, they create a Packet and output a message. If the message is not sent, the game will close.
// The only difference is what that message outputs.

void Network::send(const MsgReg& msg) {
    sf::Packet packet;
    packet << msg;

    sf::Socket::Status status = socket->send(packet);
    if (status != sf::Socket::Done) {
        std::cout << "Failed to send message.";
        exit(0);
    }
    std::cout << "Sending TCP Message.";
}

void Network::send(const MsgSelect& msg) {
    sf::Packet packet;
    packet << msg;

    sf::Socket::Status status = socket->send(packet);
    if (status != sf::Socket::Done) {
        std::cout << "Failed to send message.";
        exit(0);
    }
    std::cout << "Sending TCP Message.";
}

void Network::send(const MsgPos& msg) {
    sf::Packet packet;
    packet << msg;

    sf::Socket::Status status = socket->send(packet);
    if (status != sf::Socket::Done) {
        std::cout << "Failed to send message.";
        exit(0);
    }
    std::cout << "Sending TCP Message.";
}

void Network::send(const MsgRand& msg) {
    sf::Packet packet;
    packet << msg;

    sf::Socket::Status status = socket->send(packet);
    if (status != sf::Socket::Done) {
        std::cout << "Failed to send message.";
        exit(0);
    }
    std::cout << "Sending TCP Message.";
}