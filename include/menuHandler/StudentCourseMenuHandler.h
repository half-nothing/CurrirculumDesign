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
            MenuBase("修读信息",
                     {"1.修读列表",
                      "2.添加修读",
                      "3.删除修读",
                      "4.查询修读",
                      "5.修改修读",
                      "6.批量导入",
                      "7.保存数据",
                      "8.清空数据",
                      "9.返回上页"}) {}

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