#pragma once
#ifndef EVALUATION_MENU_HANDLER_H
#define EVALUATION_MENU_HANDLER_H

#include "EvaluationIndexMenuHandler.h"
#include "EvaluationDataMenuHandler.h"

class EvaluationMenuHandler : public MenuBase, public SingletonPatternBasics<EvaluationMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        EvaluationIndexMenuHandler::saveData();
        EvaluationDataMenuHandler::saveData();
    }

private:
    explicit EvaluationMenuHandler() : MenuBase("���۹���", {"1.���۱�׼", "2.��������", "3.������ҳ"}) {}

    ~EvaluationMenuHandler() = default;

    EvaluationMenuHandler(const EvaluationMenuHandler &evaluation) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                EvaluationIndexMenuHandler::getInstance()->menuHandler();
                break;
            case 2:
                EvaluationDataMenuHandler::getInstance()->menuHandler();
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
