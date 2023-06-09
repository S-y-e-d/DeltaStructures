#include "Panels.hpp"
#include "Structures.hpp"


int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    sf::Vector2i window_size(1920, 1080);

    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Delta-Structures");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./assets/DejaVuSansMono.ttf")){
        std::cout << "Error: Couldn't load font\n";
        return 0;
    }

    sf::Vector2f leftPanelPos(-130, 0);
    Panel lpanel(sf::Vector2f(150, window_size.y), sf::Vector2f(-130, 0));
    lpanel.getPanel().setFillColor(sf::Color::White);
    lpanel.setStates(sf::Vector2f(-130, 0), sf::Vector2f(0, 0));

    lpanel.addComponent(10, 3, sf::Color::Black, State{sf::Vector2f(137, window_size.y/2-5), 90}, State{sf::Vector2f(137, window_size.y/2-5), 270});
    Array array(window, font);
    array.addBlock(12);
    array.addBlock(123);
    array.addBlock(12345);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }else if(event.type == sf::Event::Resized){
                window.setSize(sf::Vector2u(event.size.width, event.size.height));
                // array.handleResize(); // Needs implementation
            }else if(event.type == sf::Event::TextEntered) {
                array.handleKeypress(event.text.unicode);
            }
            // Detecting keyboard input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
            // Triggering test events
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
                // array.setBlockSize(50);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Convert the position of the pixel to the relative coordinate of the original window
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    lpanel.handleClick(worldPos.x, worldPos.y);
                    array.handleClick(worldPos.x, worldPos.y);
                }
            }
        }
        window.clear();
        array.update();
        lpanel.update(window);
        window.display();
    }

    return 0;
}
