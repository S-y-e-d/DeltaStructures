#include "Structures.hpp"

void Array::addBlock(int value) {
    Block block;
    block.shape = sf::RectangleShape(sf::Vector2f(100, 100));
    block.shape.setPosition(100, 100);
    block.shape.setFillColor(sf::Color::Transparent);
    block.shape.setOutlineColor(sf::Color::White);
    block.shape.setOutlineThickness(1.0);
    block.value = value;
    array.push_back(block);
}

Array::Array() {
    addBlock(0);
}

void Array::update(sf::RenderWindow& window) {
    int x = (window.getSize().x - array[0].shape.getSize().x * array.size()) / 2;
    int y = window.getSize().y;
    int i = 0;
    for (auto block : array) {
        block.shape.setPosition(x + block.shape.getSize().y * i++, (y - block.shape.getSize().y) / 2);
        window.draw(block.shape);
    }
}