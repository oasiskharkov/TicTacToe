#include "menu.h"
#include "utils.h"

#include <vector>
#include <sstream>

using options = std::vector<std::string>;

static options ops = {
    "Start the game",
    "Restart the game",
    "Exit the game",
    "Input coordinates"
};

Menu* Menu::instance {nullptr};

Menu* Menu::getInstance()
{
    if(!instance)
    {
        instance = new Menu;
    }
    return instance;
}

Menu::Menu()
{
    field.reset(Field::getInstance());
    logic.reset(Logic::getInstance());

    if(!field or !logic)
    {
        throw std::runtime_error("Error! Incorrect game initialization!");
    }
}

void Menu::runInvitation()
{
    char choice {0};
    std::system("clear");
    std::cout << " **** TicTacToe ****" << std::endl;
    printMenuOption(1, ops[Option::START]);
    printMenuOption(2, ops[Option::EXIT]);

    choice = selectOption('1', '2');
    switch(choice)
    {
    case '1':
        doOption(Option::START);
        break;
    case '2':
        doOption(Option::EXIT);
        break;
    }
}

void Menu::runGameLoop()
{
    while(true)
    {
        char choice {0};
        field->print();
        if(logic->gameOver())
        {
            printStatus();
            printMenuOption(1, ops[Option::RESTART]);
            printMenuOption(2, ops[Option::EXIT]);
            choice = selectOption('1', '2');

            switch(choice)
            {
            case '1':
                doOption(Option::RESTART);
                continue;
            case '2':
                return doOption(Option::EXIT);
            };
        }
        else
        {
            printMenuOption(1, ops[Option::INPUT]);
            printMenuOption(2, ops[Option::RESTART]);
            printMenuOption(3, ops[Option::EXIT]);
            choice = selectOption('1', '3');

            switch(choice)
            {
            case '1':
                doOption(Option::INPUT);
                continue;
            case '2':
                doOption(Option::RESTART);
                continue;
            case '3':
                return doOption(Option::EXIT);
            };
        }
    }
}

void Menu::printMenuOption(const int i, const std::string& option) const
{
    std::cout << i << ". " << option << '.' << std::endl;
}

void Menu::printStatus() const
{
    switch (static_cast<int>(logic->gameStatus()))
    {
    case Logic::Status::DRAW:
        std::cout << " **** DRAW ****" << std::endl;
        break;
    case Logic::Status::LOSS:
        std::cout << " **** PLAYER LOSES ****" << std::endl;
        break;
    case Logic::Status::WIN:
        std::cout << " **** PLAYER WINS ****" << std::endl;
        break;
    }
}

char Menu::selectOption(const char left, const char right) const
{
    char choice {0};
    bool condition {true};
    do
    {
        std::stringstream ss;
        ss << "Choose option (" << left << '-' << right << "): ";
        std::cout << ss.str();

        choice = Utils::getch();

        condition = choice < left or choice > right;
        if(condition)
        {
            std::cout << "Uncorrect choice. Try again!" << std::endl;
        }
    } while(condition);
    return choice;
}

void Menu::doOption(const Option option)
{
    switch (static_cast<int>(option))
    {
    case Option::INPUT:
        logic->playerMove(field);
        logic->aiMove(field);
        break;
    case Option::RESTART:
        field->init();
        logic->setStatus(Logic::Status::PLAY);
        break;
    case Option::START:
        field->init();
        runGameLoop();
        break;
    case Option::EXIT:
        return;
    }
}
