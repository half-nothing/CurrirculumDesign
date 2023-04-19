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
    explicit CourseMenuHandler() : MenuBase("课程管理",
                                            {"1.课程列表", "2.添加课程", "3.删除课程", "4.修改课程", "5.搜索课程", "6.批量导入",
                                             "7.保存数据", "8.清空数据", "9.返回上页"}) {}

    ~CourseMenuHandler() = default;

    CourseMenuHandler(const CourseMenuHandler &studentInfo) = default;

    // 重写父类的处理选择内容虚函数
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
