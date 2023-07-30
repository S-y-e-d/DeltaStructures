#include "Structures.hpp"

structures::Tree::Tree(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
    window_size = window.getSize();

    addNode(1, 5);
    // std::vector<int> tree_maker = {
    //     1,
    //     2, 3,
    //     4, 5, 6, -1,
    //     8, 9, 10, 11, -1, -1, -1, -1};
    // for (int i = 0; i < tree_maker.size(); i++) {
    //     if (tree_maker[i] != -1)
    //         addNode(i + 1, tree_maker[i]);
    // }
}

void structures::Tree::addNode(int idx, int value) {
    if (idx >= tree.size()) {
        int depth = log2(idx);
        tree.resize(pow(2, depth + 1), nullptr);
    }
    if(tree[idx] != nullptr){
        return;
    }
    Node *new_node = new Node;
    new_node->value = value;
    new_node->value_text = sf::Text(std::to_string(value), font);
    new_node->value_text.setFillColor(sf::Color::White);

    new_node->value_box = sf::RectangleShape({node_size.value_width, node_size.value_height});
    new_node->value_box.setOutlineColor(sf::Color::White);
    new_node->value_box.setOutlineThickness(-2);
    new_node->value_box.setFillColor(sf::Color::Transparent);

    new_node->l_box = sf::RectangleShape({node_size.ptr_width, node_size.ptr_height});
    new_node->l_box.setOutlineColor(sf::Color::White);
    new_node->l_box.setOutlineThickness(-2);
    new_node->l_box.setFillColor(sf::Color::Transparent);

    new_node->r_box = sf::RectangleShape({node_size.ptr_width, node_size.ptr_height});
    new_node->r_box.setOutlineColor(sf::Color::White);
    new_node->r_box.setOutlineThickness(-2);
    new_node->r_box.setFillColor(sf::Color::Transparent);

    new_node->l_dot = sf::CircleShape(3);
    new_node->l_dot.setFillColor(sf::Color::Cyan);
    new_node->r_dot = sf::CircleShape(3);
    new_node->r_dot.setFillColor(sf::Color::Magenta);

    setValue(new_node, value);

    tree[idx] = new_node;
    change_made = true;
}

void structures::Tree::deleteNode(int idx) {
    if(idx == 0){
        return;
    }
    deleteNode(left(idx));
    deleteNode(right(idx));

    delete tree[idx];
    tree[idx] = nullptr;
    change_made = true;
}

void structures::Tree::setValue(Node *node, int value) {
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
        node->value_text.setCharacterSize(charsize - 1);
    }

    auto value_bounds = node->value_text.getGlobalBounds();

    float text_width = value_bounds.width;
    // This is to obtain the empty space between bounds and actual position. Else the text may not align properly.
    float text_height = value_bounds.height;
    float empty_space_above_text = value_bounds.top - node->value_text.getPosition().y;
    float empty_space_side_text = value_bounds.left - node->value_text.getPosition().x;
    // Set the position of text as right aligned and bottom aligned.
    int x_pos = node->value_box.getPosition().x + (node_size.value_width - text_width) / 2 - empty_space_side_text;
    int y_pos = node->value_box.getPosition().y + (node_size.value_height - text_height) / 2 - empty_space_above_text;

    node->value_text.setPosition(x_pos, y_pos);
}

int structures::Tree::left(int node_idx) {
    if (node_idx * 2 < tree.size() and tree[node_idx * 2] != nullptr) {
        return node_idx * 2;
    }
    return 0;
}

int structures::Tree::right(int node_idx) {
    if (node_idx * 2 + 1 < tree.size() and tree[node_idx * 2 + 1] != nullptr) {
        return node_idx * 2 + 1;
    }
    return 0;
}

int structures::Tree::parent(int node_idx) {
    return node_idx / 2;
}

int structures::Tree::depth(int node_idx) {
    return (int)log2(node_idx);
}

// return the max number of nodes on the same level under the current node.
int structures::Tree::width(int node_idx) {
    if (node_idx >= tree.size() or node_idx < 1) {
        return 0;
    }
    if (not left(node_idx) and not right(node_idx)) {
        return 1;
    }
    int a = width(left(node_idx));
    int b = width(right(node_idx));
    return a + b;
}

int structures::Tree::fillMatrix(int node_idx, int l, int r) {
    /*
        Since the nodes will be separated by one node length each, the formula for physical width is 2 times the nodes - 1
    */
    int lwidth = width(left(node_idx)) * 2 - 1;
    int lcenter = (l + r) / 2;
    if (left(node_idx)) {
        lcenter = fillMatrix(left(node_idx), l, l + lwidth);
    }
    int rcenter = (l + r) / 2;
    if (right(node_idx)) {
        rcenter = fillMatrix(right(node_idx), l + lwidth + 1, r);
    }
    int center = (lcenter + rcenter) / 2;
    position_matrix[depth(node_idx)][center] = node_idx;
    return center;
}

void structures::Tree::handleClick(float mouseX, float mouseY) {
    if (mouseX == 0 and mouseY == 0) {
        return;
    }
    bool left_click = true;
    if (mouseX < 0) {
        left_click = false;
        mouseX = -mouseX;
        mouseY = -mouseY;
    }
    for (int node = 1; node < tree.size(); node++) {
        if (tree[node] == nullptr) {
            continue;
        }
        if (tree[node]->value_box.getGlobalBounds().contains(mouseX, mouseY)) {
            tree[node]->value_text.setFillColor(sf::Color::Yellow);
            setValue(tree[node], 0);
            value_edit_index = node;
            return;
        }
        tree[node]->value_text.setFillColor(sf::Color::White);
        value_edit_index = -1;

        if (tree[node]->l_box.getGlobalBounds().contains(mouseX, mouseY)) {
            if (left_click) {
                addNode(node * 2, rand()%100);
            } else {
                deleteNode(node * 2);
            }
        }

        if (tree[node]->r_box.getGlobalBounds().contains(mouseX, mouseY)) {
            if (left_click) {
                addNode(node * 2 + 1, rand()%100);
            } else {
                deleteNode(node * 2 + 1);
            }
        }
    }
}

void structures::Tree::handleKeypress(char key) {
    if (key == '\0' or value_edit_index == -1) {
        return;
    }
    Node *node = tree[value_edit_index];
    // If pressed a number
    if (isdigit(key)) {
        if (node->value_text.getString() == "0") {
            node->value_text.setString("");
        }
        // Do nothing if the text is 18 size or smaller
        if (node->value_text.getCharacterSize() <= 18) {
            return;
        }
        setValue(node, stoi(std::string(node->value_text.getString() + key)));
    } else if (key == '\b') {
        std::string str = node->value_text.getString();
        if (str.size() > 1) {
            str.pop_back();
        } else {
            str = "0";
        }
        setValue(node, std::stoi(str));
    }
}

void structures::Tree::update(int mouseX, int mouseY, char key) {
    handleClick(mouseX, mouseY);
    handleKeypress(key);
    if (change_made) {
        int max_width = width(root) * 2 - 1;
        int max_depth = log2(tree.size());
        // Clear the position matrix and refill it with -1
        position_matrix.clear();
        for (int i = 0; i < max_depth; i++) {
            std::vector<int> temp(max_width, -1);
            position_matrix.push_back(temp);
        }
        fillMatrix(root, 0, max_width);

        int max_x = width(root) * (node_size.value_width + 20);  // The width contains the spaces between nodes
        if (max_x >= window_size.x - 100) {
            throw std::overflow_error("Max width reached");
        }

        int max_y = position_matrix.size() * node_size.value_width + (position_matrix.size() - 1) * node_size.value_height;  // the depth doesn't contain the spacing
        if (max_y >= window_size.y - 100) {
            throw std::overflow_error("Max depth reached");
        }

        int start_y = (window_size.y - max_y) / 2;
        for (int i = 0; i < position_matrix.size(); i++) {
            int start_x = (window_size.x - max_x) / 2 - 10;
            for (int j = 0; j < position_matrix[i].size(); j++) {
                start_x += 10;  // minimum spacing between nodes
                if (position_matrix[i][j] == -1) {
                    continue;
                }
                int current = position_matrix[i][j];
                // The width and the height of the total box should be the same
                tree[current]->value_box.setPosition(
                    start_x + j * node_size.ptr_width,
                    start_y + i * node_size.ptr_width);

                tree[current]->l_box.setPosition(
                    start_x + j * node_size.ptr_width,
                    start_y + i * node_size.ptr_width + node_size.value_height - 1);

                tree[current]->r_box.setPosition(
                    start_x + j * node_size.ptr_width + node_size.ptr_width - 1,
                    start_y + i * node_size.ptr_width + node_size.value_height - 1);

                tree[current]->l_dot.setPosition(
                    start_x + j * node_size.ptr_width,
                    start_y + i * node_size.ptr_width);

                tree[current]->r_dot.setPosition(
                    start_x + j * node_size.ptr_width,
                    start_y + i * node_size.ptr_width);

                // subtract the radius of the circle to adjust
                tree[current]->l_dot.setPosition(
                    tree[current]->l_box.getPosition() + sf::Vector2f(node_size.ptr_width / 2 - 3, node_size.ptr_height / 2 - 3));

                tree[current]->r_dot.setPosition(
                    tree[current]->r_box.getPosition() + sf::Vector2f(node_size.ptr_width / 2 - 3, node_size.ptr_height / 2 - 3));

                setValue(tree[current]);
            }
            start_y += node_size.value_width;
        }

        for (int i = 0; i < tree.size(); i++) {
            if (tree[i] == nullptr) {
                continue;
            }
            if (left(i)) {
                tree[i]->l_connector.first = tree[i]->l_dot.getPosition() + sf::Vector2f(3, 3);  // radius of the dot
                tree[i]->l_connector.second =
                    tree[left(i)]->value_box.getPosition() + sf::Vector2f(node_size.value_width / 2, 0) + sf::Vector2f(0, 0);
            } else {
                tree[i]->l_connector.first = tree[i]->l_dot.getPosition();
                tree[i]->l_connector.second = tree[i]->l_dot.getPosition();
            }

            if (right(i)) {
                tree[i]->r_connector.first = tree[i]->r_dot.getPosition() + sf::Vector2f(3, 3);
                tree[i]->r_connector.second =
                    tree[right(i)]->value_box.getPosition() + sf::Vector2f(node_size.value_width / 2, 0) + sf::Vector2f(0, 0);
            } else {
                tree[i]->r_connector.first = tree[i]->r_dot.getPosition();
                tree[i]->r_connector.second = tree[i]->r_dot.getPosition();
            }
        }

        change_made = false;
    }
    for (auto &node : tree) {
        if (node != nullptr) {
            window.draw(node->value_box);
            window.draw(node->value_text);
            window.draw(node->l_box);
            window.draw(node->r_box);
            window.draw(node->l_dot);
            window.draw(node->r_dot);

            sf::Vertex line[2] = {node->l_connector.first, node->l_connector.second};
            line[0].color = sf::Color::Cyan;
            line[1].color = sf::Color::Yellow;
            window.draw(line, 2, sf::Lines);

            sf::Vertex line2[2] = {node->r_connector.first, node->r_connector.second};
            line2[0].color = sf::Color::Magenta;
            line2[1].color = sf::Color::Yellow;
            window.draw(line2, 2, sf::Lines);
        }
    }
}

structures::Tree::~Tree() {
    for (int i = 0; i < tree.size(); i++) {
        if (tree[i] != nullptr) {
            delete tree[i];
        }
    }
}