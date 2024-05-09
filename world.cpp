#include "world.h"
#include "game.h"

world::world(sf::RenderWindow& w, game *g_):
    window(w),
    g(g_),
    queue(),
    network(queue)
{
    srand(time(0));
    window.setFramerateLimit(60);
    setup();
    buildersSoFar = 0;
}

void world::setup()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tiles[i][j].pos = sf::Vector2i(i, j);
            tiles[i][j].level = 0;
        }
    }
    turn = 0;
    state = WorldState::Place;
}

world::~world()
{
}

bool world::noBuilder(const sf::Vector2i& pos)
{
    for (builder& b: builders)
    {
        if (pos == b.pos)
        {
            return false;
        }
    }
    return true;
}

bool world::nearSelectecBuilder(const sf::Vector2i& pos)
{
    sf::Vector2i diff = pos - builders[selectedBuilderIndex].pos;
    return (diff.x >= -1 && diff.x <= 1 &&
            diff.y >= -1 && diff.y <= 1 &&
            !(diff.x == 0 && diff.y == 0));
}

bool world::noDome(const sf::Vector2i& pos)
{
    return tiles[pos.x][pos.y].level < 4;
}

int world::levelDiff(const sf::Vector2i& pos)
{
    sf::Vector2i bpos = builders[selectedBuilderIndex].pos;
    return tiles[pos.x][pos.y].level - tiles[bpos.x][bpos.y].level;
}

void world::build(const sf::Vector2i& pos, bool toSend)
{
    // Check if no builder is there.
    // Check if near selected builder.
    // Check if not on a dome.
    if (noBuilder(pos) && nearSelectecBuilder(pos) && noDome(pos))
    {
        // Determines whether or not the message has to be sent to the server.
        if (toSend) {
            network.send(MsgPos(MsgType::Build, id, pos));
        }

        tiles[pos.x][pos.y].build();
        state = WorldState::Select;
        turn++;
    }
}

void world::move(const sf::Vector2i& pos, bool toSend)
{
    // If on a builder and this builder is of the same colour, we should select this builder and
    // return.
    if (!noBuilder(pos)) // Builder was clicked
    {
        for (size_t i = 0; i < builders.size(); i++)
        {
            if (builders[i].pos == pos && turn % 2 == builders[i].player)
            {
                selectedBuilderIndex = i;
                if (toSend) {
                    network.send(MsgSelect(id, i));
                }
                return;
            }
        }
    }
    // Check if no builder is there.
    // Check not on top of dome.
    // Check near selected builder.
    // Check builder does not climb more than one floor.
    if (noBuilder(pos) && nearSelectecBuilder(pos) && noDome(pos) && levelDiff(pos) <= 1)
    {
        sf::Vector2i prevPos = builders[selectedBuilderIndex].pos;
        if (tiles[prevPos.x][prevPos.y].level == 2 && tiles[pos.x][pos.y].level == 3)
        {
            // TODO
            // Instead of checking the turn, we want to check if the LOCAL player
            // has won the game. Could be done easily with a class variable to note who the
            // local player is.
            if (turn % 2 == 0)
            {
                g->state = gamestate::victory;
            }
            else
            {
                g->state = gamestate::defeat;
            }
        }
        if (toSend) {
            network.send(MsgPos(MsgType::Move, id, pos));
        }
        sf::Vector2i m = pos - builders[selectedBuilderIndex].pos;
        builders[selectedBuilderIndex].move(m);
        state = WorldState::Build;
    }
}

void world::place(const sf::Vector2i& pos, bool toSend)
{
    // Check not on top of other builder.
    if (noBuilder(pos))
    {
        if (toSend) {
            network.send(MsgPos(MsgType::Place, id, pos));
        }
        builders.push_back(builder(pos.x, pos.y, buildersSoFar/2));
        buildersSoFar++;
        if (buildersSoFar == 4)
        {
            state = WorldState::Select;
        }
    }
}

void world::select(const sf::Vector2i& pos, bool toSend)
{
    // Check correct player.
    // Check builder has a valid move. TODO
    for (size_t i = 0; i < builders.size(); i++)
    {
        if (builders[i].pos == pos)
        {
            if (turn % 2 == builders[i].player)
            {
                if (toSend) {
                    network.send(MsgSelect(id, i));
                }
                selectedBuilderIndex = i;
                state = WorldState::Move;
                return;
            }
        }
    }
}

// Process Messages
// When a player makes an action, we need to ensure it gets pased to the Network and shared.
void world::processBuild(const Msg& msg) {
    // We retrieve position data and send it to the Build function.
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x,m_pos.y);
    build(pos, false);
}

void world::processMove(const Msg& msg) {
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x,m_pos.y);
    move(pos,false);
}

void world::processPlace(const Msg& msg) {
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x,m_pos.y);
    move(pos,false);
}

void world::processSelect(const Msg& msg) {
    MsgPos m_pos;
    std::memcpy(&m_pos, &msg, sizeof(MsgPos));
    sf::Vector2i pos(m_pos.x,m_pos.y);
    move(pos,false);
}


// Update function, we perform both Client and Server interactions.
void world::update()
{
    // Client Side Interactions
    static bool clicked = false;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clicked)
    {
        clicked = true;
        sf::Vector2i pos = sf::Mouse::getPosition(window) / TILE_SIZE;
        switch (state) {
            case WorldState::Build:
                std::cout << "Build\n";
                build(pos,true);
                break;
            case WorldState::Move:
                std::cout << "Move\n";
                move(pos,true);
                break;
            case WorldState::Place:
                std::cout << "Place\n";
                place(pos,true);
                break;
            case WorldState::Select:
                std::cout << "Select\n";
                select(pos,true);
                break;
            default:
                break;
        }
    }
    else
    {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            clicked = false;
        }
    }

    // Server side interactions
    // this handles incoming Messages and does not output them from the client update function.
    // We are updating it.
    Message m;
    queue.pop(m);

    // If the packet is empty (there is nothing there), we do not need to process anything.
    if (m.first.endOfPacket()) {
        return;
    }

    // We quickly create a Msg and figure out its contents.
    Msg msg;
    readMsg(m, msg);

    // We determine the output.
    switch(msg.msgtype) {
        case MsgType::Register:
            std::cout << "MsgType::Register\n";
            // Process the message.
            break;
        case MsgType::Select:
            std::cout << "MsgType::Select\n";
            processSelect(msg);
            break;
        case MsgType::Move:
            std::cout << "MsgType::Move\n";
            processMove(msg);
            break;
        case MsgType::Place:
            std::cout << "MsgType::Place\n";
            processPlace(msg);
            break;
        case MsgType::Build:
            std::cout << "MsgType::Build\n";
            processBuild(msg);
            break;
        case MsgType::Rand:
            std::cout << "MsgType::Rand\n";
            break;
    }

}

void world::draw()
{
    window.clear();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            tiles[i][j].draw(window);
        }
    }
    for (builder& b: builders)
    {
        b.draw(window);
    }
}

