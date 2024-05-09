#ifndef MYSERVER_HPP
#define MYSERVER_HPP

// Server class, this just runs a server and a UDPloop.
class myserver { 
public:
    int runServer();

    void runUDPLoop();
};

#endif