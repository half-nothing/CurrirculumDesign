#pragma once
#ifndef EVALUATION_INDEX_MENU_HANDLER_H
#define EVALUATION_INDEX_MENU_HANDLER_H

#include "../basics/MenuBase.h"
#include "../manager/EvaluationIndexManager.h"

class EvaluationIndexMenuHandler : public MenuBase, public SingletonPatternBasics<EvaluationIndexMenuHandler> {
    friend class SingletonPatternBasics;

public:
    static void saveData() {
        EvaluationIndexManager::getInstance()->saveData();
    }

private:
    explicit EvaluationIndexMenuHandler() : MenuBase("���۱�׼",
                                                     {"1.��׼�б�",
                                                      "2.��ӱ�׼",
                                                      "3.ɾ����׼",
                                                      "4.��ѯ��׼",
                                                      "5.�޸ı�׼",
                                                      "6.��������",
                                                      "7.��������",
                                                      "8.�������",
                                                      "9.������ҳ"}) {}

    ~EvaluationIndexMenuHandler() = default;

    EvaluationIndexMenuHandler(const EvaluationIndexMenuHandler &studentMenu) = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                EvaluationIndexManager::getInstance()->listHandler();
                break;
            case 2:
                EvaluationIndexManager::getInstance()->addHandler();
                break;
            case 3:
                EvaluationIndexManager::getInstance()->delHandler();
                break;
            case 4:
                EvaluationIndexManager::getInstance()->searchHandler();
                break;
            case 5:
                EvaluationIndexManager::getInstance()->changeHandler();
                break;
            case 6:
                EvaluationIndexManager::getInstance()->batchImportHandler();
                break;
            case 7:
                EvaluationIndexManager::getInstance()->saveData();
                break;
            case 8:
                EvaluationIndexManager::getInstance()->clearHandler();
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
