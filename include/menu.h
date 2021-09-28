#pragma once

#include "field.h"
#include "logic.h"

#include <string>
#include <memory>

class Menu
{
public:
    Menu (const Menu&) = delete;
    Menu& operator = (const Menu&) = delete;
    static Menu* getInstance();

    void runInvitation();
private:
    enum Option {START, RESTART, EXIT, INPUT};
private:
    Menu ();
    static Menu* instance;

    void runGameLoop();
    void printMenuOption(const int i, const std::string& option) const;
    void printStatus() const;
    char selectOption(const char left, const char right) const;
    void doOption(const Option option);
private:
    std::unique_ptr<Field> field {nullptr};
    std::unique_ptr<Logic> logic {nullptr};
};
