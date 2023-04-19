#pragma once
#ifndef STUDENT_COURSE_MENU_HANDLER_H
#define STUDENT_COURSE_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/StudentCourseManager.h"

class StudentCourseMenuHandler : public MenuBase, public SingletonPatternBasics<StudentCourseMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        StudentCourseManager::getInstance()->saveData();
    }

private:
    explicit StudentCourseMenuHandler() :
            MenuBase("�޶���Ϣ",
                     {"1.�޶��б�",
                      "2.����޶�",
                      "3.ɾ���޶�",
                      "4.��ѯ�޶�",
                      "5.�޸��޶�",
                      "6.��������",
                      "7.��������",
                      "8.�������",
                      "9.������ҳ"}) {}

    ~StudentCourseMenuHandler() = default;

    StudentCourseMenuHandler(const StudentCourseMenuHandler &studentCourse) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                StudentCourseManager::getInstance()->listHandler();
                break;
            case 2:
                StudentCourseManager::getInstance()->addHandler();
                break;
            case 3:
                StudentCourseManager::getInstance()->delHandler();
                break;
            case 4:
                StudentCourseManager::getInstance()->searchHandler();
                break;
            case 5:
                StudentCourseManager::getInstance()->changeHandler();
                break;
            case 6:
                StudentCourseManager::getInstance()->batchImportHandler();
                break;
            case 7:
                StudentCourseManager::getInstance()->saveData();
                break;
            case 8:
                StudentCourseManager::getInstance()->clearHandler();
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