#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "message.h"
#include "queue.h"
#include "util.h"
#include <thread>
#include <SFML/Network.hpp>

// Network class, it is the messanger between client and server. It parses what messages the user sends out and recieves, such as game updates or chat messages.
// Also contains a queue and socket which keeps connection and queue for synchonisation.
class Network {
public:
    Network(Queue<Message>& q);
    ~Network();

    // Registering and send packets. reg finds the ID of a received message type, send provides data on what the User is sending out.
    void reg(const sf::Uint32 id);
    void send(const MsgReg& msg);
    void send(const MsgSelect& msg);
    void send(const MsgPos& msg);
    void send(const MsgRand& msg);
private:
    Queue<Message>& queue;
    std::shared_ptr<sf::TcpSocket> socket;
};
#endif