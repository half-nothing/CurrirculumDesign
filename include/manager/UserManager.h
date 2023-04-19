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
        std::cout << "��ӭ����" << this->user->studentName << std::endl;
        std::cout << "---------------------------------------" << std::endl;
        Student::printTableHeader();
        this->user->printInfo();
        std::cout << "---------------------------------------" << std::endl;
        PAUSE;
    }

    void checkStudentCourseInfo() {
        this->info = StudentCourseManager::getInstance()->getAllStudentCoursesByStudentId(this->user->studentId);
        std::cout << "��ӭ����" << this->user->studentName << std::endl;
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
        std::cout << "��ӭ����" << this->user->studentName << std::endl;
        std::cout << "��ѡ��Ҫ���۵Ŀγ�" << std::endl;
        PAUSE;
        CLEAR;
        std::string target = MenuBase("��ѡ��",
                                      StudentCourseManager::getInstance()->getAllStudentCourseNamesByStudentId(
                                              this->user->studentId), "").get();
        if (target == "ȡ��ѡ��") {
            std::cout << "ȡ��ѡ��" << std::endl;
            PAUSE;
            return;
        }
        try {
            EvaluationManager::getInstance()->evaluate(
                    StudentCourseManager::getInstance()->getStudentCourseById(this->user->studentId,
                                                                              CourseManager::getInstance()->getCourseIdByName(
                                                                                      target)));
        } catch (TargetNotFoundException &_) {
            std::cout << "�γ̲����ڣ�����ϵ����Ա" << std::endl;
            PAUSE;
            return;
        }
    }

    bool loginHandler() {
        using namespace std;
        cout << "�������û���:" << endl;
        std::string account;
        cin >> account;
        cin.ignore();
        cout << "����������:" << endl;
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
                std::cout << "��½�ɹ�, ��ӭ��, " << (*it).studentName << std::endl;
                this->user = it;
                return true;
            } else {
                std::cout << "�˺Ż��������" << std::endl;
                return false;
            }
        } else {
            std::cout << "�˺Ż��������" << std::endl;
            return false;
        }
    }
};


#endif
