#ifndef MESSAGE_HPP
#define MESSAGE_HPP


#include "util.h"
#include <SFML/Network.hpp>


// CLassify different types of messages
// Each type represents a different multiple of 2.
enum class MsgType: sf::Uint8 {
    Undefined   = 0,
    Build       = 1,
    Move        = 2,
    Place       = 4,
    Rand        = 8,
    Register    = 16,
    Select      = 32
};

// A Packet operator, we input a packet (already defined) and message type into the Packet.
// The packet iself is not overwritten, but it's mtype is. we output the packet and mtype.
sf::Packet& operator << (sf::Packet& packet, const MsgType mtype);
sf::Packet& operator >> (sf::Packet& packet, MsgType& mtype);


// Message model
// defines a MsgType, id using Uint32, and established a char array of 216 bytes.
struct Msg {
    MsgType msgtype = MsgType::Undefined;
    sf::Uint32 id;
    char data[216];
};


// Register Message
// Stores username
struct MsgReg {
    MsgType msgtype = MsgType::Register;
    sf::Uint32 id = 0;
    std::string username;
    MsgReg() {}
    MsgReg(sf::Uint32 id_, std::string s): id(id_), username(s) {}
};

sf::Packet& operator << (sf::Packet& packet, const MsgReg msg);
sf::Packet& operator >> (sf::Packet& packet, MsgReg& msg);


// Selection Message
// Stores builderID (checks who is building)
struct MsgSelect {
    MsgType msgtype = MsgType::Select;
    sf::Uint32 id = 0;
    sf::Uint8 builderID = 0;
    MsgSelect() {}
    MsgSelect(sf::Uint32 id_, sf::Uint8 builderID_) : id(id_), builderID(builderID_) {}
};

sf::Packet& operator << (sf::Packet& packet, const MsgSelect msg);
sf::Packet& operator >> (sf::Packet& packet, MsgSelect& msg);


// Position Message
// Stores position data.
struct MsgPos {
    MsgType msgtype = MsgType::Undefined;
    sf::Uint32 id = 0;
    sf::Int8 x = 0;
    sf::Int8 y = 0;
    MsgPos() {}
    MsgPos(MsgType msgtype_, sf::Uint32 id_, sf::Vector2i pos_): msgtype(msgtype_), id(id_), x(pos_.x), y{pos_.y} {}
};

sf::Packet& operator << (sf::Packet& packet, const MsgPos msg);
sf::Packet& operator >> (sf::Packet& packet, MsgPos& msg);


// Random Message
// We don't really store anything here.
struct MsgRand {
    MsgType msgtype = MsgType::Rand;
    sf::Uint32 id = 0;
    sf::Uint32 r = 0;
    MsgRand() {}
    MsgRand(sf::Uint32 id_, sf::Uint32 r_): id(id_), r(r_) {} 
};

sf::Packet& operator << (sf::Packet& packet, const MsgRand msg);
sf::Packet& operator >> (sf::Packet& packet, MsgRand& msg);

// We read the Msg.
void readMsg(Message m, Msg& msg);

#endif