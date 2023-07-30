#ifndef Structures_hpp
#define Structures_hpp

#include <math.h>

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>
#include <vector>

namespace structures {
    using namespace std::chrono_literals;

    class Array {
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

    class LinkedList {
        struct NodeSize {
            float value_width;
            float ptr_width;
            float width;
            float height;
        };
        struct Node {
            sf::RectangleShape value_box;
            sf::RectangleShape ptr_box;
            sf::CircleShape ptr_dot;
            sf::CircleShape self_dot;
            sf::Text value_text;
            int value;
        };
        std::vector<Node> list;
        std::vector<std::pair<sf::Vertex, sf::Vertex>> connector_lines;  // could refactor to include this in node
        sf::RenderWindow &window;
        sf::Font &font;
        sf::Vector2u window_size;
        NodeSize node_size = {75, 50, 125, 75};
        bool change_made = false;
        bool in_progress = false;
        int value_char_size = 36;
        int value_edit_index = -1;
        std::vector<std::variant<sf::CircleShape, sf::Text>> temp_drawables;
        std::vector<Node> temp_nodes;

        void setValue(Node &node, int value);

        void moveNode(Node &node, sf::Vector2f to, int velocity = 1, bool relative = false);  // default velocity = 1

        void insert(int value, int after, bool thread);
        void deleteNode(int index, bool thread);

    public:
        LinkedList(sf::RenderWindow &, sf::Font &);
        void addNode(int value, int idx);
        void removeNode(int idx);  // default - 1
        void handleClick(float mouseX, float mouseY);
        void handleKeypress(char key);
        void update();

        void insertAfter(int value, int after);
        void deleteNode(int index);
    };

    // Can use this but I feel like this is unnecessary since the deletion is handled already
    // template <typename T>
    // class SmartPtr{
    //     T ptr;
    // public:
    //     explicit SmartPtr(T *p) : ptr(p){}
    //     ~SmartPtr(){
    //         delete ptr;
    //         ptr = nullptr;
    //     }
    //     T& operator*(){
    //         return *ptr;
    //     }
    //     T* operator->(){
    //         return ptr;
    //     }
    // };

    class Tree {
        struct Node {
            int value;
            sf::Text value_text;
            sf::RectangleShape value_box;
            sf::RectangleShape l_box;
            sf::RectangleShape r_box;
            sf::CircleShape l_dot;
            sf::CircleShape r_dot;
            std::pair<sf::Vertex, sf::Vertex> l_connector;
            std::pair<sf::Vertex, sf::Vertex> r_connector;
        };
        struct NodeSize {
            float value_width;
            float value_height;
            float ptr_width;
            float ptr_height;
        };

        std::vector<Node *> tree;  // used pointers because of array implementation of trees. Easier to detect nulls
        
        sf::RenderWindow &window;
        sf::Font &font;
        sf::Vector2u window_size;
        
        std::vector<std::variant<sf::CircleShape, sf::Text>> temp_drawables;
        std::vector<Node *> temp_nodes;
        // used to calculate the positions to draw the nodes on efficiently. 
        std::vector<std::vector<int>> position_matrix; 

        NodeSize node_size = {75, 51, 38, 25};
        const int root = 1;
        bool change_made = false;
        bool in_progress = false;
        int value_char_size = 36;
        int value_edit_index = -1;

        int left(int node_idx);
        int right(int node_idx);
        int parent(int node_idx);
        int width(int node_idx);
        int depth(int node_idx);
        int fillMatrix(int node_idx, int l, int r);
        void handleClick(float mouseX, float mouseY);
        void handleKeypress(char key);

    
    public:
        Tree(sf::RenderWindow&, sf::Font&);
        ~Tree();
        void addNode(int idx, int value);
        void deleteNode(int idx);
        void setValue(Node *node, int value = INT32_MIN);
        
        void update(int mouseX, int mouseY, char key);
    };
}

#endif