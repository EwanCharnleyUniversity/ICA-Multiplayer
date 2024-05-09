#ifndef ACCEPTER_HPP
#define ACCEPTER_HPP

#include "queue.h"
#include "list.h"
#include "util.h"
#include <SFML/Network.hpp>

// Accepts new clients and creates a REceiver for them.
class Accepter {
    private:
        Queue<Message>& m_queue;
        List<std::shared_ptr<sf::TcpSocket>>& m_socket;

    public:
        Accepter(Queue<Message>& q, List<std::shared_ptr<sf::TcpSocket>>& s);
        void operator()();
};


#endif
