#ifndef Structures_hpp
#define Structures_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Block {
    sf::RectangleShape shape;
    sf::Text num_text;
    int value;
};

class Array {
    std::vector<Block> array;
    int array_block_size = 100;
    bool change_made = false;

public:
    Array();
    void update(sf::RenderWindow &window);
    void addBlock(int value);
};

#endif