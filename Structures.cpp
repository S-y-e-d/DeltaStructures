#include "Structures.hpp"

// Use negative outline

void Array::addBlock(int value) {
    Block block;
    block.shape = sf::RectangleShape(sf::Vector2f(array_block_size, array_block_size));
    block.shape.setFillColor(sf::Color::Transparent);
    block.shape.setOutlineThickness(-2.0);
    block.value = value;

    block.num_text.setFont(font);
    block.index.setFont(font);
    block.index.setCharacterSize(25);
    block.num_text.setString(std::to_string(value));
    block.num_text.setCharacterSize(25);
    array.push_back(block);
    change_made = true;
}

Array::Array(sf::RenderWindow &window, sf::Font &font) : window(window), font(font){
    addBlock(0);
    block_add = sf::RectangleShape(sf::Vector2f(array_block_size / 3, array_block_size));
    block_add.setFillColor(sf::Color::Green);
    block_add.setOutlineThickness(-2.0);
    block_remove = sf::RectangleShape(sf::Vector2f(array_block_size / 3, array_block_size));
    block_remove.setFillColor(sf::Color::Red);
    block_remove.setOutlineThickness(-2.0);
    change_made = true;
}

void Array::removeBlock() {
    array.pop_back();
    change_made = true;
}

void Array::setBlockSize(int size) {
    array_block_size = size;
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
        int x = (window.getSize().x - array[0].shape.getSize().x * array.size()) / 2;
        int y = window.getSize().y;
        int i = 0;
        block_remove.setPosition(x-array_block_size/3, (y - array_block_size)/2);
        for (auto &block : array) {
            block.shape.setPosition(x + block.shape.getSize().y * i, (y - block.shape.getSize().y) / 2);
            block.index.setString(std::to_string(i));
            block.index.setPosition(x + block.shape.getSize().y * i++ + 5 /*padding*/, (y - block.shape.getSize().y) / 2);
        }
        block_add.setPosition(x+array_block_size*i, (y - array_block_size)/2);
        change_made = false;
    }
    window.draw(block_add);
    window.draw(block_remove);
    for (auto &block : array) {
        window.draw(block.shape);
        window.draw(block.index);
    }
}