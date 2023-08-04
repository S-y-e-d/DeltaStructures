#ifndef Interface_hpp
#define Interface_hpp

#include <SFML/Graphics.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include <variant>
#include <vector>

namespace ui {

    class Button {
        sf::RectangleShape rect;
        sf::Text text;
        sf::RenderWindow &window;
        sf::Font &font;
        bool pressed = false;
        bool mouse_over = false;

    public:
        Button(sf::RenderWindow &window, sf::Font &font);
        
        void setPosition(int x, int y);
        void setText(std::string txt, int font_size, sf::Color text_color = sf::Color::Black);
        sf::RectangleShape& getRect();

        bool mouseOver();
        bool mouseOut();
        bool clicked();
        void update(float mouseX, float mouseY, char key);
    };

    class Container {
        sf::RectangleShape rect;
        sf::RenderWindow &window;
        sf::Font &font;

    public:
        Container(sf::RenderWindow &window, sf::Font &font);
    };
};

#endif