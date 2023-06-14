#include "Structures.hpp"

void LinkedList::addNode(int value, int idx = -1) {
    Node temp;
    temp.value_box = sf::RectangleShape(sf::Vector2f(node_size.value_width, node_size.height));
    temp.value_box.setFillColor(sf::Color::Transparent);
    temp.value_box.setOutlineThickness(-2);
    temp.ptr_box = sf::RectangleShape(sf::Vector2f(node_size.ptr_width, node_size.height));
    temp.ptr_box.setFillColor(sf::Color::Transparent);
    temp.ptr_box.setOutlineThickness(-2);

    temp.value = value;
    temp.ptr_dot = sf::CircleShape(5);
    temp.ptr_dot.setFillColor(sf::Color::Blue);
    temp.self_dot = sf::CircleShape(5);
    temp.self_dot.setFillColor(sf::Color::Red);
    temp.value_text.setFont(font);
    temp.value_text.setCharacterSize(36);
    temp.value_text.setString(std::to_string(value));

    if (idx == -1) {
        list.push_back(temp);
    } else {
        list.insert(list.begin() + 1, temp);
    }
    change_made = true;
}

LinkedList::LinkedList(sf::RenderWindow& window, sf::Font& font) : window(window), font(font) {
    window_size = window.getSize();
    addNode(0);
    addNode(1);
    addNode(2);
}

void LinkedList::removeNode() {
    if (list.size() > 1) {
        list.pop_back();
    }
}

void LinkedList::handleClick() {
}

void LinkedList::handleKeypress() {
}

void LinkedList::setValue(Node& node, int value = INT32_MIN) {
    int padding = 10;
    // The default value of value is set to INT32_MIN. This was done to update the block characters without changing the value, in case of a size change only.
    if (value != INT32_MIN) {
        node.value_text.setString(std::to_string(value));
        node.value = value;
    }

    // while the characters are smaller than the optimal size, increase the char size
    while (node.value_text.getGlobalBounds().width < node_size.value_width - 2 * padding and node.value_text.getCharacterSize() <= value_char_size) {
        float charsize = node.value_text.getCharacterSize();
        node.value_text.setCharacterSize(charsize + 1);
    }

    // while the characters are larger, i.e don't fit within the box, decrease the block size
    while (node.value_text.getGlobalBounds().width > node_size.value_width - 2 * padding) {
        float charsize = node.value_text.getCharacterSize();
        node.value_text.setCharacterSize(charsize - 1);
    }

    auto value_bounds = node.value_text.getGlobalBounds();

    float text_width = value_bounds.width;
    // This is to obtain the empty space between bounds and actual position. Else the text may not align properly.
    float text_height = value_bounds.height;
    float empty_space_above_text = value_bounds.top - node.value_text.getPosition().y;
    // Set the position of text as right aligned and bottom aligned.
    float x_pos = node.value_box.getPosition().x + (node_size.value_width - text_width) / 2;
    float y_pos = node.value_box.getPosition().y + (node_size.height - text_height) / 2 - empty_space_above_text;
    node.value_text.setPosition(x_pos, y_pos);
}

void LinkedList::update() {
    if (change_made) {
        float x_pos = window_size.x;                       // get the window width
        x_pos -= node_size.width * list.size();            // subtract the size of all the node blocks (value+ptr box)
        x_pos -= node_size.ptr_width * (list.size() - 1);  // subtract the size of the gaps in between
        x_pos /= 2;                                        // divide by 2 to get the center point where the list should start
        float y_pos = ((float)(window_size.y - node_size.height)) / 2;
        for (std::vector<Node>::iterator node = list.begin(); node != list.end(); node++) {
            node->value_box.setPosition(x_pos, y_pos);
            node->ptr_box.setPosition(x_pos + node_size.value_width, y_pos);
            node->self_dot.setPosition(x_pos - 5 + 1, y_pos + (node_size.height - 10) / 2);  // compensate for the border thickness to draw circle on the line
            node->ptr_dot.setPosition(x_pos + node_size.value_width + (node_size.ptr_width - 10) / 2, y_pos + (node_size.height - 10) / 2);
            if(node != list.begin()){
                node->connector_line[0] = (node-1)->ptr_dot.getPosition() + sf::Vector2f(5, 5);
                node->connector_line[0].color = sf::Color::Blue;
                node->connector_line[1] = node->self_dot.getPosition() + sf::Vector2f(5, 5);
                node->connector_line[1].color = sf::Color::Red;
            }
            setValue(*node);
            x_pos += node_size.width + node_size.ptr_width;
        }
        change_made = false;
    }
    for (auto& node : list) {
        window.draw(node.connector_line, 2, sf::Lines);
        window.draw(node.ptr_box);
        window.draw(node.value_box);
        window.draw(node.self_dot);
        window.draw(node.ptr_dot);
        window.draw(node.value_text);
    }
}
