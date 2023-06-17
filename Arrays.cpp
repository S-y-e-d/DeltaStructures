#include "Structures.hpp"

// function to add a new block to the array
void Array::addBlock(int value)
{
    Block block;
    block.shape = sf::RectangleShape(sf::Vector2f(block_size, block_size));
    block.shape.setFillColor(sf::Color::Transparent);
    block.shape.setOutlineThickness(-2.0);

    block.index.setFont(font);
    block.index.setCharacterSize(25);

    block.num_text.setFont(font);
    block.num_text.setString(std::to_string(value));
    block.num_text.setCharacterSize(value_char_size);

    block.value = value;

    array.push_back(block);
    change_made = true;
}

Array::Array(sf::RenderWindow &window, sf::Font &font) : window(window), font(font)
{
    window_size = {window.getSize().x, window.getSize().y};

    // block add/remove are the two ends of the array that add/remove blocks from it
    block_add = sf::RectangleShape(sf::Vector2f(block_size / 3, block_size));
    block_add.setFillColor(sf::Color::Cyan);
    block_add.setOutlineThickness(-2.0);
    block_remove = sf::RectangleShape(sf::Vector2f(block_size / 3, block_size));
    block_remove.setFillColor(sf::Color::Magenta);
    block_remove.setOutlineThickness(-2.0);

    // The variable that forces the entire array structure to be updated. Did this so don't have to constantly update and recalculate all positions every frame.
    change_made = true;
}

void Array::removeBlock()
{
    array.pop_back();
    change_made = true;
}

// function to adjust the size of the individual blocks.
void Array::setBlockSize(int size)
{
    block_size = size;
    for (auto &block : array)
    {
        block.shape.setSize(sf::Vector2f(size, size));
    }
    change_made = true;
}

void Array::handleClick(float mouseX, float mouseY)
{
    for (auto &block : array)
    {
        // if mouse is clicked inside array block
        if (block.shape.getGlobalBounds().contains(mouseX, mouseY))
        {
            // if a previous block was being edited
            if (value_edit_index != -1)
            {
                array[value_edit_index].num_text.setFillColor(sf::Color::White);
            }
            // set the clicked block as the block to be edited
            std::string index = block.index.getString();
            value_edit_index = std::stoi(index);
            block.num_text.setFillColor(sf::Color::Yellow);
            setValue(block, 0);
            change_made = true; // -----------> could potentiall remove this if test goes well
            return;
        }
    }
    // If the click was not in a block, reset the edit block selected if any
    if (value_edit_index != -1)
    {
        array[value_edit_index].num_text.setFillColor(sf::Color::White);
        value_edit_index = -1;
        change_made = true;
    }
    // detect click on adding and removing blocks
    if (block_add.getGlobalBounds().contains(mouseX, mouseY))
    {
        addBlock(0);
        change_made = true;
    }
    else if (block_remove.getGlobalBounds().contains(mouseX, mouseY))
    {
        removeBlock();
        change_made = true;
    }
}

// Set the numerical value of the block
void Array::setValue(Block &block, int value)
{
    int padding = 10;
    // The default value of value is set to INT32_MIN. This was done to update the block characters without changing the value, in case of a size change only.
    if (value != INT32_MIN)
    {
        block.num_text.setString(std::to_string(value));
        block.value = value;
    }

    // while the characters are smaller than the optimal size, increase the char size
    while (block.num_text.getGlobalBounds().width < block_size - 2 * padding and block.num_text.getCharacterSize() <= value_char_size)
    {
        float charsize = block.num_text.getCharacterSize();
        block.num_text.setCharacterSize(charsize + 1);
    }

    // while the characters are larger, i.e don't fit within the box, decrease the block size
    while (block.num_text.getGlobalBounds().width > block_size - 2 * padding)
    {
        float charsize = block.num_text.getCharacterSize();
        block.num_text.setCharacterSize(charsize - 1);
    }

    auto value_bounds = block.num_text.getGlobalBounds();

    float value_width = value_bounds.width;
    // This is to obtain the empty space between bounds and actual position. Else the text may not align to the bottom properly.
    float value_height = value_bounds.top + value_bounds.height - block.num_text.getPosition().y;
    // Set the position of text as right aligned and bottom aligned.
    int xpos = block.shape.getPosition().x + (block_size - value_width) - padding;
    int ypos = block.shape.getPosition().y + (block_size - value_height) - padding;
    block.num_text.setPosition(xpos, ypos);
}

void Array::randomize()
{
    srand(time(0));
    std::vector<int> temp_arr(array.size());
    for (int i = 0; i < temp_arr.size(); i++)
    {
        temp_arr[i] = i + 1;
    }
    for (int i = 0; i < temp_arr.size(); i++)
    {
        std::swap(temp_arr[i], temp_arr[rand() % temp_arr.size()]);
    }
    for (int i = 0; i < array.size(); i++)
    {
        setValue(array[i], temp_arr[i]);
    }
    change_made = true;
}

void Array::handleKeypress(char key)
{
    // If editing the value of a block
    if (value_edit_index != -1)
    {
        Block &block = array[value_edit_index];
        // If pressed a number
        if (isdigit(key))
        {
            if (block.num_text.getCharacterSize() <= 18)
            {
                return;
            }
            if (block.num_text.getString() == "0")
            {
                block.num_text.setString("");
            }
            setValue(block, stoi(std::string(block.num_text.getString() + key)));
        }
        else if (key == '\b')
        {
            std::string str = block.num_text.getString();
            if (str.size() > 1)
            {
                str.pop_back();
            }
            else
            {
                str = "0";
            }
            setValue(block, stoi(str));
        }
    }
}

void Array::insertAt(int val, int index, bool overload)
{
    using namespace std::chrono_literals;
    if (in_progress)
        return;
    in_progress = true;

    // Set the position to be inserted at as green
    array[index].num_text.setFillColor(sf::Color::Green);
    for (int i = array.size() - 1; i > index; i--)
    {
        // Create a temporary text of the previous number, to animate it moving to the right
        temp_drawables.push_back(array[i - 1].num_text);
        sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
        temp_text.setFillColor(sf::Color::White);
        for (int i = 0; i <= block_size; i++)
        {
            temp_text.move(1, 0);
            std::this_thread::sleep_for(10ms);
        }
        temp_drawables.pop_back();
        setValue(array[i], array[i - 1].value);
    }
    // create a temporary number to animate the actual number to be inserted dropping down from top on it's place
    temp_drawables.push_back(array[index].num_text);
    sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
    temp_text.setString(std::to_string(val));
    temp_text.move(0, -100);

    for (int i = 0; i < 100; i++)
    {
        temp_text.move(0, 1);
        std::this_thread::sleep_for(10ms);
    }
    temp_drawables.pop_back();
    setValue(array[index], val);
    array[index].num_text.setFillColor(sf::Color::White);
    in_progress = false;
}

void Array::insertAt(int val, int index)
{
    void (Array::*func)(int, int, bool) = &Array::insertAt;
    std::thread(func, this, val, index, true).detach();
}

void Array::remove(int index, bool overload)
{
    using namespace std::chrono_literals;

    if (in_progress)
        return;
    in_progress = true;

    // Set the number to be deleted as red
    array[index].num_text.setFillColor(sf::Color::Red);

    for (int i = index; i < array.size() - 1; i++)
    {
        // animate moving the next number to the left and overriding the current number
        temp_drawables.push_back(array[i + 1].num_text);
        sf::Text &temp_text = std::get<sf::Text>(temp_drawables[0]);
        for (int j = 0; j <= block_size; j++)
        {
            temp_text.move(-1, 0);
            std::this_thread::sleep_for(10ms);
        }
        temp_drawables.pop_back();
        setValue(array[i], array[i + 1].value);
        array[i].num_text.setFillColor(array[i + 1].num_text.getFillColor());
        std::this_thread::sleep_for(0.5s);
    }
    // Set the final value to 0. Since a deleted element still exists in memory, and is just set to override upon the next insert
    // the final element is set to 0 to visualise that it's not accessible in the current array of decreased size
    setValue(array.back(), 0);
    in_progress = false;
}

void Array::remove(int index)
{
    void (Array::*func)(int, bool) = &Array::remove;
    std::thread(func, this, index, true).detach();
}

void Array::swapBlocks(int i, int j)
{
    // swap two blocks within 100 steps with 10ms interval to get a time of around 1 second.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    if (i > j)
        std::swap(i, j);
    sf::Text &num_i = array[i].num_text;
    sf::Text &num_j = array[j].num_text;
    float distance = (j - i) * block_size;
    float velocity = distance / 100;
    for (int i = 0; i < 100; i++)
    {
        num_i.move(velocity, 0);
        num_j.move(-velocity, 0);
        std::this_thread::sleep_for(10ms);
    }
    std::swap(array[i], array[j]);
}

void Array::selectionSort()
{
    using namespace std::chrono_literals;
    if (in_progress)
        return;
    in_progress = true;
    // Circles representing i and j pointers
    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);
    iptr.setPosition(array[0].shape.getPosition().x - block_size / 2, array[0].shape.getPosition().y - 20);

    for (int i = 0; i < array.size() - 1; i++)
    {
        iptr.move(block_size, 0);
        jptr.setPosition(iptr.getPosition());

        for (int j = i + 1; j < array.size(); j++)
        {
            jptr.move(block_size, 0);

            if (array[i].value > array[j].value)
            {
                swapBlocks(i, j);
            }
            std::this_thread::sleep_for(1s);
        }
    }
    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::bubbleSort()
{
    using namespace std::chrono_literals;

    if (in_progress)
        return;
    in_progress = true;

    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);

    for (int i = 0; i < array.size() - 1; i++)
    {
        iptr.setPosition(array[0].shape.getPosition().x - block_size / 2, array[0].shape.getPosition().y - 20);
        std::get<sf::CircleShape>(temp_drawables[1]).setPosition(iptr.getPosition());
        std::get<sf::CircleShape>(temp_drawables[1]).move(block_size, 0);

        for (int j = 0; j < array.size() - i - 1; j++)
        {
            iptr.move(block_size, 0);
            std::get<sf::CircleShape>(temp_drawables[1]).move(block_size, 0);

            if (array[j].value > array[j + 1].value)
            {
                swapBlocks(j, j + 1);
            }
            std::this_thread::sleep_for(1s);
        }
    }
    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::insertionSort()
{
    using namespace std::chrono_literals;

    if (in_progress)
        return;
    in_progress = true;

    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));

    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);

    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);

    for (int i = 1; i < array.size(); i++)
    {
        iptr.setPosition(array[i].shape.getPosition().x + block_size / 2, array[i].shape.getPosition().y - 20);
        jptr.setPosition(iptr.getPosition());
        jptr.move(-block_size, 0);
        int j = i;
        while (j >= 1 and array[j].value < array[j - 1].value)
        {
            swapBlocks(j, j - 1);
            j--;
            jptr.move(-block_size, 0);
        }
        std::this_thread::sleep_for(1s);
    }

    temp_drawables.clear();
    change_made = true;
    in_progress = false;
}

void Array::moveBlock(int index, sf::Vector2f to)
{
    using namespace std::chrono_literals;
    Block &block = array[index];
    sf::Vector2f displacement(to.x - block.shape.getPosition().x, to.y - block.shape.getPosition().y);
    sf::Vector2f velocity(displacement.x / 100, displacement.y / 100);
    for (int i = 0; i < 100; i++)
    {
        block.shape.move(velocity);
        block.num_text.move(velocity);
        block.index.move(velocity);
        std::this_thread::sleep_for(10ms);
    }
}

void Array::mergeSort()
{
    using namespace std::chrono_literals;

    if (in_progress)
        return;
    in_progress = true;
    std::vector<std::vector<int>> states;

    // find the depth to calculate the step height of blocks to and position blocks evenly
    int depth = ceil(log2(array.size()));
    float max_height = window_size.y * 0.1;
    float min_height = array[0].shape.getPosition().y;
    float step_size_y = (min_height - max_height) / depth;
    float start_y = max_height;

    // i and j pointers
    temp_drawables.push_back(sf::CircleShape(5));
    temp_drawables.push_back(sf::CircleShape(5));
    sf::CircleShape &iptr = std::get<sf::CircleShape>(temp_drawables[0]);
    sf::CircleShape &jptr = std::get<sf::CircleShape>(temp_drawables[1]);
    iptr.setFillColor(sf::Color::Green);
    jptr.setFillColor(sf::Color::Blue);

    // fill a tempoarary array of indices.
    // Instead of swapping the real array, I have swapped an array of idices. This was to ensure i don't have to copy the real array to work with. Could be improved.
    for (int i = 0; i < array.size(); i++)
    {
        std::vector<int> temp;
        temp.push_back(i);
        states.push_back(temp);
    }

    hide_index = true;
    float max_width = array.size() * block_size + (states.size() - 1) * block_size;
    float start_x = (window_size.x - max_width) / 2;

    // move array blocks to the highest position to start sorting.
    for (auto &arr : states)
    {
        for (int &idx : arr)
        {
            std::thread(&Array::moveBlock, this, idx, sf::Vector2f(start_x, start_y)).detach();
            start_x += block_size;
        }
        start_x += block_size;
    }
    start_y += step_size_y;
    std::this_thread::sleep_for(1s);
    // This is a badly optimized iterative merge sort I came up with.
    // This sort works on a breadth-first basis and thus makes easier to animate rather than the usual depth-first approach of the recursive merge sort.
    while (states.size() > 1)
    {
        max_width = array.size() * block_size + (ceil(states.size() / 2.0) - 1) * block_size;
        start_x = (window_size.x - max_width) / 2;
        for (int it = 0; it < states.size(); it++)
        {
            // Handle the singleton left with no other to compare with
            if (it == states.size() - 1)
            {
                for (auto i : states[it])
                {
                    moveBlock(i, sf::Vector2f(start_x, start_y));
                }
                continue;
            }
            std::vector<int> temp;
            int i = 0;
            int j = 0;
            iptr.setPosition(array[states[it][i]].shape.getPosition() + sf::Vector2f(block_size / 2 - 5, -20));
            jptr.setPosition(array[states[it + 1][j]].shape.getPosition() + sf::Vector2f(block_size / 2 - 5, -20));

            std::this_thread::sleep_for(1s);
            // copy the array in array of arrays and merge the first two, delete them and reinsert the merged array. Repeat until there is only one array left.
            while (i < states[it].size() and j < states[it + 1].size())
            {
                if (array[states[it][i]].value <= array[states[it + 1][j]].value)
                {
                    moveBlock(states[it][i], sf::Vector2f(start_x, start_y));
                    start_x += block_size;
                    temp.push_back(states[it][i++]);
                    iptr.move(block_size, 0);
                    if (i == states[it].size())
                    {
                        iptr.setPosition(-1, -1);
                    }
                    std::this_thread::sleep_for(0.5s);
                }
                else
                {
                    moveBlock(states[it + 1][j], sf::Vector2f(start_x, start_y));
                    start_x += block_size;
                    temp.push_back(states[it + 1][j++]);
                    jptr.move(block_size, 0);
                    if (j == states[it + 1].size())
                    {
                        jptr.setPosition(-1, -1);
                    }
                    std::this_thread::sleep_for(0.5s);
                }
            }
            while (i < states[it].size())
            {
                moveBlock(states[it][i], sf::Vector2f(start_x, start_y));
                start_x += block_size;
                temp.push_back(states[it][i++]);
                iptr.move(block_size, 0);
                if (i == states[it].size())
                {
                    iptr.setPosition(-1, -1);
                }
                std::this_thread::sleep_for(0.5s);
            }
            while (j < states[it + 1].size())
            {
                moveBlock(states[it + 1][j], sf::Vector2f(start_x, start_y));
                start_x += block_size;
                temp.push_back(states[it + 1][j++]);
                jptr.move(block_size, 0);
                if (j == states[it + 1].size())
                {
                    jptr.setPosition(-1, -1);
                }
                std::this_thread::sleep_for(0.5s);
            }
            start_x += block_size;
            states.erase(states.begin() + it);
            states[it] = temp;
        }
        start_y += step_size_y;
    }
    // Since there was no sorting of the real array, just sort the real array somehow and update it.
    // Could've done this better
    std::sort(array.begin(), array.end(), [](Block a, Block b)
              { return a.value < b.value; });
    temp_drawables.clear();
    hide_index = false;
    change_made = true;
    in_progress = false;
}

// The function that is callable from outside, calls sort threads.
void Array::sort(SortType type)
{
    for (auto &block : array)
    {
        block.shape.setOutlineColor(sf::Color::White);
    }
    switch (type)
    {
    case SortType::SELECTION:
        std::thread(&Array::selectionSort, this).detach();
        return;
    case SortType::BUBBLE:
        std::thread(&Array::bubbleSort, this).detach();
        return;
    case SortType::INSERTION:
        std::thread(&Array::insertionSort, this).detach();
        return;
    case SortType::MERGE:
        std::thread(&Array::mergeSort, this).detach();
        return;
    default:
        return;
    }
}

void Array::makeGradient()
{
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(100ms);
    int r = 255, g = 0, b = 255;
    int gradient = (array.size() > 1) ? array.size() - 1 : 1;
    for (auto &block : array)
    {
        block.shape.setOutlineColor(sf::Color(r, g, b, 255));
        r -= 255 / gradient;
        g += 255 / gradient;
        std::this_thread::sleep_for(50ms);
    }
}

void Array::update()
{
    if (change_made)
    {
        // Reset positions of everything by recalculating the middle of the page.
        int x = (window_size.x - block_size * array.size()) / 2;
        int y = (window_size.y - block_size) / 2;
        int i = 0;
        block_remove.setPosition(x - block_size / 3, y);

        int padding = 10;
        for (auto &block : array)
        {
            block.shape.setPosition(x + block_size * i, y);
            block.index.setString(std::to_string(i));
            block.index.setPosition(x + block_size * i + padding, y);
            setValue(block);
            i++;
        }
        block_add.setPosition(x + block_size * i, y);
        std::thread(&Array::makeGradient, this).detach();
        change_made = false;
    }
    window.draw(block_add);
    window.draw(block_remove);
    for (auto &block : array)
    {
        window.draw(block.shape);
        if (not hide_index)
            window.draw(block.index);
        window.draw(block.num_text);
    }
    // Draw temporary blocks that were used to animate.
    for (auto &temp : temp_drawables)
    {
        std::visit([&](auto &&obj)
                   { window.draw(obj); },
                   temp);
    }
}