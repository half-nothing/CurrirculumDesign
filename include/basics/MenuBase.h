/**
 * @author Half_nothing
 * @date 2023.02
 * @class MenuBase
 * @details 存放菜单的基类,其中实现了方向键操作的框架
 * @include "Definitions.h" iostream string vector
 */

#pragma once
#ifndef MENU_BASE_H
#define MENU_BASE_H

#include "Definitions.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// 菜单类基类
class MenuBase {
public:
    MenuBase() = default;

    explicit
    MenuBase(std::string title,
             std::vector<std::string> menuList)
            : title(std::move(title)), menuList(std::move(menuList)) {
        this->upper = (short) this->menuList.size();
    }

    explicit
    MenuBase(std::string title,
             std::vector<std::string> menuList,
             std::string space)
            : title(std::move(title)), menuList(std::move(menuList)), space(std::move(space)) {
        this->upper = (short) this->menuList.size();
    }

    ~MenuBase() = default;

    std::string get() {
        this->menuHandler();
        return this->menuList[this->line - this->lower];
    }

    uint get(bool){
        this->menuHandler();
        return this->line;
    }

    void menuHandler() {
        this->showMenu();
        this->init();
        while (true) {
            char temp;
            while ((temp = (char) getch()) != '\r') {
                if (temp != -32) {
                    continue;
                }
                temp = (char) getch();
                if (temp == DOWN_KEY) {
                    MenuBase::gotoPos(0, this->line);
                    IN_DEFAULT;
                    this->changeText(this->line);
                    if (this->line >= this->upper) {
                        this->line = this->lower;
                    } else {
                        this->line += 1;
                    }
                    MenuBase::gotoPos(0, this->line);
                    IN_CYAN;
                    this->changeText(this->line);
                }
                if (temp == UP_KEY) {
                    MenuBase::gotoPos(0, this->line);
                    IN_DEFAULT;
                    this->changeText(this->line);
                    if (this->line <= this->lower) {
                        this->line = this->upper;
                    } else {
                        this->line -= 1;
                    }
                    MenuBase::gotoPos(0, this->line);
                    IN_CYAN;
                    this->changeText(this->line);
                }
            }
            IN_DEFAULT;
            CLEAR;
            MenuBase::gotoPos(0, 0);
            if (this->choiceHandler()) {
                return;
            }
            CLEAR;
            this->showMenu();
            this->init();
        }
    }

protected:
    short lower = 1;
    short upper = 1;
    short line = 1;
    std::string space = "          ";
    std::vector<std::string> menuList;
    std::string title;

    virtual bool choiceHandler() {
        return true;
    };

private:
    void showMenu() {
        using namespace std;
        cout << "-----------" << this->title << "------------" << endl;
        for (const auto &str: this->menuList)
            cout << this->space << str << endl;
        cout << "------------------------------" << endl;
        cout << "使用↑和↓移动光标，回车键选择" << endl;
    };

    void changeText(int x) {
        std::cout << this->space << this->menuList[x - 1] << this->space;
    };

    void init() {
        this->line = 1;
        MenuBase::gotoPos(0, this->line);
        IN_CYAN;
        this->changeText(this->line);
        IN_DEFAULT;
    }

    static void gotoPos(short x, short y) {
        COORD pos = {x, y};
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);
    }
};

#endif
