#pragma once
#ifndef COURSE_MENU_HANDLER_H
#define COURSE_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/CourseManager.h"

class CourseMenuHandler : public MenuBase, public SingletonPatternBasics<CourseMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        CourseManager::getInstance()->saveData();
    }

private:
    explicit CourseMenuHandler() : MenuBase("�γ̹���",
                                            {"1.�γ��б�", "2.��ӿγ�", "3.ɾ���γ�", "4.�޸Ŀγ�", "5.�����γ�", "6.��������",
                                             "7.��������", "8.�������", "9.������ҳ"}) {}

    ~CourseMenuHandler() = default;

    CourseMenuHandler(const CourseMenuHandler &studentInfo) = default;

    // ��д����Ĵ���ѡ�������麯��
    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                CourseManager::getInstance()->listHandler();
                break;
            case 2:
                CourseManager::getInstance()->addHandler();
                break;
            case 3:
                CourseManager::getInstance()->delHandler();
                break;
            case 4:
                CourseManager::getInstance()->changeHandler();
                break;
            case 5:
                CourseManager::getInstance()->searchHandler();
                break;
            case 6:
                CourseManager::getInstance()->batchImportHandler();
                break;
            case 7:
                CourseManager::getInstance()->saveData();
                break;
            case 8:
                CourseManager::getInstance()->clearHandler();
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
