/**
 * @author Half_nothing
 * @data 2023.03
 * @class StudentCourseManager
 */

#pragma once
#ifndef STUDENT_COURSE_MANAGER_H
#define STUDENT_COURSE_MANAGER_H

#include "../basics/BasicOperation.h"
#include "StudentManager.h"
#include "CourseManager.h"

#include <utility>
#include "../basics/ChangeBase.h"
#include "../basics/SingletonPatternBasics.h"

class StudentCourseManager : public BasicOperation<StudentCourse>, public SingletonPatternBasics<StudentCourseManager> {
    friend class SingletonPatternBasics;

public:
    StudentCourseManager() = delete;

    StudentCourseManager(StudentCourseManager &) = delete;

    StudentCourseManager(StudentCourseManager &&) = delete;

    StudentCourseManager &operator=(const StudentCourseManager &) = delete;

    StudentCourse &getStudentCourseById(const std::string &studentId, const std::string &lessonId) {
        for (const auto &index: this->inUse) {
            StudentCourse &temp = this->data[index];
            if (temp.usageStatus && temp.studentId == studentId && temp.lessonId == lessonId) {
                return temp;
            }
        }
        throw TargetNotFoundException();
    }

    std::vector<StudentCourse> getAllStudentCoursesByStudentId(const std::string &target) {
        std::vector<StudentCourse> res;
        for (const auto &index: this->inUse) {
            StudentCourse &temp = this->data[index];
            if (temp.usageStatus && temp.studentId == target) {
                res.push_back(temp);
            }
        }
        return res;
    }

    std::vector<std::string> getAllStudentCourseNamesByStudentId(const std::string &target) {
        std::vector<std::string> res;
        for (const auto &index: this->inUse) {
            StudentCourse &temp = this->data[index];
            if (temp.usageStatus && !temp.evaluated && temp.studentId == target) {
                res.emplace_back(temp.lessonName);
            }
        }
        res.emplace_back("取消选择");
        return res;
    }

    std::vector<StudentCourse> getAllStudentCourseByStudentName(const std::string &target) {
        std::vector<StudentCourse> res;
        for (const auto &index: this->inUse) {
            StudentCourse &temp = this->data[index];
            if (temp.usageStatus && temp.studentName == target) {
                res.push_back(temp);
            }
        }
        return res;
    }

    void addHandler() override {
        using namespace std;
        StudentCourse target;
        StudentManager *student = StudentManager::getInstance();
        CLEAR;
        ::strcpy(target.lessonName,
                 MenuBase("请选择课程", CourseManager::getInstance()->getCourseName(), "").get().c_str());
        cout << "正在录入学生课程修读信息(输入0退出):" << endl;
        cout << "请输入课程名称:" << endl << target.lessonName << endl;
        try {
            ::strcpy(target.lessonId,
                     std::string(CourseManager::getInstance()->getCourseIdByName(target.lessonName)).c_str());
        } catch (TargetNotFoundException &_) {
            std::cout << "取消选择" << std::endl;
            PAUSE;
            return;
        }
        cout << "查询到教学班号:" << endl << target.lessonId << endl;
        cout << "请输入学生学号(输入list展开选择菜单):" << endl;
        while (true) {
            cin >> target.studentId;
            if (string(target.studentId) == "0") { return; }
            if (string(target.studentId) == "list") {
                try {
                    CLEAR;
                    ::strcpy(target.studentName, MenuBase("请选择学生", student->getStudentName(), "").get().c_str());
                    ::strcpy(target.studentId, student->getStudentIdByName(target.studentName).c_str());
                } catch (TargetNotFoundException &_) {
                    cout << "已取消" << endl;
                    PAUSE;
                    continue;
                }
            }
            if (!student->checkStudentID(target.studentId)) {
                cout << "未查询到学生，请重新输入" << endl;
                PAUSE;
                continue;
            }
            break;
        }
        this->add(target);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        std::string target;
        cout << "输入q键退出" << endl;
        cout << "请选择要删除的修读信息" << endl;
        PAUSE;
        CLEAR;
        auto it = this->getIteratorById(this->inUse[MenuBase(" 列表 ", this->studentCourse, "").get(true)] + 1);
        if (it == this->data.end()) {
            cout << "无法找到对应的条目" << endl;
            PAUSE;
            return;
        }
        cout << "查询到条目:" << endl;
        StudentCourse::printTableHeader();
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
        cout << "请选择要修改的修读信息:" << endl;
        PAUSE;
        CLEAR;
        auto it = this->getIteratorById(this->inUse[MenuBase(" 列表 ", this->studentCourse, "").get(true)]);
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
        this->calculate();
    }

private:
    std::vector<std::string> studentCourse;

    inline std::vector<StudentCourse>::iterator getIteratorById(uint id) {
        return std::find_if(this->data.begin(), this->data.end(), [id](const StudentCourse &temp) {
            return temp.usageStatus && temp.storageId == id;
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
            StudentCourse temp;
            getline(sin, field, ',');
            ::strcpy(temp.studentId, field.c_str());
            if (!StudentManager::getInstance()->checkStudentID(field)) {
                std::cout << "学号[" << field << "]不存在" << std::endl
                          << "添加学生修读信息失败" << std::endl;
                continue;
            } else {
                ::strcpy(temp.studentName, StudentManager::getInstance()->getStudentNameById(field).c_str());
            }
            getline(sin, field);
            ::strcpy(temp.lessonName, field.c_str());
            if (!CourseManager::getInstance()->checkLessonId(field)) {
                std::cout << "课程[" << field << "]不存在" << std::endl
                          << "添加学生修读信息失败" << std::endl;
                continue;
            } else {
                ::strcpy(temp.lessonId, CourseManager::getInstance()->getCourseIdByName(field).c_str());
            }
            this->add(temp);
         }
        input.close();
        PAUSE;
    }

    void clear() override {
        std::cout << "删除了" << this->data.size() << "条记录" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->studentCourse.clear();
    }

    void add(StudentCourse &target) override {
        target.usageStatus = true;
        target.evaluated = false;
        target.score = 0;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "添加学生修读信息成功" << std::endl;
    }

    void del(std::vector<StudentCourse>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "删除修读信息成功" << std::endl;
    }

    void change(std::vector<StudentCourse>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            StudentCourse &temp = this->data[index];
            if (std::string(temp.lessonName).find(target) != std::string::npos ||
                std::string(temp.lessonId).find(target) != std::string::npos ||
                std::string(temp.studentId).find(target) != std::string::npos ||
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
        StudentCourse::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    explicit StudentCourseManager(std::string_view pathName)
            : BasicOperation<StudentCourse>(pathName) {
        for (uint i = 0; i < this->data.size(); i++) {
            if (this->data[i].usageStatus) {
                this->inUse.push_back(i);
            }
        }
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->studentCourse.clear();
        for (auto &temp: this->inUse) {
            this->studentCourse.emplace_back(
                    std::string(this->data[temp].studentName) + "-" + std::string(this->data[temp].lessonName));
        }
        this->studentCourse.emplace_back("取消选择");
    }

    ~StudentCourseManager() = default;

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->studentCourse.clear();
        for (auto &temp: this->inUse) {
            this->studentCourse.emplace_back(std::string(this->data[temp].studentName) + "-" + std::string(this->data[temp].lessonName));
        }
        this->studentCourse.emplace_back("取消选择");
    }

    class ChangeInfo : public ChangeBase<StudentCourse> {
    public:
        explicit ChangeInfo(std::vector<StudentCourse>::iterator target) :
                ChangeBase<StudentCourse>(target, "修改列表",
                                          {"1.修改教学班",
                                           "2.修改学生",
                                           "3.退出修改"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            switch (this->line) {
                case 1: {
                    cout << "原来的课程为：" << this->target->lessonName << endl;
                    cout << "请选择新课程名：" << endl;
                    PAUSE;
                    CLEAR;
                    string temp = MenuBase("请选择课程", CourseManager::getInstance()->getCourseName(), "").get();
                    if (temp == "取消选择") { break; }
                    if (temp == this->target->lessonName) {
                        cout << "与原课程名相同" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonName, temp.c_str());
                    ::strcpy(this->target->lessonId, CourseManager::getInstance()->getCourseIdByName(temp).c_str());
                    cout << "新课程名为：" << temp << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "原来的学生姓名为:" << this->target->studentName << endl;
                    cout << "请选择新学生姓名：" << endl;
                    PAUSE;
                    CLEAR;
                    string temp = MenuBase("请选择学生", StudentManager::getInstance()->getStudentName(), "").get();
                    if (temp == "取消选择") { break; }
                    if (temp == this->target->studentName) {
                        cout << "与原学生名相同" << endl;
                        PAUSE;
                        break;
                    }
                    std::vector<StudentCourse> data_ = StudentCourseManager::getInstance()->getAllStudentCourseByStudentName(temp);
                    auto it = std::find_if(data_.begin(), data_.end(), [this](const StudentCourse &item) {
                        return item.usageStatus && std::string(item.lessonName) == std::string(this->target->lessonName);
                    });
                    if (it != data_.end() && std::string(it->lessonId) == std::string(this->target->lessonId)) {
                        cout << "学生[" << temp << "]已修读课程[" << this->target->lessonName << "]" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentName, temp.c_str());
                    ::strcpy(this->target->studentId, StudentManager::getInstance()->getStudentIdByName(temp).c_str());
                    cout << "新学生名为：" << temp << endl;
                    PAUSE;
                    break;
                }
                case 3:
                    return true;
                default:
                    break;
            }
            return false;
        }
    };
};

#endif
