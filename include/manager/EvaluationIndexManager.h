/**
 * @author Half_nothing
 * @data 2023.03
 * @class EvaluationIndexManager
 */

#pragma once
#ifndef EVALUATION_INDEX_MANAGER_H
#define EVALUATION_INDEX_MANAGER_H

#include "../basics/BasicOperation.h"
#include "../basics/ChangeBase.h"
#include "../basics/SingletonPatternBasics.h"

class EvaluationIndexManager
        : public BasicOperation<EvaluationIndex>, public SingletonPatternBasics<EvaluationIndexManager> {
    friend class SingletonPatternBasics;

public:
    EvaluationIndexManager() = delete;

    EvaluationIndexManager(EvaluationIndexManager &) = delete;

    EvaluationIndexManager(EvaluationIndexManager &&) = delete;

    EvaluationIndexManager &operator=(const EvaluationIndexManager &) = delete;

    inline bool checkEvaluationIndex(const std::string &target) {
        for (const auto &index: this->inUse) {
            EvaluationIndex &temp = this->data[index];
            if (temp.usageStatus && temp.evaluationID == target) { return true; }
        }
        return false;
    }

    void addHandler() override {
        using namespace std;
        cout << "����¼�����۱�׼(����0�˳�):" << endl;
        EvaluationIndex temp;
        cout << "���������۱��:" << endl;
        while (true) {
            cin >> temp.evaluationID;
            if (this->checkEvaluationIndex(temp.evaluationID)) {
                cout << "�˱���ѱ�ʹ��,����������" << endl;
                continue;
            }
            break;
        }
        cout << "����������Ȩ��:" << endl;
        while (true) {
            cin >> temp.weight;
            if (abs(temp.weight - 0) < 1e-6) { return; }
            break;
        }
        cout << "��������������:" << endl;
        while (true) {
            cin >> temp.description;
            if (string(temp.description) == "0") { return; }
            break;
        }
        this->add(temp);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        std::string target;
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫɾ�������۱�׼ID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = MenuBase(" �б� ", this->evaluationIndex, "").get();
            } catch (TargetNotFoundException &_) {
                cout << "ȡ��ѡ��" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "�޷��ҵ���Ӧ����Ŀ" << endl;
            PAUSE;
            return;
        }
        cout << "��ѯ����Ŀ:" << endl;
        EvaluationIndex::printTableHeader();
        (*it).printInfo();
        cout << "ȷ��ɾ��(y/n)?" << endl;
        cin >> target;
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            this->del(it);
            PAUSE;
            return;
        }
        cout << "��ȡ��" << endl;
        PAUSE;
    }

    void changeHandler() override {
        using namespace std;
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫ�޸ĵ����۱�׼ID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = MenuBase(" �б� ", this->evaluationIndex, "").get();
            } catch (TargetNotFoundException &_) {
                cout << "ȡ��ѡ��" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "�޷��ҵ���Ӧ����Ŀ" << endl;
            PAUSE;
            return;
        }
        if (!(*it).usageStatus) {
            cout << "����Ŀ�ѱ�ɾ��" << endl;
            PAUSE;
            return;
        }
        this->change(it);
    }

    std::vector<EvaluationIndex> &getEvaluationIndex() { return this->data; }

private:
    std::vector<std::string> evaluationIndex;

    std::vector<EvaluationIndex>::iterator getIteratorById(const std::string &target) {
        return std::find_if(this->data.begin(), this->data.end(), [target](EvaluationIndex &temp) {
            return temp.usageStatus && temp.evaluationID == target;
        });
    }

    void batchImport(std::filesystem::path path) override {
        std::fstream input(path, std::ios::in);
        if (!input.is_open()) {
            std::cout << "�ļ�������" << std::endl;
            return;
        }
        std::string line;
        while (getline(input, line)) {
            std::string field;
            std::istringstream sin(line);
            EvaluationIndex temp;
            getline(sin, field, ',');
            ::strcpy(temp.evaluationID, field.c_str());
            if (this->checkEvaluationIndex(field)) {
                std::cout << "���۱�׼ID[" << field << "]�ѱ�ռ��" << std::endl
                          << "������۱�׼ʧ��" << std::endl;
                continue;
            }
            getline(sin, field, ',');
            temp.weight = ::strtod(field.c_str(), nullptr);
            getline(sin, field);
            ::strcpy(temp.description, field.c_str());
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    void add(EvaluationIndex &target) override {
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "������۱�׼�ɹ�" << std::endl;
    }

    void del(std::vector<EvaluationIndex>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "ɾ�����۱�׼�ɹ�" << std::endl;
    }

    void change(std::vector<EvaluationIndex>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            EvaluationIndex &temp = this->data[index];
            if (std::string(temp.evaluationID).find(target) != std::string::npos ||
                std::string(temp.description).find(target) != std::string::npos) {
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
        EvaluationIndex::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "ɾ����" << this->data.size() << "����¼" << std::endl;
        this->data.clear();
        this->inUse.clear();
    }

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->evaluationIndex.clear();
        for (auto &temp: this->inUse) { this->evaluationIndex.emplace_back(this->data[temp].evaluationID); }
        this->evaluationIndex.emplace_back("ȡ��ѡ��");
    }

    explicit EvaluationIndexManager(std::string_view pathName) : BasicOperation<EvaluationIndex>(pathName) {
        for (uint i = 0; i < this->data.size(); i++) {
            if (this->data[i].usageStatus) {
                this->inUse.push_back(i);
            }
        }
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->evaluationIndex.clear();
        for (auto &temp: this->inUse) { this->evaluationIndex.emplace_back(this->data[temp].evaluationID); }
        this->evaluationIndex.emplace_back("ȡ��ѡ��");
    }

    ~EvaluationIndexManager() = default;

    class ChangeInfo : public ChangeBase<EvaluationIndex> {
    public:
        explicit ChangeInfo(std::vector<EvaluationIndex>::iterator target) : ChangeBase<EvaluationIndex>(target,
                                                                                                         "�޸��б�",
                                                                                                         {"1.�޸�ID",
                                                                                                          "2.�޸�Ȩ��",
                                                                                                          "3.�޸�����",
                                                                                                          "4.�˳��޸�"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            switch (this->line) {
                case 1: {
                    cout << "ԭ����IDΪ��" << this->target->evaluationID << endl;
                    cout << "��������ID��" << endl;
                    std::string temp;
                    cin >> temp;
                    if (temp == "0") { break; }
                    if (temp == this->target->evaluationID) {
                        cout << "��ԭID��ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    if (EvaluationIndexManager::getInstance()->checkEvaluationIndex(temp)) {
                        cout << "��ID�ѱ�ռ��" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->evaluationID, temp.c_str());
                    cout << "��IDΪ:" << temp << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "ԭ����Ȩ����:" << this->target->weight << endl;
                    cout << "������Ȩ�أ�" << endl;
                    double temp;
                    cin >> temp;
                    if (abs(temp - 0) < 1e-6) { break; }
                    if (abs(temp - this->target->weight) < 1e-6) {
                        cout << "��ԭȨ����ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->weight = temp;
                    cout << "��Ȩ��Ϊ��" << temp << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "ԭ����������:" << this->target->description << endl;
                    cout << "������������:" << endl;
                    string temp;
                    cin >> temp;
                    if (temp == "0") { break; }
                    if (temp == this->target->description) {
                        cout << "��ԭ������ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->description, temp.c_str());
                    cout << "������Ϊ��" << temp << endl;
                    PAUSE;
                    break;
                }
                case 4:
                    return true;
                default:
                    break;
            }
            return false;
        }
    };
};

#endif
