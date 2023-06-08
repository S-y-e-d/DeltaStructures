#include "Panels.hpp"

Panel::Panel(sf::Vector2f size, sf::Vector2f pos) {
    this->pos = pos;
    panel.setSize(size);
    panel.setPosition(pos);
}
void Panel::handleClick(float mouseX, float mouseY) {
    if (moving) return;
    if(components[0].shape.getGlobalBounds().contains(mouseX, mouseY)){
        moving = true;
    }
}

void Panel::setStates(sf::Vector2f s1, sf::Vector2f s2) {
    state_1 = s1;
    state_2 = s2;
    velocity.x = (s2.x - s1.x) / 10;
    velocity.y = (s2.y - s1.y) / 10;
}

void Panel::update(sf::RenderWindow& window) {
    if (moving) {
        if (state == 1) {
            // If increasing true greater must be true. If increasing false, greater must be false.
            if ((velocity.x > 0) == (pos.x + velocity.x > state_2.x)) {
                pos.x = state_2.x;
            } else {
                pos.x += velocity.x;
            }
            if ((velocity.y > 0) == (pos.y + velocity.y > state_2.y)) {
                pos.y = state_2.y;
            } else {
                pos.y += velocity.y;
            }
            panel.setPosition(pos);
            for (auto& component : components) {
                component.shape.setPosition(component.state_1.pos.x + pos.x, component.state_1.pos.y + pos.y);
            }
            if (pos.x == state_2.x and pos.y == state_2.y) {
                for (auto& component : components) {
                    component.shape.setRotation(component.state_2.rotation);
                }
                moving = false;
                state = 2;
            }
        } else if (state == 2) {
            if ((velocity.x > 0) == (pos.x - velocity.x < state_1.x)) {
                pos.x = state_1.x;
            } else {
                pos.x -= velocity.x;
            }
            if ((velocity.y > 0) == (pos.y - velocity.y < state_1.y)) {
                pos.y = state_1.y;
            } else {
                pos.y -= velocity.y;
            }
            panel.setPosition(pos);
            for (auto& component : components) {
                component.shape.setPosition(component.state_2.pos.x + pos.x, component.state_2.pos.y + pos.y);
            }
            if (pos.x == state_1.x and pos.y == state_1.y) {
                for (auto& component : components) {
                    component.shape.setRotation(component.state_1.rotation);
                }
                moving = false;
                state = 1;
            }
        }
    }
    window.draw(panel);
    for (auto& component : components) {
        window.draw(component.shape);
    }
}

sf::RectangleShape& Panel::getPanel() {
    return panel;
}

// Components can be any regular polygon, and the position is relative to the parent panel.
void Panel::addComponent(int radius, int sides, sf::Color color, State state1, State state2) {
    sf::CircleShape shape(radius, sides);
    shape.setPosition(pos.x + state1.pos.x, pos.y + state1.pos.y);
    shape.setOrigin(radius, radius);
    shape.setFillColor(color);
    Component component;
    component.shape = shape;
    component.state_1 = state1;
    component.state_2 = state2;
    component.shape.setRotation(state1.rotation);
    components.push_back(component);
}

void Panel::addComponent(Component component){
    components.push_back(component);
}