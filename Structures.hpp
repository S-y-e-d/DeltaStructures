#ifndef Structures_hpp
#define Structures_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct Block {
    sf::RectangleShape shape;
    sf::Text num_text;
    sf::Text index;
    int value;
};

class Array {
    std::vector<Block> array;
    sf::RenderWindow &window;
    sf::Font &font;
    int block_size = 75;
    bool change_made = false;
    sf::RectangleShape block_add;
    sf::RectangleShape block_remove;

public:
    Array(sf::RenderWindow &window, sf::Font &font);
    void update();
    void addBlock(int value);
    void removeBlock();
    void setBlockSize(int size);
    void handleClick(float mouseX, float mouseY);
};

#endif