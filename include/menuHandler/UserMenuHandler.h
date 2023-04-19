/**
 * @author Half_nothing
 * @date 2023.02
 * @details ����ʵ��Ŀ¼����
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
    explicit UserMenuHandler() : MenuBase("ѧ���˵�",{"1.������Ϣ", "2.�޶���Ϣ", "3.��ʼ����", "4.�޸�����", "5.�˳���¼"}) {}

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
