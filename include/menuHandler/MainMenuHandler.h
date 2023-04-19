/**
 * @author Half_nothing
 * @date 2023.02
 * @details 用于实现目录的类
 */

#pragma once
#ifndef MENU_HANDLER_H
#define MENU_HANDLER_H

#include "AdminMenuHandler.h"
#include "UserMenuHandler.h"

class MainMenuHandler : public MenuBase, public SingletonPatternBasics<MainMenuHandler> {
    friend class SingletonPatternBasics;

public:
    MainMenuHandler(MainMenuHandler &) = delete;

    MainMenuHandler(MainMenuHandler &&) = delete;

    MainMenuHandler &operator=(const MainMenuHandler &) = delete;

private:
    explicit MainMenuHandler() : MenuBase(" 主菜单 ", {"1.学生登录", "2.管理后台", "3.退出系统"}) {}

    ~MainMenuHandler() = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                if (UserManager::getInstance()->loginHandler()) {
                    PAUSE;
                    CLEAR;
                    UserMenuHandler::getInstance()->menuHandler();
                } else {
                    PAUSE;
                }
                break;
            case 2:
                if (AdminMenuHandler::getInstance()->login()) {
                    std::cout << "登陆成功，欢迎您，管理员" << std::endl;
                    PAUSE;
                    CLEAR;
                    AdminMenuHandler::getInstance()->menuHandler();
                } else {
                    std::cout << "账号或密码错误" << std::endl;
                    PAUSE;
                    CLEAR;
                }
                break;
            case 3:
                AdminMenuHandler::saveALL();
                ::exit(0);
            default:
                break;
        }
        return false;
    }

};

#endif
