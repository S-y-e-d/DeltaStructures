#ifndef Panels_hpp
#define Panels_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

struct State{
    sf::Vector2f pos;
    int rotation;
};
struct Component {
    sf::CircleShape shape;
    State state_1;
    State state_2;
};

class Panel {
    sf::RectangleShape panel;
    sf::Vector2f pos;
    sf::Vector2f state_1;
    sf::Vector2f state_2;
    sf::Vector2f velocity;
    bool moving = false;
    int state = 1;

    std::vector<Component> components;

public:
    Panel(sf::Vector2f size, sf::Vector2f pos);
    sf::RectangleShape& getPanel();
    bool clicked(float mouseX, float mouseY);
    void setStates(sf::Vector2f, sf::Vector2f);
    void move();
    void update(sf::RenderWindow& window);

    void addComponent(int radius, int sides, sf::Color, State state1, State state2);
    void addComponent(Component component);
};

#endif