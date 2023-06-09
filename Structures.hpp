#ifndef Structures_hpp
#define Structures_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

struct Block {
    sf::RectangleShape shape;
    sf::Text num_text;
    sf::Text index;
};

enum SortType {
    NONE, BUBBLE, INSERTION, SELECTION, MERGE
};

class Array {
    std::vector<Block> array;
    sf::RenderWindow &window;
    sf::Font &font;
    int block_size = 75;
    int value_char_size = 36;
    bool change_made = false;
    sf::RectangleShape block_add;
    sf::RectangleShape block_remove;

    int value_edit_index = -1;
    SortType sort_type = NONE;
    void selectionSort();

public:
    Array(sf::RenderWindow &window, sf::Font &font);
    void update();
    void addBlock(int value);
    void removeBlock();
    void setBlockSize(int size);
    void handleClick(float mouseX, float mouseY);
    void handleKeypress(char key);
    void sort(SortType type);
};

#endif