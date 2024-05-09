#include <SFML/Network.hpp>
#include <functional>
#include <memory>

// We define a Port and UPort.
#define PORT 4300
#define UPORT 4400

// We setup a typedef which is a pair packaging a Packet and a Shared Pointer TcpSocket. This creates a "Message" which the network and server will use to share data.
typedef std::pair<sf::Packet, std::shared_ptr<sf::TcpSocket>> Message;

