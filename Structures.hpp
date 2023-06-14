#ifndef Structures_hpp
#define Structures_hpp


#include <SFML/Graphics.hpp>
#include <math.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>
#include <vector>

struct Block {
    sf::RectangleShape shape;
    sf::Text num_text;
    sf::Text index;
    int value;
};

enum SortType {
    NONE,
    BUBBLE,
    INSERTION,
    SELECTION,
    MERGE
};

class Array {
    std::vector<Block> array;
    sf::RenderWindow &window;
    sf::Font &font;
    sf::Vector2u window_size;
    int block_size = 75;
    int value_char_size = 36;
    bool change_made = false;
    bool hide_index = false;
    bool in_progress = false;
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
    void makeGradient();

public:
    Array(sf::RenderWindow &window, sf::Font &font);
    void addBlock(int value);
    void removeBlock();
    void update();
    void randomize();
    void setBlockSize(int size);
    void handleClick(float mouseX, float mouseY);
    void handleKeypress(char key);

    void sort(SortType type);
    void insertAt(int val, int index);
    void remove(int index);
};


struct NodeSize{
    float value_width;
    float ptr_width;
    float width;
    float height;
};

struct Node {
    sf::RectangleShape value_box;
    sf::RectangleShape ptr_box;
    sf::Vertex connector_line[2] = {sf::Vertex(sf::Vector2f(0, 0)), sf::Vertex(sf::Vector2f(0, 0))};
    sf::CircleShape ptr_dot;
    sf::CircleShape self_dot;
    sf::Text value_text;
    int value;
};

class LinkedList {
    std::vector<Node> list;
    sf::RenderWindow &window;
    sf::Font &font;
    sf::Vector2u window_size;
    NodeSize node_size = {75, 50, 125, 75};
    bool change_made = false;
    bool in_progress = false;
    int value_char_size = 36;
    int value_edit_index = -1;

    void setValue(Node &node, int value);

public:
    LinkedList(sf::RenderWindow &, sf::Font &);
    void addNode(int value, int idx);
    void removeNode();
    void handleClick();
    void handleKeypress();
    void update();
};

#endif