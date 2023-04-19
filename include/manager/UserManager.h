/**
 * @author Half_nothing
 * @data 2023.04
 * @class UserManager
 */

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "../basics/SingletonPatternBasics.h"
#include "StudentCourseManager.h"
#include "StudentManager.h"

class UserManager : public SingletonPatternBasics<UserManager> {
    friend class SingletonPatternBasics;

public:
    UserManager(UserManager &) = delete;

    UserManager(UserManager &&) = delete;

    UserManager &operator=(const UserManager &) = delete;

    void showInfo() {
        std::cout << "欢迎您，" << this->user->studentName << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        Student::printTableHeader();
        this->user->printInfo();
        std::cout << "---------------------------------------" << std::endl;
        PAUSE;
    }

    void checkStudentCourseInfo() {
        this->info = StudentCourseManager::getInstance()->getAllStudentCoursesByStudentId(this->user->studentId);
        std::cout << "欢迎您，" << this->user->studentName << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        StudentCourse::printTableHeader();
        for (const auto &temp: info) { temp.printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
        PAUSE;
    }

    void passwordChanger() {
        StudentManager::passwordChangeHandler(this->user);
    }

    void startEvaluation() {
        std::cout << "欢迎您，" << this->user->studentName << std::endl;
        std::cout << "请选择要评价的课程" << std::endl;
        PAUSE;
        CLEAR;
        std::string target = MenuBase("请选择",
                                      StudentCourseManager::getInstance()->getAllStudentCourseNamesByStudentId(
                                              this->user->studentId), "").get();
        if (target == "取消选择") {
            std::cout << "取消选择" << std::endl;
            PAUSE;
            return;
        }
        try {
            EvaluationManager::getInstance()->evaluate(
                    StudentCourseManager::getInstance()->getStudentCourseById(this->user->studentId,
                                                                              CourseManager::getInstance()->getCourseIdByName(
                                                                                      target)));
        } catch (TargetNotFoundException &_) {
            std::cout << "课程不存在，请联系管理员" << std::endl;
            PAUSE;
            return;
        }
    }

    bool loginHandler() {
        using namespace std;
        cout << "请输入用户名:" << endl;
        std::string account;
        cin >> account;
        cin.ignore();
        cout << "请输入密码:" << endl;
        std::string password;
        char temp;
        while ((temp = (char) _getch()) != '\r') {
            password += temp;
            cout << "*";
        }
        cout << endl;
        return this->login(account, password);
    }

private:
    std::vector<Student>::iterator user;
    std::vector<StudentCourse> info;

    UserManager() = default;

    ~UserManager() = default;

    bool login(const std::string &account, const std::string &password) {
        if (StudentManager::getInstance()->checkStudentID(account)) {
            auto it = StudentManager::getInstance()->getIteratorById(account);
            if ((*it).password == password) {
                std::cout << "登陆成功, 欢迎您, " << (*it).studentName << std::endl;
                this->user = it;
                return true;
            } else {
                std::cout << "账号或密码错误" << std::endl;
                return false;
            }
        } else {
            std::cout << "账号或密码错误" << std::endl;
            return false;
        }
    }
};


#endif
