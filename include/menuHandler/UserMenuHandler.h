/**
 * @author Half_nothing
 * @date 2023.02
 * @details 用于实现目录的类
 */

#pragma once
#ifndef User_MENU_HANDLER_H
#define User_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/UserManager.h"

class UserMenuHandler : public MenuBase, public SingletonPatternBasics<UserMenuHandler> {
    friend class SingletonPatternBasics;
public:
    UserMenuHandler(UserMenuHandler &) = delete;
    UserMenuHandler(UserMenuHandler &&) = delete;
    UserMenuHandler &operator=(const UserMenuHandler &) = delete;
private:
    explicit UserMenuHandler() : MenuBase("学生菜单",{"1.个人信息", "2.修读信息", "3.开始评价", "4.修改密码", "5.退出登录"}) {}

    ~UserMenuHandler() = default;

    bool choiceHandler() override {
        using namespace std;
        switch (this->line) {
            case 1:
                UserManager::getInstance()->showInfo();
                break;
            case 2:
                UserManager::getInstance()->checkStudentCourseInfo();
                break;
            case 3:
                UserManager::getInstance()->startEvaluation();
                break;
            case 4:
                UserManager::getInstance()->passwordChanger();
                break;
            case 5:
                return true;
            default:
                break;
        }
        return false;
    }

};

#endif
