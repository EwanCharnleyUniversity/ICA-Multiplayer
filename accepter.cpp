#include "accepter.h"
#include "receiver.h"
#include "util.h"
#include <iostream>
#include <sstream>
#include <thread>

Accepter::Accepter(Queue<Message>& q, List<std::shared_ptr<sf::TcpSocket>>& s):
    m_queue(q),
    m_socket(s)
{}

void Accepter::operator()()
{
    sf::TcpListener listener;

    // We check if the TCP Listener cannot find an active port to connect to.
    sf::Socket::Status status = listener.listen(PORT);
    if (status != sf::Socket::Done) {
        std::cout << "Listener cannot find a port.\n";
        return;
    }
    

    std::cout << "Bound to port\n";
    while (true)
    {
        std::shared_ptr<sf::TcpSocket> socket = std::make_shared<sf::TcpSocket>();
        // TODO accept a connection on socket

        // We accept the connection.
        status = listener.accept(*socket);

        if (status != sf::Socket::Done) {
            std::cout << "Could not accept the connection.\n";
            return;
        }

        m_socket.push(socket);
        std::stringstream ss;
        ss << "Accepted a connection from: "
            << socket->getRemoteAddress()
            << ":"
            << socket->getRemotePort()
            << std::endl;
        std::cout << ss.str();
        std::shared_ptr<Receiver> receiver = std::make_shared<Receiver>(socket, m_queue);

        // Launch a thread to receive data from the Receiver.
        std::thread launch(&Receiver::recv_loop, receiver);
        launch.detach();
    }
}

