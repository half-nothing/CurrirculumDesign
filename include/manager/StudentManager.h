/**
 * @author Half_nothing
 * @date 2023.02
 * @details 学生管理的核心实现
 */
#pragma once
#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "../basics/BasicOperation.h"
#include "../basics/ChangeBase.h"
#include "../basics/SingletonPatternBasics.h"

class StudentManager : public BasicOperation<Student>, public SingletonPatternBasics<StudentManager> {
    friend class SingletonPatternBasics;

    friend class UserManager;

public:
    StudentManager() = delete;

    StudentManager(StudentManager &) = delete;

    StudentManager(StudentManager &&) = delete;

    StudentManager &operator=(const StudentManager &) = delete;

    std::string getStudentNameById(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Student &temp) {
            return temp.usageStatus && temp.studentId == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return it->studentName;
    }

    std::string getStudentIdByName(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Student &temp) {
            return temp.usageStatus && temp.studentName == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return it->studentId;
    }

    std::vector<std::string> &getStudentName() { return this->studentName; }

    inline bool checkStudentID(const std::string &target) {
        return std::any_of(this->inUse.begin(), this->inUse.end(), [target, this](const auto &item){
            return this->data[item].usageStatus && this->data[item].studentId == target;
        });
    }

    void addHandler() override {
        using namespace std;
        Student temp;
        cout << "正在录入学生(输入0退出):" << endl
             << "请输入学生姓名:" << endl;
        cin >> temp.studentName;
        cin.ignore();
        if (string(temp.studentName) == "0") { return; }
        cout << "请输入性别:" << endl;
        while (true) {
            cin >> temp.sex;
            cin.ignore();
            if (string(temp.sex) == "0") { return; }
            if (string(temp.sex) != "男" && string(temp.sex) != "女") {
                cout << "性别只能是男或女,请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入学号:" << endl;
        while (true) {
            cin >> temp.studentId;
            cin.ignore();
            if (string(temp.studentId) == "0") { return; }
            if (this->checkStudentID(temp.studentId)) {
                cout << "此学号已被占用,请重新输入" << endl;
                continue;
            }
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
        cout << "请输入要删除的学生学号" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getStudentIdByName(MenuBase(" 列表 ", this->studentName, "").get());
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
        Student::printTableHeader();
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
        cout << "请输入要修改的学生学号:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getStudentIdByName(MenuBase(" 列表 ", this->studentName, "").get());
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

    static void passwordChangeHandler(std::vector<Student>::iterator target) {
        using namespace std;
        cout << "欢迎您，" << (*target).studentName << endl
             << "您正在修改登录密码" << endl
             << "请输入原密码：" << endl;
        std::string passwordOriginal;
        std::string passwordNew;
        char temp;
        while ((temp = (char) _getch()) != '\r') {
            passwordOriginal += temp;
            cout << "*";
        }
        cout << endl << "请输入新密码:" << endl;
        while ((temp = (char) _getch()) != '\r') {
            passwordNew += temp;
            cout << "*";
        }
        cout << endl;
        if ((*target).password != passwordOriginal) {
            cout << "原密码输入错误" << endl;
            PAUSE;
        } else {
            StudentManager::passwordChanger(target, passwordNew);
            PAUSE;
        }
    }

    void passwordChangeHandler(bool) {
        using namespace std;
        cout << "欢迎您，管理员" << endl
             << "请选择要修改密码的目标" << endl;
        PAUSE;
        CLEAR;
        std::vector<Student>::iterator target;
        try {
            target = this->getIteratorById(this->getStudentIdByName(MenuBase(" 列表 ", this->studentName, "").get()));
        } catch (TargetNotFoundException &_) {
            cout << "取消选择" << endl;
            PAUSE;
            return;
        }
        if (target == this->data.end()){
            cout << "目标不存在" << endl;
            PAUSE;
            return;
        }
        std::string password;
        cout << "要修改的目标为" << endl;
        Student::printTableHeader();
        (*target).printInfo();
        cout << "请输入新密码" << endl;
        cin >> password;
        StudentManager::passwordChanger(target, password);
        PAUSE;
    }

private:
    std::vector<std::string> studentName;

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
            Student temp;
            getline(sin, field, ',');
            ::strcpy(temp.studentName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.studentId, field.c_str());
            if (this->checkStudentID(field)) {
                std::cout << "学号[" << field << "]重复" << std::endl
                          << "添加学生[" << temp.studentName << "]失败" << std::endl;
                continue;
            }
            getline(sin, field);
            ::strcpy(temp.sex, field.c_str());
            if (field != "男" && field != "女") {
                std::cout << "性别[" << temp.sex << "]只能是\"男\"或\"女\"" << std::endl
                          << "添加学生[" << temp.studentName << "]失败" << std::endl;
                continue;
            }
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    std::vector<Student>::iterator getIteratorById(const std::string &target) {
        return std::find_if(this->data.begin(), this->data.end(), [target](Student &temp) {
            return temp.usageStatus && temp.studentId == target;
        });
    }

    static void passwordChanger(std::vector<Student>::iterator target, const std::string &password) {
        ::strcpy((*target).password, password.c_str());
    }

    void add(Student &target) override {
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        std::string temp(target.studentId);
        if (temp.length() >= 6) {
            ::strcpy(target.password, temp.substr(temp.length() - 6).c_str());
        } else {
            ::strcpy(target.password, "123456");
        }
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "添加学生[" << target.studentName << "]成功" << std::endl;
    }

    void del(std::vector<Student>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "删除学生[" << (*target).studentName << "]成功" << std::endl;
    }

    void change(std::vector<Student>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            Student &temp = this->data[index];
            if (std::string(temp.studentId).find(target) != std::string::npos ||
                std::string(temp.studentName).find(target) != std::string::npos) {
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
        Student::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "清空了" << this->data.size() << "条数据" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->studentName.clear();
    }

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->studentName.clear();
        for (auto &temp: this->inUse) { this->studentName.emplace_back(this->data[temp].studentName); }
        this->studentName.emplace_back("取消选择");
    }

    explicit StudentManager(std::string_view pathName) : BasicOperation<Student>(pathName) {
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
        this->studentName.clear();
        for (auto &temp: this->inUse) { this->studentName.emplace_back(this->data[temp].studentName); }
        this->studentName.emplace_back("取消选择");
    }

    ~StudentManager() = default;

    class ChangeInfo : public ChangeBase<Student> {
    public:
        explicit ChangeInfo(std::vector<Student>::iterator target) : ChangeBase<Student>(target, "修改列表",
                                                                                         {"1.修改姓名",
                                                                                          "2.修改性别",
                                                                                          "3.修改学号",
                                                                                          "4.退出修改"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "输入0返回上一级" << endl;
            switch (this->line) {
                case 1: {
                    cout << "原来的姓名为：" << this->target->studentName << endl;
                    cout << "请输入新名字：" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->studentName) {
                        cout << "与原来的名字相同" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentName, target.c_str());
                    cout << "新名字为：" << this->target->studentName << endl;
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
                        cout << "与原性别相同" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->sex, target.c_str());
                    cout << "新性别为:" << this->target->sex << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "原来的学号是:" << this->target->studentId << endl;
                    cout << "请输入新学号:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->studentId) {
                        cout << "与原学号相同" << endl;
                        PAUSE;
                        break;
                    }
                    if (StudentManager::getInstance()->checkStudentID(target)) {
                        cout << "该学号已被使用，请重新输入" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentId, target.c_str());
                    cout << "新学号为:" << this->target->studentId << endl;
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