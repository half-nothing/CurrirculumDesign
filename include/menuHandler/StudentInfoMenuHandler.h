#pragma once
#ifndef STUDENT_INFO_MENU_HANDLER_H
#define STUDENT_INFO_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/StudentManager.h"

class StudentInfoMenuHandler : public MenuBase, public SingletonPatternBasics<StudentInfoMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        StudentManager::getInstance()->saveData();
    }

private:
    explicit StudentInfoMenuHandler() : MenuBase("学生管理", {"1.学生列表", "2.添加学生", "3.删除学生", "4.修改学生", "5.查找学生", "6.修改密码", "7.批量导入",
                                                  "8.保存数据", "9.清空数据", "0.返回上页"}) {}

    ~StudentInfoMenuHandler() = default;

    StudentInfoMenuHandler(const StudentInfoMenuHandler &studentInfo) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                StudentManager::getInstance()->listHandler();
                break;
            case 2:
                StudentManager::getInstance()->addHandler();
                break;
            case 3:
                StudentManager::getInstance()->delHandler();
                break;
            case 4:
                StudentManager::getInstance()->changeHandler();
                break;
            case 5:
                StudentManager::getInstance()->searchHandler();
                break;
            case 6:
                StudentManager::getInstance()->passwordChangeHandler(true);
                break;
            case 7:
                StudentManager::getInstance()->batchImportHandler();
                break;
            case 8:
                StudentManager::getInstance()->saveData();
                break;
            case 9:
                StudentManager::getInstance()->clearHandler();
                break;
            case 10:
                return true;
            default:
                break;
        }
        return false;
    }
};

#endif
