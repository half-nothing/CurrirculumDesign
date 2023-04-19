#pragma once
#ifndef STUDENT_MENU_HANDLER_H
#define STUDENT_MENU_HANDLER_H

#include "StudentInfoMenuHandler.h"
#include "StudentCourseMenuHandler.h"

class StudentMenuHandler : public MenuBase, public SingletonPatternBasics<StudentMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        StudentInfoMenuHandler::saveData();
        StudentCourseMenuHandler::saveData();
    }

private:
    explicit StudentMenuHandler() : MenuBase("ѧ������", {"1.ѧ����Ϣ����", "2.ѧ���޶�����", "3.������ҳ"}) {}

    ~StudentMenuHandler() = default;

    StudentMenuHandler(const StudentMenuHandler &studentMenu) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                StudentInfoMenuHandler::getInstance()->menuHandler();
                break;
            case 2:
                StudentCourseMenuHandler::getInstance()->menuHandler();
                break;
            case 3:
                return true;
            default:
                break;
        }
        return false;
    }
};

#endif
