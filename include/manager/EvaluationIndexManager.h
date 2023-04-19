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
        cout << "正在录入评价标准(输入0退出):" << endl;
        EvaluationIndex temp;
        cout << "请输入评价编号:" << endl;
        while (true) {
            cin >> temp.evaluationID;
            if (this->checkEvaluationIndex(temp.evaluationID)) {
                cout << "此编号已被使用,请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入评价权重:" << endl;
        while (true) {
            cin >> temp.weight;
            if (abs(temp.weight - 0) < 1e-6) { return; }
            break;
        }
        cout << "请输入评价描述:" << endl;
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
        cout << "输入q键退出" << endl;
        cout << "输入list展开选择菜单" << endl;
        cout << "请输入要删除的评价标准ID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = MenuBase(" 列表 ", this->evaluationIndex, "").get();
            } catch (TargetNotFoundException &_) {
                cout << "取消选择" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "无法找到对应的条目" << endl;
            PAUSE;
            return;
        }
        cout << "查询到条目:" << endl;
        EvaluationIndex::printTableHeader();
        (*it).printInfo();
        cout << "确认删除(y/n)?" << endl;
        cin >> target;
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            this->del(it);
            PAUSE;
            return;
        }
        cout << "已取消" << endl;
        PAUSE;
    }

    void changeHandler() override {
        using namespace std;
        cout << "输入q键退出" << endl;
        cout << "输入list展开选择菜单" << endl;
        cout << "请输入要修改的评价标准ID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = MenuBase(" 列表 ", this->evaluationIndex, "").get();
            } catch (TargetNotFoundException &_) {
                cout << "取消选择" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "无法找到对应的条目" << endl;
            PAUSE;
            return;
        }
        if (!(*it).usageStatus) {
            cout << "此条目已被删除" << endl;
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
            std::cout << "文件不存在" << std::endl;
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
                std::cout << "评价标准ID[" << field << "]已被占用" << std::endl
                          << "添加评价标准失败" << std::endl;
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
        std::cout << "添加评价标准成功" << std::endl;
    }

    void del(std::vector<EvaluationIndex>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "删除评价标准成功" << std::endl;
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
            std::cout << "未查询到记录" << std::endl;
            PAUSE;
            return;
        }
        std::cout << "找到如下结果: " << std::endl
                  << "---------------------------------------" << std::endl;
        EvaluationIndex::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "删除了" << this->data.size() << "条记录" << std::endl;
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
        this->evaluationIndex.emplace_back("取消选择");
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
        this->evaluationIndex.emplace_back("取消选择");
    }

    ~EvaluationIndexManager() = default;

    class ChangeInfo : public ChangeBase<EvaluationIndex> {
    public:
        explicit ChangeInfo(std::vector<EvaluationIndex>::iterator target) : ChangeBase<EvaluationIndex>(target,
                                                                                                         "修改列表",
                                                                                                         {"1.修改ID",
                                                                                                          "2.修改权重",
                                                                                                          "3.修改描述",
                                                                                                          "4.退出修改"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            switch (this->line) {
                case 1: {
                    cout << "原来的ID为：" << this->target->evaluationID << endl;
                    cout << "请输入新ID：" << endl;
                    std::string temp;
                    cin >> temp;
                    if (temp == "0") { break; }
                    if (temp == this->target->evaluationID) {
                        cout << "与原ID相同" << endl;
                        PAUSE;
                        break;
                    }
                    if (EvaluationIndexManager::getInstance()->checkEvaluationIndex(temp)) {
                        cout << "此ID已被占用" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->evaluationID, temp.c_str());
                    cout << "新ID为:" << temp << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "原来的权重是:" << this->target->weight << endl;
                    cout << "请输入权重：" << endl;
                    double temp;
                    cin >> temp;
                    if (abs(temp - 0) < 1e-6) { break; }
                    if (abs(temp - this->target->weight) < 1e-6) {
                        cout << "与原权重相同" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->weight = temp;
                    cout << "新权重为：" << temp << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "原来的描述是:" << this->target->description << endl;
                    cout << "请输入新描述:" << endl;
                    string temp;
                    cin >> temp;
                    if (temp == "0") { break; }
                    if (temp == this->target->description) {
                        cout << "与原描述相同" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->description, temp.c_str());
                    cout << "新描述为：" << temp << endl;
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
