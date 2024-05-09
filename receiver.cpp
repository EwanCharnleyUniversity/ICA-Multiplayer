#include "receiver.h"
#include "util.h"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>

Receiver::Receiver(std::shared_ptr<sf::TcpSocket> s,
        Queue<Message> &q):
    m_socket(s),
    m_queue(q)
{
}

void Receiver::recv_loop() {
    // Creates a Character buffer array. A single char stores one character and occupies only one byte of memory.
    // a value of 256 is used because it's considered the perfect amount - not too much, not too litte.
    // This means we are taking up 256 bytes of memory for the Receiver's buffer.
    char buffer[256];
    sf::Packet packet;

    // Curly Brackets are not for a conditional statement, they split up the code for easier understanding.
    {
        std::stringstream ss;
        ss << m_socket->getRemoteAddress() << ":" << m_socket->getRemotePort() << std::endl;
        std::cout << ss.str();
    }
    while(1)    // While loop has no condition, this will always run.
    {
        // Sets up a memset using the char buffer.
        // Memset sets N bytes of S to C. We are setting 256 bytes of 0.
        std::memset(buffer, 0, 256);
        std::size_t received;

        // Checks status and stops the function if a status is not completed.
        sf::Socket::Status status = m_socket->receive(packet);
        if (status != sf::Socket::Done) {
            std::cout << "There has been an error in recieving a client.";
            return;
        }

        // output Buffer and Received 
        std::stringstream ss;
        ss << "Received: \"" << buffer << "\", " << received << " bytes." << std::endl;
        std::cout << ss.str();

        // Queue of Messages
        m_queue.push(std::make_pair(packet, m_socket));
    }
}

