#include <SFML/Graphics.hpp>
#include <array>
#include <cstring>
#include "tile.h"
#include "builder.h"
#include "queue.h"
#include "network.h"
#include "message.h"

#ifndef WORLD_HPP
#define WORLD_HPP

class game;

enum class WorldState {
    Place,
    Select,
    Move,
    Build
};


// World class, this handles everything in the game, and provides client input and output.
class world {
public:
    world(sf::RenderWindow& w, game *g);
    ~world();
    void update();
    void draw();
    void setup();
private:
    game *g;

    // Handles Client Messages
    Queue<Message> queue;
    sf::Uint32 id;
    Network network;

    // All the game related content. It's not based around  
    sf::RenderWindow& window;
    std::array<std::array<tile, 5>, 5> tiles{{{{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}, {{tile(), tile(), tile()}}}} ;
    std::vector<builder> builders;
    WorldState state = WorldState::Place;
    unsigned short buildersSoFar;
    unsigned short turn;
    std::size_t selectedBuilderIndex;

    // Actions
    void build(const sf::Vector2i& pos, bool toSend);
    void move(const sf::Vector2i& pos, bool toSend);
    void place(const sf::Vector2i& pos, bool toSend);
    void select(const sf::Vector2i& pos, bool toSend);

    // Conditions
    bool noBuilder(const sf::Vector2i& pos);
    bool nearSelectecBuilder(const sf::Vector2i& pos);
    bool noDome(const sf::Vector2i& pos);
    int levelDiff(const sf::Vector2i& pos);

    // Process
    void processBuild(const Msg& msg);
    void processMove(const Msg& msg);
    void processPlace(const Msg& msg);
    void processSelect(const Msg& msg);
};
#endif
