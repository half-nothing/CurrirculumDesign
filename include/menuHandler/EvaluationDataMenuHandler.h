#pragma once
#ifndef EVALUATION_DATA_MENU_HANDLER_H
#define EVALUATION_DATA_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/EvaluationManager.h"

class EvaluationDataMenuHandler : public MenuBase, public SingletonPatternBasics<EvaluationDataMenuHandler> {
    friend class SingletonPatternBasics;
public:
    static void saveData() {
        EvaluationManager::getInstance()->saveData();
    }
private:
    explicit EvaluationDataMenuHandler() : MenuBase("���۹���",{"1.�鿴����", "2.��������", "3.�������", "4.������ҳ"}) {}

    ~EvaluationDataMenuHandler() = default;

    EvaluationDataMenuHandler(const EvaluationDataMenuHandler &studentMenu) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                EvaluationManager::getInstance()->listHandler();
                break;
            case 2:
                EvaluationManager::getInstance()->searchHandler();
                break;
            case 3:
                EvaluationManager::getInstance()->clearHandler();
                break;
            case 4:
                return true;
            default:
                break;
        }
        return false;
    }
};

#endif
