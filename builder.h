#include <SFML/Graphics.hpp>
#include "entity.h"

#ifndef BUILDER_HPP
#define BUILDER_HPP

class builder : public entity {
public:
    builder(unsigned short x, unsigned short y, unsigned short player);
    unsigned short player;
    void draw(sf::RenderWindow &window);
    void move(sf::Vector2i m);

    // Gives players an outline if its their turn to build
    //void buildmode(sf::Color col);

    sf::Vector2i pos;
private:
    //sf::Color outlineCol;
};

#endif
