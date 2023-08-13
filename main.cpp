#include "Panels.hpp"
#include "Interface.hpp"
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
    if (!font.loadFromFile("./assets/ARIAL.TTF")) {
        std::cout << "Error: Couldn't load font\n";
        return 0;
    }

    
    structures::Array array(window, font);
    structures::LinkedList list(window, font);
    structures::Tree tree(window, font);

    enum ActiveStructure {
        ARRAY,
        LIST,
        TREE,
        NONE
    } active_structure = NONE;

    ui::Container lpanel(window, font);
    lpanel.setSize(window_size.x/4, window_size.y);
    lpanel.setFillColor(sf::Color(10, 10, 10, 255));
    lpanel.setOutlineColor(sf::Color::White);
    lpanel.setOutlineThickness(-2);
    lpanel.setPosition(-window_size.x/4 + 70, 0);       // panel peaking to show button (50) + 10 padding each side
    
    lpanel.addComponent(ui::Container::ComponentType::BUTTON, "btn1", window_size.x/4 - 60, 10, 50, 50);
    lpanel.getButtonById("btn1").setOutlineThickness(-2);
    lpanel.getButtonById("btn1").setOutlineColor(sf::Color::White);
    lpanel.getButtonById("btn1").setFillColor(sf::Color::Transparent);
    lpanel.getButtonById("btn1").setText("≡", 18, sf::Color::White);
    lpanel.setState(0);
    


    // ui::Input inp(window, font);
    // inp.setSize(500, 50);
    // inp.setOutlineThickness(-1);
    // inp.setOutlineColor(sf::Color::White);
    // inp.setFillColor(sf::Color(50, 50, 50, 255));
    // inp.setPosition(200, 200);
    // inp.setTextColor(sf::Color::White);
    // inp.setPlaceholder("enter text...", sf::Color(100, 100, 100, 255));

    // delete this and references
    

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
                // if (event.key.code == sf::Keyboard::A) {
                //     active_structure = ARRAY;
                // }
                // if (event.key.code == sf::Keyboard::L) {
                //     active_structure = LIST;
                // }
                // if (event.key.code == sf::Keyboard::T) {
                //     active_structure = TREE;
                // }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Convert the position of the pixel to the relative coordinate of the original window
                    mouse_click_x = worldPos.x;
                    mouse_click_y = worldPos.y;
                } else {
                    // Using negative to show right click. Surely nothing can go wrong.
                    mouse_click_x = -worldPos.x;
                    mouse_click_y = -worldPos.y;
                }
            }
        }
        window.clear();

        switch (active_structure) {
        case ARRAY:
            array.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        case LIST:
            list.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        case TREE:
            tree.update(mouse_click_x, mouse_click_y, key_pressed);
            break;
        default:
            break;
        }

        if(lpanel.getButtonById("btn1").mouseOver()){
            lpanel.getButtonById("btn1").setOutlineThickness(2);
        }
        if(lpanel.getButtonById("btn1").mouseOut()){
            lpanel.getButtonById("btn1").setOutlineThickness(-2);
        }
        
        if(lpanel.getButtonById("btn1").clicked()){
            lpanel.getButtonById("btn1").setOutlineThickness(-2);
            
            if(lpanel.getState()== 0){
                lpanel.setState(1);
                lpanel.move(lpanel.getSize().x - 70, 0, 2);     // 70 being the button size
            }else if(lpanel.getState() == 1){
                lpanel.setState(0);
                lpanel.move(-lpanel.getSize().x + 70, 0, 2);
            }
            
        }
        lpanel.update(mouse_click_x, mouse_click_y, key_pressed);
        // inp.update(mouse_click_x, mouse_click_y, key_pressed);
        window.display();
    }

    return 0;
}