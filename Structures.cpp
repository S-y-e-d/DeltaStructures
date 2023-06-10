#include "Structures.hpp"

// Use negative outline

void Array::addBlock(int value) {
    Block block;
    block.shape = sf::RectangleShape(sf::Vector2f(block_size, block_size));
    block.shape.setFillColor(sf::Color::Transparent);
    block.shape.setOutlineThickness(-2.0);

    block.index.setFont(font);
    block.index.setCharacterSize(25);

    block.num_text.setFont(font);
    block.num_text.setString(std::to_string(value));
    block.num_text.setCharacterSize(value_char_size);

    array.push_back(block);
    change_made = true;
}

Array::Array(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
    window_size = {window.getSize().x, window.getSize().y};
    addBlock(1);
    block_add = sf::RectangleShape(sf::Vector2f(block_size / 3, block_size));
    block_add.setFillColor(sf::Color::Green);
    block_add.setOutlineThickness(-2.0);
    block_remove = sf::RectangleShape(sf::Vector2f(block_size / 3, block_size));
    block_remove.setFillColor(sf::Color::Red);
    block_remove.setOutlineThickness(-2.0);
    change_made = true;

    auto temp = sf::RectangleShape(sf::Vector2f(1000, 200));
    temp.setPosition(600, 100);
    temp.setFillColor(sf::Color::Transparent);
    temp.setOutlineThickness(-1);
}

void Array::removeBlock() {
    array.pop_back();
    change_made = true;
}

void Array::setBlockSize(int size) {
    block_size = size;
    for (auto &block : array) {
        block.shape.setSize(sf::Vector2f(size, size));
    }
    change_made = true;
}

void Array::handleClick(float mouseX, float mouseY) {
    for (auto &block : array) {
        if (block.shape.getGlobalBounds().contains(mouseX, mouseY)) {
            if (value_edit_index != -1) {
                array[value_edit_index].num_text.setFillColor(sf::Color::White);
            }
            std::string index = block.index.getString();
            value_edit_index = std::stoi(index);
            block.num_text.setFillColor(sf::Color::Yellow);
            block.num_text.setString("0");
            change_made = true;
            return;
        }
    }
    if (value_edit_index != -1) {
        array[value_edit_index].num_text.setFillColor(sf::Color::White);
        value_edit_index = -1;
        change_made = true;
    }
    if (block_add.getGlobalBounds().contains(mouseX, mouseY)) {
        addBlock(0);
        change_made = true;
    } else if (block_remove.getGlobalBounds().contains(mouseX, mouseY)) {
        removeBlock();
        change_made = true;
    }
}

void Array::handleKeypress(char key) {
    if (value_edit_index != -1) {
        sf::Text &block = array[value_edit_index].num_text;
        if (isdigit(key)) {
            if (block.getString() == "0") {
                block.setString("");
            }
            block.setString(block.getString() + key);
            change_made = true;
        } else if (key == '\b') {
            std::string str = block.getString();
            if (str.size() > 1) {
                str.pop_back();
            } else {
                str = "0";
            }
            block.setString(str);
            change_made = true;
        }
    }
}

bool Array::swapBlocks(int i = -1, int j = -1) {
    return true;
}
void Array::selectionSort() {
    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables[0].setFillColor(sf::Color::Green);
    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables[1].setFillColor(sf::Color::Blue);
    temp_drawables[0].setPosition(array[0].shape.getPosition().x - block_size / 2, array[0].shape.getPosition().y - 20);

    for (int i = 0; i < array.size() - 1; i++) {
        auto &num_i = array[i].num_text;

        temp_drawables[0].move(block_size, 0);
        temp_drawables[1].setPosition(temp_drawables[0].getPosition());

        for (int j = i + 1; j < array.size(); j++) {
            auto &num_j = array[j].num_text;

            temp_drawables[1].move(block_size, 0);
            

            if (stoi(std::string(num_i.getString())) > stoi(std::string(num_j.getString()))) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
                float startpos_i = num_i.getPosition().x;
                float startpos_j = num_j.getPosition().x;
                float distance = (j - i) * block_size;
                float velocity = distance / 100;
                while (num_i.getPosition().x < startpos_i + distance and num_j.getPosition().x > startpos_j - distance) {
                    num_i.move(velocity, 0);
                    num_j.move(-velocity, 0);
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                num_i.setPosition(startpos_i + distance, num_i.getPosition().y);
                num_j.setPosition(startpos_j - distance, num_j.getPosition().y);
                std::swap(array[i], array[j]);
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    temp_drawables.clear();
    change_made = true;
    sort_type = SortType::NONE;
}

void Array::sort(SortType type) {
    if (sort_type != SortType::NONE) {
        return;
    }
    if (type == SortType::SELECTION) {
        sort_type = SortType::SELECTION;
        std::thread(&Array::selectionSort, this).detach();
    }
}

void Array::update() {
    if (change_made) {
        int x = (window_size.x - block_size * array.size()) / 2;
        int y = (window_size.y - block_size) / 2;
        int i = 0;
        block_remove.setPosition(x - block_size / 3, y);

        int padding = 10;

        for (auto &block : array) {
            block.shape.setPosition(x + block_size * i, y);
            block.index.setString(std::to_string(i));
            block.index.setPosition(x + block_size * i + padding, y);

            while (block.num_text.getGlobalBounds().width < block_size - 2 * padding and block.num_text.getCharacterSize() <= value_char_size) {
                float charsize = block.num_text.getCharacterSize();
                block.num_text.setCharacterSize(charsize + 1);
            }

            while (block.num_text.getGlobalBounds().width > block_size - 2 * padding) {
                float charsize = block.num_text.getCharacterSize();
                block.num_text.setCharacterSize(charsize - 1);
            }
            auto value_bounds = block.num_text.getGlobalBounds();
            float value_width = value_bounds.width;
            float value_height = value_bounds.top + value_bounds.height - block.num_text.getPosition().y;

            block.num_text.setPosition(x + block_size * i + (block_size - value_width) - padding, y + (block_size - value_height) - padding);

            i++;
        }
        block_add.setPosition(x + block_size * i, y);

        change_made = false;
    }
    window.draw(block_add);
    window.draw(block_remove);
    for (auto &block : array) {
        window.draw(block.shape);
        window.draw(block.index);
        window.draw(block.num_text);
    }
    for (auto &temp : temp_drawables) {
        window.draw(temp);
    }
}