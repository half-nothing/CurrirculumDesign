/**
 * @author Half_nothing
 * @date 2023.02
 * @details 用于实现目录的类
 */

#pragma once
#ifndef ADMIN_MENU_HANDLER_H
#define ADMIN_MENU_HANDLER_H

#include "CourseMenuHandler.h"
#include "StudentMenuHandler.h"
#include "TeacherMenuHandler.h"
#include "EvaluationMenuHandler.h"

class AdminMenuHandler : public MenuBase, public SingletonPatternBasics<AdminMenuHandler> {
    friend class SingletonPatternBasics;

public:
    AdminMenuHandler(AdminMenuHandler &) = delete;

    AdminMenuHandler(AdminMenuHandler &&) = delete;

    AdminMenuHandler &operator=(const AdminMenuHandler &) = delete;

    bool login() {
        using namespace std;
        cout << "请输入管理员账号：" << endl;
        std::string account;
        cin >> account;
        cout << "请输入密码：" << endl;
        std::string password;
        char temp;
        while ((temp = (char) _getch()) != '\r') {
            password += temp;
            cout << "*";
        }
        cout << endl;
        return account == this->admin.account && password == this->admin.password;
    }

    static void saveALL() {
        CourseMenuHandler::saveData();
        StudentMenuHandler::saveData();
        TeacherMenuHandler::saveData();
        EvaluationMenuHandler::saveData();
    }

private:
    Admin admin = {"admin", "123456"};

    void changePassword() {
        using namespace std;
        cout << "欢迎您，管理员" << endl
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
        if (this->admin.password != passwordOriginal) {
            cout << "原密码输入错误" << endl;
            PAUSE;
        } else {
            ::strcpy(this->admin.password, passwordNew.c_str());
            PAUSE;
        }
        std::fstream(std::filesystem::path(".\\data\\admin.dat"), std::ios::out | std::ios::binary).write(
                (char *) &(this->admin), sizeof(Admin));
    }

    void changeUserName() {
        using namespace std;
        cout << "欢迎您，管理员" << endl
             << "您正在修改登录用户名" << endl
             << "请输入新用户名:" << endl;
        std::string username;
        cin >> username;
        if (username == this->admin.account) {
            cout << "与原用户名相同" << endl;
            PAUSE;
            return;
        }
        cout << "确认将[" << this->admin.account << "]改成[" << username << "]吗?(y/n)" << endl;
        std::string target;
        cin >> target;
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            ::strcpy(this->admin.account, username.c_str());
            std::fstream(std::filesystem::path(".\\data\\admin.dat"), std::ios::out | std::ios::binary).write(
                    (char *) &(this->admin), sizeof(Admin));
            PAUSE;
            return;
        }
        cout << "已取消" << endl;
        PAUSE;
    }

    explicit AdminMenuHandler() : MenuBase("管理后台",
                                           {"1.课程管理", "2.学生管理", "3.教师管理", "4.评价管理", "5.修改用户",
                                            "6.修改密码",
                                            "7.退出登录", "8.不保存退出"}) {
        std::fstream input(std::filesystem::path(".\\data\\admin.dat"), std::ios::in);
        if (!input.is_open()) {
            std::fstream output(std::filesystem::path(".\\data\\admin.dat"), std::ios::out | std::ios::binary);
            output.write((char *) &(this->admin), sizeof(Admin));
        } else {
            input.read((char *) &(this->admin), sizeof(Admin));
        }
    }

    ~AdminMenuHandler() = default;

    bool choiceHandler() override {
        switch (this->line) {
            case 1:
                CourseMenuHandler::getInstance()->menuHandler();
                break;
            case 2:
                StudentMenuHandler::getInstance()->menuHandler();
                break;
            case 3:
                TeacherMenuHandler::getInstance()->menuHandler();
                break;
            case 4:
                EvaluationMenuHandler::getInstance()->menuHandler();
                break;
            case 5:
                this->changeUserName();
                break;
            case 6:
                this->changePassword();
                break;
            case 7:
                AdminMenuHandler::saveALL();
            case 8:
                return true;
            default:
                break;
        }
        return false;
    }
};

#endif
