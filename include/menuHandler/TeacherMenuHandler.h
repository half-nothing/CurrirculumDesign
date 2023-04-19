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
            MenuBase("教师管理", {"1.教师列表", "2.添加教师", "3.删除教师", "4.修改教师", "5.查找教师", "6.批量导入", "7.保存数据", "8.清空数据",
                                  "9.返回上页"}) {}

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
