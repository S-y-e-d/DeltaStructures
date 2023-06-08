#include "Structures.hpp"

// Use negative outline

void Array::addBlock(int value) {
    Block block;
    block.shape = sf::RectangleShape(sf::Vector2f(block_size, block_size));
    block.shape.setFillColor(sf::Color::Transparent);
    block.shape.setOutlineThickness(-2.0);
    block.value = value;

    block.index.setFont(font);
    block.index.setCharacterSize(25);

    block.num_text.setFont(font);
    block.num_text.setString(std::to_string(value));
    block.num_text.setCharacterSize(36);

    array.push_back(block);
    change_made = true;
}

Array::Array(sf::RenderWindow &window, sf::Font &font) : window(window), font(font) {
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
    if (block_add.getGlobalBounds().contains(mouseX, mouseY)) {
        addBlock(0);
        change_made = true;
    }
    if (block_remove.getGlobalBounds().contains(mouseX, mouseY)) {
        removeBlock();
        change_made = true;
    }
}

void Array::update() {
    if (change_made) {
        int x = (window.getSize().x - block_size * array.size()) / 2;
        int y = (window.getSize().y - block_size)/2;
        int i = 0;
        block_remove.setPosition(x - block_size / 3, y);
        
        int padding = 10;



        for (auto &block : array) {
            block.shape.setPosition(x + block_size * i, y);
            block.index.setString(std::to_string(i));
            block.index.setPosition(x + block_size * i + padding, y);

            while(block.num_text.getGlobalBounds().width > block_size - 2*padding){
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