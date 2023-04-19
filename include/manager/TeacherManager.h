/**
 * @author Half_nothing
 * @date 2023.02
 * @details 教师管理的核心实现
 */
#pragma once
#ifndef TEACHER_MANAGER_H
#define TEACHER_MANAGER_H

#include "../basics/BasicOperation.h"
#include "../basics/ChangeBase.h"
#include "../basics/SingletonPatternBasics.h"

class TeacherManager : public BasicOperation<Teacher>, public SingletonPatternBasics<TeacherManager> {
    friend class SingletonPatternBasics;

public:
    TeacherManager() = delete;

    TeacherManager(TeacherManager &) = delete;

    TeacherManager(TeacherManager &&) = delete;

    TeacherManager &operator=(const TeacherManager &) = delete;

    std::string getTeacherIdByName(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Teacher &temp) {
            return temp.usageStatus && temp.teacherName == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return (*it).teacherId;
    }

    inline std::vector<std::string> &getTeacherNameList() {
        return this->teacherNameList;
    }

    void addHandler() override {
        using namespace std;
        cout << "正在录入教师(输入0退出):" << endl;
        Teacher target;
        cout << "请输入教师姓名:" << endl;
        cin >> target.teacherName;
        if (std::string(target.teacherName) == "0") { return; }
        cout << "请输入性别:" << endl;
        while (true) {
            cin >> target.sex;
            if (std::string(target.sex) == "0") { return; }
            if (std::string(target.sex) != "男" && std::string(target.sex) != "女") {
                cout << "性别只能是男或女,请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入教师号:" << endl;
        while (true) {
            cin >> target.teacherId;
            if (string(target.teacherId) == "0") { return; }
            auto it = this->getIteratorById(target.teacherId);
            if (it != this->data.end()) {
                cout << "此教师号已被占用，请重新输入" << endl;
                continue;
            }
            break;
        }
        this->add(target);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        string target;
        cout << "输入q键退出" << endl;
        cout << "输入list展开选择菜单" << endl;
        cout << "请输入要删除的教师ID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getTeacherIdByName(MenuBase(" 列表 ", this->teacherNameList, "").get());
            } catch (TargetNotFoundException &_) {
                cout << "取消选择" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "无法找到对应条目" << endl;
            PAUSE;
            return;
        }
        cout << "查询到条目:" << endl;
        Teacher::printTableHeader();
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
        cout << "请输入要修改的教师ID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getTeacherIdByName(MenuBase(" 列表 ", this->teacherNameList, "").get());
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

private:
    std::vector<std::string> teacherNameList;

    inline bool checkTeacherId(const std::string &target) {
        return std::any_of(this->inUse.begin(), this->inUse.end(), [target, this](const auto &index){
            return this->data[index].usageStatus && this->data[index].teacherId == target;
        });
    }

    inline std::vector<Teacher>::iterator getIteratorById(const std::string &id) {
        return std::find_if(this->data.begin(), this->data.end(), [id](const Teacher &temp) {
            return temp.usageStatus && temp.teacherId == id;
        });
    }

    void batchImport(std::filesystem::path path) override {
        std::fstream input(path, std::ios::in);
        if (!input.is_open()) {
            std::cout << "文件不存在" << std::endl;
            PAUSE;
            return;
        }
        std::string line;
        while (getline(input, line)) {
            std::string field;
            std::istringstream sin(line);
            Teacher temp;
            getline(sin, field, ',');
            ::strcpy(temp.teacherName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.teacherId, field.c_str());
            if (this->checkTeacherId(field)) {
                std::cout << "教师号[" << field << "]重复" << std::endl
                          << "添加教师[" << temp.teacherName << "]失败" << std::endl;
                continue;
            }
            getline(sin, field);
            ::strcpy(temp.sex, field.c_str());
            if (field != "男" && field != "女") {
                std::cout << "性别[" << temp.teacherId << "]只能是\"男\"或\"女\"" << std::endl
                          << "添加学生[" << temp.teacherName << "]失败" << std::endl;
                continue;
            }
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    void add(Teacher &target) override {
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "添加教师[" << target.teacherName << "]成功" << std::endl;
    }

    void del(std::vector<Teacher>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "删除教师[" << (*target).teacherName << "]成功" << std::endl;
    }

    void change(std::vector<Teacher>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            const Teacher &temp = this->data[index];
            if (std::string(temp.teacherName).find(target) != std::string::npos ||
                std::string(temp.teacherId).find(target) != std::string::npos) {
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
        Teacher::printTableHeader();
        for (const auto &temp: res) { this->data[temp - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "清空了" << this->data.size() << "条数据" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->teacherNameList.clear();
    }

    explicit TeacherManager(std::string_view pathName) : BasicOperation<Teacher>(pathName) {
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
        this->teacherNameList.clear();
        for (auto &temp: this->inUse) {
            this->teacherNameList.emplace_back(this->data[temp].teacherName);
        }
        this->teacherNameList.emplace_back("取消选择");
    }

    ~TeacherManager() = default;

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->teacherNameList.clear();
        for (auto &temp: this->inUse) {
            this->teacherNameList.emplace_back(this->data[temp].teacherName);
        }
        this->teacherNameList.emplace_back("取消选择");
    }

    class ChangeInfo : public ChangeBase<Teacher> {
    public:
        explicit ChangeInfo(std::vector<Teacher>::iterator target) : ChangeBase<Teacher>(target, "修改列表",
                                                                                         {"1.修改姓名",
                                                                                          "2.修改性别",
                                                                                          "3.修改教师号",
                                                                                          "4.退出修改"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "输入0返回上一级" << endl;
            switch (this->line) {
                case 1: {
                    cout << "原来的姓名为：" << this->target->teacherName << endl;
                    cout << "请输入新名字：" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->teacherName) {
                        cout << "与原教师名一致" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->teacherName, target.c_str());
                    cout << "修改后的名字为：" << this->target->teacherName << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "原来的性别是:" << this->target->sex << endl;
                    cout << "请输入性别：" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target != "男" && target != "女") {
                        cout << "性别只能是男或女" << endl;
                        PAUSE;
                        break;
                    }
                    if (target == this->target->sex) {
                        cout << "与原性别一致" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->sex, target.c_str());
                    cout << "新性别为：" << this->target->sex << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "原来的教师号是:" << this->target->teacherId << endl;
                    cout << "请输入新教师号:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->teacherId) {
                        cout << "与原教师号相同" << endl;
                        PAUSE;
                        break;
                    }
                    if (TeacherManager::getInstance()->checkTeacherId(target)) {
                        cout << "该教师号已被使用，请重新输入" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->teacherId, target.c_str());
                    cout << "新教师号为:" << this->target->teacherId << endl;
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
