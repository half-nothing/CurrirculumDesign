/**
 * @author Half_nothing
 * @date 2023.02
 * @class CourseManager
 * @details 课程管理的核心实现
 */
#pragma once
#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <utility>
#include "../basics/BasicOperation.h"
#include "../basics/SingletonPatternBasics.h"
#include "TeacherManager.h"

// 这里不是模板类，所以要继承特化了的BasicOperation类
// 特化类型是Course,即DataType::CourseInfo
class CourseManager : public BasicOperation<Course>, public SingletonPatternBasics<CourseManager> {
    friend class SingletonPatternBasics;

public:
    CourseManager() = delete;

    CourseManager(CourseManager &) = delete;

    CourseManager(CourseManager &&) = delete;

    CourseManager &operator=(const CourseManager &) = delete;

    std::string getCourseIdByName(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Course &temp) {
            return temp.usageStatus && temp.lessonName == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return it->lessonId;
    }

    Course &getCourseByNameOrId(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Course &temp) {
            return temp.usageStatus && (temp.lessonId == target || temp.lessonName == target);
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return *it;
    }

    std::vector<std::string> &getCourseName() { return this->courseName; }

    inline bool checkLessonId(const std::string &target) {
        return std::any_of(this->inUse.begin(), this->inUse.end(), [target, this](const auto &item){
            return this->data[item].usageStatus && (this->data[item].lessonId == target || this->data[item].lessonName == target);
        });
    }

    void addHandler() override {
        using namespace std;
        Course temp;
        cout << "正在添加课程(输入0退出):" << endl
             << "请输入开始时间(1900<=x):" << endl;
        while (true) {
            cin >> temp.startData;
            if (temp.startData == 0) { return; }
            if (temp.startData < 1900) {
                cout << "开始时间必须大于或等于1900年，请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入结束时间(x<=y):" << endl;
        while (true) {
            cin >> temp.endData;
            if (temp.endData == 0) { return; }
            if (temp.endData < temp.startData) {
                cout << "结束时间必须小于或等于开始时间，请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入开设学期(1/2):" << endl;
        while (true) {
            cin >> temp.openTerm;
            if (temp.openTerm == '\n') { continue; }
            if (temp.openTerm == '0') { return; }
            if (temp.openTerm != '1' && temp.openTerm != '2') {
                cout << "学期只能是1或者2，请重新输入" << endl;
                continue;
            }
            break;
        }
        cout << "请输入课程名称:" << endl;
        cin >> temp.lessonName;
        if (std::string(temp.lessonName) == "0") { return; }
        cout << "请输入课程ID" << endl;
        while (true) {
            cin >> temp.lessonId;
            cin.ignore();
            if (string(temp.lessonId) == "0") { return; }
            if (this->checkLessonId(temp.lessonId)) {
                cout << "此课程ID已被占用，请重新输入" << endl;
                continue;
            }
            break;
        }
        CLEAR;
        std::string res = std::string(
                MenuBase(" 列表 ", TeacherManager::getInstance()->getTeacherNameList(), "").get());
        cout << "正在添加课程(输入0退出):" << endl;
        cout << "请输入开始时间(1900<=x):" << endl << temp.startData << endl;
        cout << "请输入结束时间(x<=y):" << endl << temp.endData << endl;
        cout << "请输入开设学期(1/2):" << endl << temp.openTerm << endl;
        cout << "请输入课程名称" << endl << temp.lessonName << endl;
        cout << "请输入课程ID" << endl << temp.lessonId << endl;
        cout << "请选择授课教师:" << endl;
        if (res == "取消选择") {
            PAUSE;
            return;
        }
        ::strcpy(temp.teacherName, res.c_str());
        res = TeacherManager::getInstance()->getTeacherIdByName(res);
        ::strcpy(temp.teacherId, res.c_str());
        cout << temp.teacherName << endl;
        this->add(temp);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        std::string target;
        cout << "输入q键退出" << endl;
        cout << "输入list展开选择菜单" << endl;
        cout << "请输入要删除的课程ID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getCourseIdByName(MenuBase(" 列表 ", this->courseName, "").get());
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
        Course::printTableHeader();
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
        cout << "请输入要修改的课程ID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getCourseIdByName(MenuBase(" 列表 ", this->courseName, "").get());
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
    std::vector<std::string> courseName;

    inline std::vector<Course>::iterator getIteratorById(const std::string &id) {
        return std::find_if(this->data.begin(), this->data.end(), [id](const Course &temp) {
            return temp.usageStatus && temp.lessonId == id;
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
            Course temp;
            getline(sin, field, ',');
            temp.startData = ::strtol(field.c_str(), nullptr, 10);
            getline(sin, field, ',');
            temp.endData = ::strtol(field.c_str(), nullptr, 10);
            getline(sin, field, ',');
            temp.openTerm = field.c_str()[0];
            getline(sin, field, ',');
            ::strcpy(temp.lessonName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.lessonId, field.c_str());
            getline(sin, field);
            ::strcpy(temp.teacherName, field.c_str());
            if (1900 > temp.startData) {
                std::cout << "开始日期[" << temp.startData << "]小于1900年" << std::endl;
                std::cout << "添加课程[" << temp.lessonName << "]失败" << std::endl;
                continue;
            }
            if (temp.endData < temp.startData) {
                std::cout << "开始日期[" << temp.startData << "]大于结束日期[" << temp.endData << "]" << std::endl;
                std::cout << "添加课程[" << temp.lessonName << "]失败" << std::endl;
                continue;
            }
            if (temp.openTerm != '1' && temp.openTerm != '2') {
                std::cout << "学期[" << temp.openTerm << "]只能是1或2" << std::endl;
                std::cout << "添加课程[" << temp.lessonName << "]失败" << std::endl;
                continue;
            }
            if (this->checkLessonId(temp.lessonId)) {
                std::cout << "课程ID[" << temp.lessonId << "]已被占用" << std::endl;
                std::cout << "添加课程[" << temp.lessonName << "]失败" << std::endl;
                continue;
            }
            try {
                ::strcpy(temp.teacherId, TeacherManager::getInstance()->getTeacherIdByName(temp.teacherName).c_str());
            } catch (TargetNotFoundException &_) {
                std::cout << "无法找到教师[" << temp.teacherName << "]的教师号" << std::endl;
                std::cout << "添加课程[" << temp.lessonName << "]失败" << std::endl;
                continue;
            }
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    void add(Course &target) override {
        target.generateName();
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "添加课程[" << target.lessonName << "]成功" << std::endl;
    }

    void del(std::vector<Course>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "删除课程[" << (*target).lessonName << "]成功" << std::endl;
    }

    void change(std::vector<Course>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            Course &temp = this->data[index];
            if (std::string(temp.lessonName).find(target) != std::string::npos ||
                std::string(temp.lessonId).find(target) != std::string::npos ||
                std::string(temp.teacherName).find(target) != std::string::npos) {
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
        Course::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "清空了" << this->data.size() << "条数据" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->courseName.clear();
    }

    explicit CourseManager(std::string_view fileName) : BasicOperation<Course>(fileName) {
        for (const auto &temp: this->data) {
            if (temp.usageStatus) { this->inUse.push_back(temp.storageId - 1); }
        }
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->courseName.clear();
        for (auto &temp: this->inUse) { this->courseName.emplace_back(this->data[temp].lessonName); }
        this->courseName.emplace_back("取消选择");
    }

    ~CourseManager() = default;

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->courseName.clear();
        for (auto &temp: this->inUse) { this->courseName.emplace_back(this->data[temp].lessonName); }
        this->courseName.emplace_back("取消选择");
    }

    class ChangeInfo : public ChangeBase<Course> {
    public:
        explicit
        ChangeInfo(std::vector<Course>::iterator target)
                : ChangeBase<Course>(target, "修改列表",
                                     {"1.课程名称", "2.课程ID", "3.教师ID", "4.开始年份", "5.结束年份", "6.开设学期",
                                      "7.退出修改"}) {};

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "输入0返回上一级" << endl;
            switch (this->line) {
                case 1: {
                    cout << "原课程名称为: " << this->target->lessonName << endl;
                    cout << "请输入新课程名称:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->lessonName) {
                        cout << "与原来课程名一致" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonName, target.c_str());
                    cout << "修改后的课程名称是:" << this->target->lessonName << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "原课程ID为: " << this->target->lessonId << endl;
                    cout << "请输入新课程ID:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->lessonId) {
                        cout << "与原课程ID一致" << endl;
                        PAUSE;
                        break;
                    }
                    if (CourseManager::getInstance()->checkLessonId(target)) {
                        cout << "此ID已被使用" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonId, target.c_str());
                    this->target->generateName();
                    cout << "新课程ID为:" << this->target->lessonId << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    CLEAR;
                    TeacherManager *instance = TeacherManager::getInstance();
                    std::string temp = instance->getTeacherIdByName(
                            MenuBase(" 列表 ", instance->getTeacherNameList(), "").get());
                    cout << "原授课教师为: " << this->target->teacherName << endl;
                    if (temp == "取消选择") { break; }
                    if (temp == this->target->teacherName) {
                        cout << "与原授课教师相同" << endl;
                        PAUSE;
                        break;
                    }
                    cout << "选择的新授课教师为:" << temp << endl;
                    ::strcpy(this->target->teacherName, temp.c_str());
                    ::strcpy(this->target->teacherId, instance->getTeacherIdByName(temp).c_str());
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 4: {
                    cout << "原开始年份为: " << this->target->startData << endl;
                    cout << "请输入新开始年份:" << endl;
                    uint temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp > this->target->endData) {
                        cout << "开始年份不能大于结束年份" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->startData = temp;
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 5: {
                    cout << "原结束年份为: " << this->target->endData << endl;
                    cout << "请输入新结束年份:" << endl;
                    uint temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp < this->target->startData) {
                        cout << "结束年份不能小于开始年份" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->endData = temp;
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 6: {
                    cout << "原学期为: " << this->target->openTerm << endl;
                    cout << "请输入学期:" << endl;
                    int temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp != 1 && temp != 2) {
                        cout << "学期只能是1或者2" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->openTerm = (char) (temp + '0');
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 7:
                    return true;
            }
            return false;
        }
    };
};

#endif
