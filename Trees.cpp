#include "Structures.hpp"

structures::Tree::Tree(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
    window_size = window.getSize();
}

void structures::Tree::addNode(int idx, int value){
    if(idx >= tree.size()){
        int depth = log2(idx);
        tree.resize(pow(depth, 2), nullptr);
    }
    Node *new_node = new Node;
    new_node->value = value;
    new_node->value_text = sf::Text(std::to_string(value), font);

    new_node->value_box = sf::RectangleShape({node_size.value_width, node_size.value_width});
    new_node->l_box = sf::RectangleShape({node_size.value_width/2, node_size.ptr_heigth});
    new_node->r_box = sf::RectangleShape({node_size.value_width/2, node_size.ptr_heigth});
    
    new_node->l_dot = sf::CircleShape(5);
    new_node->l_dot.setFillColor(sf::Color::Cyan);
    new_node->r_dot = sf::CircleShape(5);
    new_node->r_dot.setFillColor(sf::Color::Red);

    setValue(new_node, value);
}

void structures::Tree::setValue(Node *node, int value){
    int padding = 10;
    // The default value of value is set to INT32_MIN. This was done to update the block characters without changing the value, in case of a size change only.
    if (value != INT32_MIN) {
        node->value_text.setString(std::to_string(value));
        node->value = value;
    }

    // while the characters are smaller than the optimal size, increase the char size
    while (node->value_text.getGlobalBounds().width < node_size.value_width - 2 * padding and node->value_text.getCharacterSize() <= value_char_size) {
        float charsize = node->value_text.getCharacterSize();
        node->value_text.setCharacterSize(charsize + 1);
    }

    // while the characters are larger, i.e don't fit within the box, decrease the block size
    while (node->value_text.getGlobalBounds().width > node_size.value_width - 2 * padding) {
        float charsize = node->value_text.getCharacterSize();
        if (charsize <= 19) {
            node->value_text.setCharacterSize(18);
            break;
        }
        std::cout << charsize << std::endl;
        node->value_text.setCharacterSize(charsize - 1);
    }

    auto value_bounds = node->value_text.getGlobalBounds();

    float text_width = value_bounds.width;
    // This is to obtain the empty space between bounds and actual position. Else the text may not align properly.
    float text_height = value_bounds.height;
    float empty_space_above_text = value_bounds.top - node->value_text.getPosition().y;
    // Set the position of text as right aligned and bottom aligned.
    int x_pos = node->value_box.getPosition().x + (node_size.value_width - text_width) / 2;
    int y_pos = node->value_box.getPosition().y + (node_size.value_width - text_height) / 2 - empty_space_above_text;
}

void structures::Tree::update(){
    if(change_made){
        
    }
    for(auto node: tree){
        if(node != nullptr){
            window.draw(node->value_box);
            window.draw(node->value_text);
            window.draw(node->l_box);
            window.draw(node->r_box);
            window.draw(node->l_dot);
            window.draw(node->r_dot);
            
        }
    }
}