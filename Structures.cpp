#include "Structures.hpp"

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

    block.value = value;

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
            block.value = 0;
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

void Array::setValue(Block &block, int value) {
    int padding = 10;
    if (value != INT32_MIN) {
        block.num_text.setString(std::to_string(value));
        block.value = value;
    }

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

    block.num_text.setPosition(block.shape.getPosition().x + (block_size - value_width) - padding, block.shape.getPosition().y + (block_size - value_height) - padding);
}

void Array::handleKeypress(char key) {
    if (value_edit_index != -1) {
        Block &block = array[value_edit_index];
        if (isdigit(key)) {
            if (block.num_text.getString() == "0") {
                block.num_text.setString("");
            }
            setValue(block, stoi(std::string(block.num_text.getString()+key)));
        } else if (key == '\b') {
            std::string str = block.num_text.getString();
            if (str.size() > 1) {
                str.pop_back();
            } else {
                str = "0";
            }
            setValue(block, stoi(std::string(block.num_text.getString()+key)));
        }
    }
}

void Array::insertAt(int val, int index, bool overload) {
    using namespace std::chrono_literals;

    static bool in_progress = false;
    if (in_progress) return;
    in_progress = true;

    array[index].num_text.setFillColor(sf::Color::Green);
    for (int i = array.size() - 1; i > index; i--) {
        temp_drawables.push_back(array[i - 1].num_text);
        sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
        temp_text.setFillColor(sf::Color::White);
        for (int i = 0; i <= block_size; i++) {
            temp_text.move(1, 0);
            std::this_thread::sleep_for(10ms);
        }
        temp_drawables.pop_back();
        setValue(array[i], array[i - 1].value);
    }
    temp_drawables.push_back(array[index].num_text);
    sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
    temp_text.setString(std::to_string(val));
    temp_text.move(0, -100);

    for (int i = 0; i < 100; i++) {
        temp_text.move(0, 1);
        std::this_thread::sleep_for(10ms);
    }
    temp_drawables.pop_back();
    setValue(array[index], val);
    array[index].num_text.setFillColor(sf::Color::White);
    in_progress = false;
}

void Array::insertAt(int val, int index) {
    void (Array::*func)(int, int, bool) = &Array::insertAt;
    std::thread(func, this, val, index, true).detach();
}

void Array::remove(int index, bool overload) {
    using namespace std::chrono_literals;

    static bool in_progress = false;
    if (in_progress) return;
    in_progress = true;

    array[index].num_text.setFillColor(sf::Color::Red);

    for (int i = index; i < array.size() - 1; i++) {
        temp_drawables.push_back(array[i + 1].num_text);
        sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
        for (int j = 0; j <= block_size; j++) {
            temp_text.move(-1, 0);
            std::this_thread::sleep_for(10ms);
        }

        temp_drawables.pop_back();
        setValue(array[i], array[i + 1].value);
        array[i].num_text.setFillColor(array[i + 1].num_text.getFillColor());
        std::this_thread::sleep_for(0.5s);
    }
    setValue(array.back(), 0);
    in_progress = false;
}

void Array::remove(int index) {
    void (Array::*func)(int, bool) = &Array::remove;
    std::thread(func, this, index, true).detach();
}

void Array::swapBlocks(int i, int j) {
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    if (i > j) std::swap(i, j);
    sf::Text &num_i = array[i].num_text;
    sf::Text &num_j = array[j].num_text;
    float distance = (j - i) * block_size;
    float velocity = distance / 100;
    for (int i = 0; i < 100; i++) {
        num_i.move(velocity, 0);
        num_j.move(-velocity, 0);
        std::this_thread::sleep_for(10ms);
    }
    std::swap(array[i], array[j]);
}

void Array::selectionSort() {
    using namespace std::chrono_literals;
    static bool in_progress = false;
    if (in_progress) return;
    in_progress = true;
    // Circles representing i and j pointers
    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);
    iptr.setPosition(array[0].shape.getPosition().x - block_size / 2, array[0].shape.getPosition().y - 20);

    for (int i = 0; i < array.size() - 1; i++) {
        iptr.move(block_size, 0);
        jptr.setPosition(iptr.getPosition());

        for (int j = i + 1; j < array.size(); j++) {
            jptr.move(block_size, 0);

            if (array[i].value > array[j].value) {
                swapBlocks(i, j);
            }
            std::this_thread::sleep_for(1s);
        }
    }
    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::bubbleSort() {
    using namespace std::chrono_literals;

    static bool in_progress = false;
    if (in_progress) return;
    in_progress = true;

    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);

    for (int i = 0; i < array.size() - 1; i++) {
        iptr.setPosition(array[0].shape.getPosition().x - block_size / 2, array[0].shape.getPosition().y - 20);
        std::get<sf::CircleShape>(temp_drawables[1]).setPosition(iptr.getPosition());
        std::get<sf::CircleShape>(temp_drawables[1]).move(block_size, 0);

        for (int j = 0; j < array.size() - i - 1; j++) {
            iptr.move(block_size, 0);
            std::get<sf::CircleShape>(temp_drawables[1]).move(block_size, 0);

            if (array[j].value > array[j + 1].value) {
                swapBlocks(j, j + 1);
            }
            std::this_thread::sleep_for(1s);
        }
    }
    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::insertionSort() {
    using namespace std::chrono_literals;

    static bool in_progress = false;
    if (in_progress) return;
    in_progress = true;

    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);

    for (int i = 1; i < array.size(); i++) {
        iptr.setPosition(array[i].shape.getPosition().x + block_size / 2, array[i].shape.getPosition().y - 20);
        jptr.setPosition(iptr.getPosition());
        jptr.move(-block_size, 0);
        int j = i;
        while (j >= 1 and array[j].value < array[j - 1].value) {
            swapBlocks(j, j - 1);
            j--;
            jptr.move(-block_size, 0);
        }
        std::this_thread::sleep_for(1s);
    }

    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::moveBlock(int index, sf::Vector2f to) {
    using namespace std::chrono_literals;
    Block block = array[index];
    sf::Vector2f displacement(to.x - block.shape.getPosition().x, to.y - block.shape.getPosition().y);
    sf::Vector2f velocity(displacement.x / 100, displacement.y / 100);
    for (int i = 0; i < 100; i++) {
        block.shape.move(displacement);
        block.num_text.move(displacement);
        block.index.move(displacement);
        std::this_thread::sleep_for(10ms);
    }
}

void Array::sort(SortType type) {
    switch (type) {
        case SortType::SELECTION:
            std::thread(&Array::selectionSort, this).detach();
            return;
        case SortType::BUBBLE:
            std::thread(&Array::bubbleSort, this).detach();
            return;
        case SortType::INSERTION:
            std::thread(&Array::insertionSort, this).detach();
            return;
        default:
            return;
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

            setValue(block);
            // while (block.num_text.getGlobalBounds().width < block_size - 2 * padding and block.num_text.getCharacterSize() <= value_char_size) {
            //     float charsize = block.num_text.getCharacterSize();
            //     block.num_text.setCharacterSize(charsize + 1);
            // }

            // while (block.num_text.getGlobalBounds().width > block_size - 2 * padding) {
            //     float charsize = block.num_text.getCharacterSize();
            //     block.num_text.setCharacterSize(charsize - 1);
            // }
            // auto value_bounds = block.num_text.getGlobalBounds();
            // float value_width = value_bounds.width;
            // float value_height = value_bounds.top + value_bounds.height - block.num_text.getPosition().y;

            // block.num_text.setPosition(x + block_size * i + (block_size - value_width) - padding, y + (block_size - value_height) - padding);

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
        std::visit([&](auto &&obj) { window.draw(obj); }, temp);
    }
}