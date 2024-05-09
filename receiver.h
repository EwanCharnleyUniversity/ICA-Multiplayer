#ifndef RECEIVER_HPP
#define RECEIVER_HPP

#include "queue.h"
#include "util.h"
#include <SFML/Network.hpp>

// Receiver Class, it receives signals for the Network and contains a TCP socket and queue.
class Receiver {
public:
    Receiver(std::shared_ptr<sf::TcpSocket> s, Queue<Message>& queue);
    void recv_loop();
private:
    std::shared_ptr<sf::TcpSocket> m_socket;
    Queue<Message> &m_queue;
};
#endif