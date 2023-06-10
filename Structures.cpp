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
    static sf::Text *num_text1 = nullptr;
    static std::pair<int, int> indices;
    static sf::Text *num_text2 = nullptr;
    static std::pair<float, float> swap_pos;
    static float velocity = 0;
    if (i == -1 and num_text1 == nullptr) {
        return false;
    }
    if (i != -1) {
        num_text1 = &array[std::min(i, j)].num_text;
        num_text2 = &array[std::max(i, j)].num_text;
        indices = {i, j};
        swap_pos = {num_text1->getPosition().x, num_text2->getPosition().x};
        velocity = (swap_pos.second - swap_pos.first) / 60;
    }
    if (num_text1 != nullptr) {
        if (num_text1->getPosition().x < swap_pos.first + block_size * (indices.second- indices.first)) {
            num_text1->move(velocity, 0);
            num_text2->move(-velocity, 0);
            return true;
        } else {
            num_text1->setPosition(swap_pos.first + block_size * (indices.second - indices.first), num_text1->getPosition().y);
            num_text2->setPosition(swap_pos.second - block_size * (indices.second - indices.first), num_text2->getPosition().y);
            
            std::swap(array[indices.first], array[indices.second]);
            num_text1->setFillColor(sf::Color::Green);
            num_text2->setFillColor(sf::Color::Blue);
            
            num_text1 = nullptr;
            num_text2 = nullptr;
        }
    }
    return false;
}

void Array::selectionSort() {
    static int i = 0;
    static int j = 1;
    static int frame_counter = 0;
    frame_counter = (frame_counter + 1) % 60;
    static sf::Text *t1, *t2;

    // Returns if the blocks are currently swapping
    if (swapBlocks()) {
        return;
    } else {
        // If I is at the end of the array, this means the sorting is done. Reset all values.
        if (i == array.size() - 1) {
            i = 0;
            j = 1;
            t1->setFillColor(sf::Color::White);
            t2->setFillColor(sf::Color::White);
            t1 = nullptr;
            t2 = nullptr;
            sort_type = SortType::NONE;
            change_made = true;
            return;
        }
        // If text is not the same as the previous, set the previous to white and current highligthted. Might change to i and j pointers instead of colors.
        if (t1 != &array[i].num_text) {
            if (t1 != nullptr) {
                t1->setFillColor(sf::Color::White);
            }
            t1 = &array[i].num_text;
            t1->setFillColor(sf::Color::Green);
            return;
        }
        if (t2 != &array[j].num_text) {
            if (t2 != nullptr) {
                t2->setFillColor(sf::Color::White);
            }
            t2 = &array[j].num_text;
            t2->setFillColor(sf::Color::Blue);
            return;
        }

        int val_i = std::stoi((std::string)array[i].num_text.getString());
        int val_j = std::stoi((std::string)array[j].num_text.getString());
        if (val_j < val_i) {
            // Used a roundabout method to sleep to prevent everything from freezing. This it probably a bad way but cannod refactor everything to consider sleep into account.
            if (frame_counter) {
                return;
            }
            swapBlocks(i, j);
            return;
        } else {
            if (frame_counter){
                return;
            }
        }
        j++;
        if (j == array.size()) {
            i++;
            j = i + 1;
        }
    }
}

void Array::sort(SortType type) {
    if (sort_type == SortType::NONE) {
        sort_type = type;
    }
}

void Array::update() {
    if (sort_type != SortType::NONE) {
        std::thread(&Array::selectionSort, this).detach();
    }
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
}