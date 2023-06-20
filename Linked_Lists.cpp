#include "Structures.hpp"

void LinkedList::addNode(int value, int idx = -1)
{
    Node temp;
    temp.value_box = sf::RectangleShape(sf::Vector2f(node_size.value_width, node_size.height));
    temp.value_box.setFillColor(sf::Color::Transparent);
    temp.value_box.setOutlineThickness(-2);
    temp.ptr_box = sf::RectangleShape(sf::Vector2f(node_size.ptr_width, node_size.height));
    temp.ptr_box.setFillColor(sf::Color::Transparent);
    temp.ptr_box.setOutlineThickness(-2);

    temp.value = value;
    temp.ptr_dot = sf::CircleShape(5);
    temp.ptr_dot.setFillColor(sf::Color::Blue);
    temp.self_dot = sf::CircleShape(5);
    temp.self_dot.setFillColor(sf::Color::Red);
    temp.value_text.setFont(font);
    temp.value_text.setCharacterSize(36);
    temp.value_text.setString(std::to_string(value));

    if (idx == -1)
    {
        list.push_back(temp);
    }
    else
    {
        list.insert(list.begin() + idx, temp);
    }
    change_made = true;
}

LinkedList::LinkedList(sf::RenderWindow &window, sf::Font &font) : window(window), font(font)
{
    window_size = window.getSize();
    addNode(0);
    addNode(1);
    addNode(2);
    addNode(3);
}

void LinkedList::removeNode(int idx = -1)
{
    if (list.size() > 1)
    {
        if (idx == -1)
        {
            list.pop_back();
        }
        else
        {
            list.erase(list.begin() + idx);
        }
        change_made = true;
    }
}

void LinkedList::handleClick(float mouseX, float mouseY)
{
    for (int i = 0; i < list.size(); i++)
    {
        Node &node = list[i];
        if (node.ptr_dot.getGlobalBounds().contains(mouseX, mouseY))
        {
            addNode(0, i + 1);
            return;
        }
        if (node.self_dot.getGlobalBounds().contains(mouseX, mouseY))
        {
            removeNode(i);
            return;
        }
        if (node.value_box.getGlobalBounds().contains(mouseX, mouseY))
        {
            node.value_text.setFillColor(sf::Color::Yellow);
            setValue(node, 0);
            value_edit_index = i;
            return;
        }
        else
        {
            value_edit_index = -1;
            node.value_text.setFillColor(sf::Color::White);
        }
    }
}

void LinkedList::handleKeypress(char key)
{
    // If editing the value of a block
    if (value_edit_index != -1)
    {
        Node &node = list[value_edit_index];
        // If pressed a number
        if (isdigit(key))
        {
            if (node.value_text.getString() == "0")
            {
                node.value_text.setString("");
            }
            if (node.value_text.getCharacterSize() <= 18)
            {
                return;
            }
            setValue(node, stoi(std::string(node.value_text.getString() + key)));
        }
        else if (key == '\b')
        {
            std::string str = node.value_text.getString();
            if (str.size() > 1)
            {
                str.pop_back();
            }
            else
            {
                str = "0";
            }
            setValue(node, std::stoi(str));
        }
    }
}

void LinkedList::setValue(Node &node, int value = INT32_MIN)
{
    int padding = 10;
    // The default value of value is set to INT32_MIN. This was done to update the block characters without changing the value, in case of a size change only.
    if (value != INT32_MIN)
    {
        node.value_text.setString(std::to_string(value));
        node.value = value;
    }

    // while the characters are smaller than the optimal size, increase the char size
    while (node.value_text.getGlobalBounds().width < node_size.value_width - 2 * padding and node.value_text.getCharacterSize() <= value_char_size)
    {
        float charsize = node.value_text.getCharacterSize();
        node.value_text.setCharacterSize(charsize + 1);
    }

    // while the characters are larger, i.e don't fit within the box, decrease the block size
    while (node.value_text.getGlobalBounds().width > node_size.value_width - 2 * padding)
    {
        float charsize = node.value_text.getCharacterSize();
        if (charsize <= 19)
        {
            node.value_text.setCharacterSize(18);
            break;
        }
        std::cout << charsize << std::endl;
        node.value_text.setCharacterSize(charsize - 1);
    }

    auto value_bounds = node.value_text.getGlobalBounds();

    float text_width = value_bounds.width;
    // This is to obtain the empty space between bounds and actual position. Else the text may not align properly.
    float text_height = value_bounds.height;
    float empty_space_above_text = value_bounds.top - node.value_text.getPosition().y;
    // Set the position of text as right aligned and bottom aligned.
    int x_pos = node.value_box.getPosition().x + (node_size.value_width - text_width) / 2;
    int y_pos = node.value_box.getPosition().y + (node_size.height - text_height) / 2 - empty_space_above_text;
    node.value_text.setPosition(x_pos, y_pos);
}

void LinkedList::moveNode(Node &node, sf::Vector2f to, int vel, bool relative) // vel = 1, relative = false
{
    using namespace std::chrono_literals;

    if (relative)
    {
        to += node.value_box.getPosition();
    }

    sf::Vector2f displacement = to - node.value_box.getPosition();
    sf::Vector2f velocity(displacement.x / 100 * vel, displacement.y / 100 * vel);

    // Finding the before and after points to be moved to adjust connector lines
    int cnn_idx = 0;
    sf::Vector2f *connector_self = nullptr;
    sf::Vector2f *connector_ptr = nullptr;
    while (cnn_idx < connector_lines.size() and !node.ptr_box.getGlobalBounds().contains(connector_lines[cnn_idx].first.position))
    {
        cnn_idx++;
    }
    if (cnn_idx < connector_lines.size())
    {
        connector_ptr = &connector_lines[cnn_idx].first.position;
    }
    cnn_idx = 0;
    while (cnn_idx < connector_lines.size() and !node.value_box.getGlobalBounds().contains(connector_lines[cnn_idx].second.position))
    {
        cnn_idx++;
    }
    if (cnn_idx < connector_lines.size())
    {
        connector_self = &connector_lines[cnn_idx].second.position;
    }
    for (int i = 0; i < 100 / vel; i++)
    {
        node.ptr_box.move(velocity.x, velocity.y);
        node.ptr_dot.move(velocity.x, velocity.y);
        node.self_dot.move(velocity.x, velocity.y);
        node.value_box.move(velocity.x, velocity.y);
        node.value_text.move(velocity.x, velocity.y);
        if (connector_self != nullptr)
        {
            *connector_self += velocity;
        }
        if (connector_ptr != nullptr)
        {
            *connector_ptr += velocity;
        }
        std::this_thread::sleep_for(10ms);
    }
}

void LinkedList::insert(int value, int after, bool thread)
{
    using namespace std::chrono_literals;
    if (in_progress)
    {
        return;
    }
    in_progress = true;

    temp_nodes.push_back(list[0]);
    Node &temp_node = temp_nodes[0];
    temp_node.ptr_box.move(0, 100);
    temp_node.ptr_dot.move(0, 100);
    temp_node.self_dot.move(0, 100);
    temp_node.value_box.move(0, 100);
    temp_node.value_text.move(0, 100);
    setValue(temp_node, value);

    int i = 0;
    std::this_thread::sleep_for(1s);

    while (i < after)
    {
        i++;
        moveNode(temp_node, sf::Vector2f(node_size.width + node_size.ptr_width, 0), 2, true);
        std::this_thread::sleep_for(1s);
    }

    // drawing the connection lines
    if (after < list.size() - 1)
    {
        Node &next_node = list[after + 1];
        connector_lines.push_back({temp_node.ptr_dot.getPosition() + sf::Vector2f(5, 5),
                                   next_node.self_dot.getPosition() + sf::Vector2f(5, 5)});

        std::pair<sf::Vertex, sf::Vertex> &line = connector_lines.back();
        // make the connector line stretch to the position
        sf::Vector2f vel((line.second.position.x - line.first.position.x) / 50, (line.second.position.y - line.first.position.y) / 50);
        line.second.position = line.first.position;
        for (int i = 0; i < 50; i++)
        {
            line.second.position = line.second.position + sf::Vector2f(vel.x, vel.y);
            std::this_thread::sleep_for(5ms);
        }

        std::this_thread::sleep_for(1s);
    }
    if (after != -1)
    {
        Node &prev_node = list[after];

        sf::Vector2f vel((connector_lines[after].second.position.x - connector_lines[after].first.position.x) / 50,
                         (connector_lines[after].second.position.y - connector_lines[after].first.position.y) / 50);
        // Retract the already connected line
        for (int i = 0; i < 50; i++)
        {
            connector_lines[after].second.position -= vel;
            std::this_thread::sleep_for(5ms);
        }
        if(after < connector_lines.size()){
            connector_lines.erase(connector_lines.begin() + after);
        }

        connector_lines.push_back({prev_node.ptr_dot.getPosition() + sf::Vector2f(5, 5),
                                   temp_node.self_dot.getPosition() + sf::Vector2f(5, 5)});

        std::pair<sf::Vertex, sf::Vertex> &line = connector_lines.back();
        vel = {(line.second.position.x - line.first.position.x) / 50, (line.second.position.y - line.first.position.y) / 50};
        line.second.position = line.first.position;
        // Extend the line to connect
        for (int i = 0; i < 50; i++)
        {
            line.second.position += sf::Vector2f(vel.x, vel.y);
            std::this_thread::sleep_for(10ms);
        }

        std::this_thread::sleep_for(1s);
    }

    for (int i = 0; i <= after; i++)
    {
        moveNode(list[i], sf::Vector2f(-(node_size.width + node_size.ptr_width) / 2, 0), 4, true);
    }
    for (int i = after + 1; i < list.size(); i++)
    {
        moveNode(list[i], sf::Vector2f((node_size.width + node_size.ptr_width) / 2, 0), 4, true);
    }

    sf::Vector2f final_position = list[after].value_box.getPosition() + sf::Vector2f(node_size.width + node_size.ptr_width, 0);
    
    moveNode(temp_node, final_position, 2);

    if (after < list.size() - 1)
    {
        list.insert(list.begin() + after + 1, temp_node);
    }
    else
    {
        list.push_back(temp_node);
    }

    temp_nodes.clear();
    change_made = true;
    in_progress = false;
}

void LinkedList::insert(int value, int after)
{
    void (LinkedList::*func)(int, int, bool) = &LinkedList::insert;
    std::thread(func, this, value, after, true).detach();
}

void LinkedList::update()
{
    if (change_made)
    {
        float x_pos = window_size.x;                      // get the window width
        x_pos -= node_size.width * list.size();           // subtract the size of all the node blocks (value+ptr box)
        x_pos -= node_size.ptr_width * (list.size() - 1); // subtract the size of the gaps in between
        x_pos /= 2;                                       // divide by 2 to get the center point where the list should start
        float y_pos = ((float)(window_size.y - node_size.height)) / 2;
        connector_lines.clear(); // clear the connector lines to recalculate them.
        for (std::vector<Node>::iterator node = list.begin(); node != list.end(); node++)
        {
            node->value_box.setPosition(x_pos, y_pos);
            node->ptr_box.setPosition(x_pos + node_size.value_width, y_pos);
            node->self_dot.setPosition(x_pos - 5 + 1, y_pos + (node_size.height - 10) / 2); // compensate for the border thickness to draw circle on the line
            node->ptr_dot.setPosition(x_pos + node_size.value_width + (node_size.ptr_width - 10) / 2, y_pos + (node_size.height - 10) / 2);
            setValue(*node);
            if (node != list.begin())
            {
                std::pair<sf::Vertex, sf::Vertex> line = {
                    sf::Vertex((node - 1)->ptr_dot.getPosition() + sf::Vector2f(5, 5)),
                    sf::Vertex(node->self_dot.getPosition() + sf::Vector2f(5, 5))};
                connector_lines.push_back(line);
            }
            x_pos += node_size.width + node_size.ptr_width;
        }
        change_made = false;
    }
    for (auto &node : list)
    {
        window.draw(node.ptr_box);
        window.draw(node.value_box);
        window.draw(node.self_dot);
        window.draw(node.ptr_dot);
        window.draw(node.value_text);
    }
    for (auto &line : connector_lines)
    {
        sf::Vertex l[2] = {line.first, line.second};
        l[0].color = sf::Color::Blue;
        l[1].color = sf::Color::Red;
        window.draw(l, 2, sf::Lines);
    }

    for (auto &temp : temp_drawables)
    {
        std::visit([&](auto &&drawable)
                   { window.draw(drawable); },
                   temp);
    }

    for (auto &node : temp_nodes)
    {
        window.draw(node.ptr_box);
        window.draw(node.value_box);
        window.draw(node.self_dot);
        window.draw(node.ptr_dot);
        window.draw(node.value_text);
    }
}
