#include "Panels.hpp"
#include "Structures.hpp"

// refactor panels to use threads and use move instead of set positions.

int main() {

    srand(time(0));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8.0;
    sf::Vector2i window_size(1920, 1080);

    sf::RenderWindow window(sf::VideoMode(window_size.x, window_size.y), "Delta-Structures");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("./assets/DejaVuSansMono.ttf")) {
        std::cout << "Error: Couldn't load font\n";
        return 0;
    }

    sf::Vector2f leftPanelPos(-130, 0);
    Panel lpanel(sf::Vector2f(150, window_size.y), sf::Vector2f(-130, 0));
    lpanel.getPanel().setFillColor(sf::Color::White);
    lpanel.setStates(sf::Vector2f(-130, 0), sf::Vector2f(0, 0));

    lpanel.addComponent(10, 3, sf::Color::Black, State{sf::Vector2f(137, window_size.y / 2 - 5), 90}, State{sf::Vector2f(137, window_size.y / 2 - 5), 270});
    // Array array(window, font);
    // structures::LinkedList list(window, font);
    structures::Tree tree(window, font);

    while (window.isOpen()) {
        sf::Event event;
        int mouse_click_x = 0, mouse_click_y = 0;
        char key_pressed = '\0';
        while (window.pollEvent(event)) {
            
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::TextEntered) {
                key_pressed = event.text.unicode;
            }
            // Detecting keyboard input
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                window.close();
            }
            // Triggering test events

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::T) {
                    // array.sort(SortType::MERGE);
                    // list.deleteNode(1);
                    // list.insertAfter(7, 2);
                }
                if (event.key.code == sf::Keyboard::R) {
                    // array.randomize();
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Convert the position of the pixel to the relative coordinate of the original window
                    lpanel.handleClick(worldPos.x, worldPos.y);
                    mouse_click_x = worldPos.x;
                    mouse_click_y = worldPos.y;
                }else{
                    // Using negative to show right click. Surely nothing can go wrong.
                    mouse_click_x = -worldPos.x;
                    mouse_click_y = -worldPos.y;
                }
            }
        }
        window.clear();
        // array.update(mouse_click_x, mouse_click_y, key_pressed);
        // list.update(mouse_click_x, mouse_click_y, key_pressed);
        tree.update(mouse_click_x, mouse_click_y, key_pressed);
        lpanel.update(window);
        window.display();
    }

    return 0;
}
