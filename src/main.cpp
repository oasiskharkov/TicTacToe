#include "menu.h"

#include <iostream>

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    try
    {
        std::unique_ptr<Menu> menu {Menu::getInstance()};
        if(menu)
        {
            menu->runInvitation();
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::system("clear");
    return EXIT_SUCCESS;
}
