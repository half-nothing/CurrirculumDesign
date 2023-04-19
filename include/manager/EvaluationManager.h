/**
 * @author Half_nothing
 * @date 2023.02
 * @details ���۹���ĺ���ʵ��
 */
#pragma once
#ifndef EVALUATION_MANAGER_H
#define EVALUATION_MANAGER_H

#include <utility>

#include "../basics/BasicOperation.h"
#include "../basics/SingletonPatternBasics.h"

class EvaluationManager : public BasicOperation<Evaluation>, public SingletonPatternBasics<EvaluationManager> {
    friend class SingletonPatternBasics;

public:
    EvaluationManager() = delete;

    EvaluationManager(EvaluationManager &) = delete;

    EvaluationManager(EvaluationManager &&) = delete;

    EvaluationManager &operator=(const EvaluationManager &) = delete;

    void evaluate(StudentCourse &target) {
        std::vector<EvaluationIndex> &evaluationIndex = EvaluationIndexManager::getInstance()->getEvaluationIndex();
        std::cout << "��ʼ���ۿγ�[" << target.lessonName << "]" << std::endl
                  << "�ڿν�ʦ[" << CourseManager::getInstance()->getCourseByNameOrId(target.lessonId).teacherName
                  << "]" << std::endl;
        for (const auto &index: evaluationIndex) {
            std::cout << "���۱�׼���[" << index.evaluationID << "]" << std::endl
                      << "��������[" << index.description << "]" << std::endl
                      << "���������۷���:" << std::endl;
            double score;
            std::cin >> score;
            if (score < 0) { score = 0; }
            if (score > 100) { score = 100; }
            Evaluation temp;
            temp.score = score;
            temp.weight = index.weight;
            ::strcpy(temp.evaluationID, index.evaluationID);
            ::strcpy(temp.lessonId, target.lessonId);
            ::strcpy(temp.lessonName, target.lessonName);
            ::strcpy(temp.studentName, target.studentName);
            ::strcpy(temp.studentId, target.studentId);
            this->add(temp);
        }
        this->calculateScore(target);
        std::cout << "�������" << std::endl;
        PAUSE;
    }

private:
    void calculateScore(StudentCourse &target) {
        double weight = 0;
        double res = 0;
        auto temp = this->getAllEvaluationDataByNameOrId(target.lessonId, target.studentId);
        for (const auto &item: temp) { weight += item.weight; }
        for (const auto &item: temp) {
            res += item.score * item.weight / weight;
        }
        target.score = res;
        target.evaluated = true;
    }

    std::vector<Evaluation> getAllEvaluationDataByNameOrId(const std::string &lessonId, const std::string &studentId) {
        std::vector<Evaluation> res;
        for (const auto &index: this->inUse) {
            Evaluation &temp = this->data[index];
            if (temp.usageStatus && temp.lessonId == lessonId && temp.studentId == studentId) {
                res.push_back(temp);
            }
        }
        return res;
    }

    void add(Evaluation &target) override {
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            Evaluation &temp = this->data[index];
            if (std::string(temp.studentId).find(target) != std::string::npos ||
                std::string(temp.studentName).find(target) != std::string::npos||
                std::string(temp.lessonId).find(target) != std::string::npos||
                std::string(temp.lessonName).find(target) != std::string::npos) {
                res.push_back(temp.storageId);
            }
        }
        if (res.empty()) {
            std::cout << "δ��ѯ����¼" << std::endl;
            PAUSE;
            return;
        }
        std::cout << "�ҵ����½��: " << std::endl
                  << "---------------------------------------" << std::endl;
        Evaluation::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void batchImport(std::filesystem::path path) override {}

    void clear() override {
        std::cout << "�����" << this->data.size() << "������" << std::endl;
        this->data.clear();
        this->inUse.clear();
    }

    explicit EvaluationManager(std::string_view pathName) : BasicOperation<Evaluation>(pathName) {
        for (uint i = 0; i < this->data.size(); i++) {
            if (this->data[i].usageStatus) {
                this->inUse.push_back(i);
            }
        }
        this->calculate();
    }

    ~EvaluationManager() = default;
};

#endif
