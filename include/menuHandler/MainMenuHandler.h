/**
 * @author Half_nothing
 * @date 2023.02
 * @details ����ʵ��Ŀ¼����
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
    explicit MainMenuHandler() : MenuBase(" ���˵� ", {"1.ѧ����¼", "2.�����̨", "3.�˳�ϵͳ"}) {}

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
                    std::cout << "��½�ɹ�����ӭ��������Ա" << std::endl;
                    PAUSE;
                    CLEAR;
                    AdminMenuHandler::getInstance()->menuHandler();
                } else {
                    std::cout << "�˺Ż��������" << std::endl;
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
