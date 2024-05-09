#include "accepter.h"
#include "queue.h"
#include "list.h"
#include "receiver.h"
#include "util.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include "myserver.h"

// The purpose of this main function is to setup the server and all of its contents.
// Once run, it will generate a viable server that clients can interact and join with.
int main()
{
    myserver newServer;
    newServer.runServer();

    return 0;
}
