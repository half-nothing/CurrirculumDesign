#pragma once
#ifndef TEACHER_MENU_HANDLER_H
#define TEACHER_MENU_HANDLER_H

#include "../basics/MenuBase.h"
    #include "../manager/TeacherManager.h"

class TeacherMenuHandler : public MenuBase, public SingletonPatternBasics<TeacherMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        TeacherManager::getInstance()->saveData();
    }

private:
    explicit TeacherMenuHandler() :
            MenuBase("��ʦ����", {"1.��ʦ�б�", "2.��ӽ�ʦ", "3.ɾ����ʦ", "4.�޸Ľ�ʦ", "5.���ҽ�ʦ", "6.��������", "7.��������", "8.�������",
                                  "9.������ҳ"}) {}

    ~TeacherMenuHandler() = default;

    TeacherMenuHandler(const TeacherMenuHandler &studentMenu) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                TeacherManager::getInstance()->listHandler();
                break;
            case 2:
                TeacherManager::getInstance()->addHandler();
                break;
            case 3:
                TeacherManager::getInstance()->delHandler();
                break;
            case 4:
                TeacherManager::getInstance()->changeHandler();
                break;
            case 5:
                TeacherManager::getInstance()->searchHandler();
                break;
            case 6:
                TeacherManager::getInstance()->batchImportHandler();
                break;
            case 7:
                TeacherManager::getInstance()->saveData();
                break;
            case 8:
                TeacherManager::getInstance()->clearHandler();
                break;
            case 9:
                return true;
            default:
                break;
        }
        return false;
    }
};

#endif
