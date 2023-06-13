#ifndef Structures_hpp
#define Structures_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <variant>
#include <math.h>

struct Block {
    sf::RectangleShape shape;
    sf::Text num_text;
    sf::Text index;
    int value;
};

enum SortType {
    NONE, BUBBLE, INSERTION, SELECTION, MERGE
};

class Array {
    std::vector<Block> array;
    sf::RenderWindow &window;
    sf::Vector2u window_size;
    sf::Font &font;
    int block_size = 75;
    int value_char_size = 36;
    bool change_made = false;
    bool hide_index = false;
    sf::RectangleShape block_add;
    sf::RectangleShape block_remove;
    std::vector<std::variant<sf::CircleShape, sf::Text>> temp_drawables;
    int value_edit_index = -1;

    void setValue(Block &block, int value = INT32_MIN);

    void insertAt(int val, int index, bool overload);
    void remove(int index, bool overload);

    void swapBlocks(int i, int j);
    void moveBlock(int index, sf::Vector2f to);
    void selectionSort();
    void bubbleSort();
    void insertionSort();
    void mergeSort();

public:
    Array(sf::RenderWindow &window, sf::Font &font);
    void update();
    void addBlock(int value);
    void removeBlock();
    void setBlockSize(int size);
    void handleClick(float mouseX, float mouseY);
    void handleKeypress(char key);
    
    void sort(SortType type);
    void insertAt(int val, int index);
    void remove(int index);
};

#endif