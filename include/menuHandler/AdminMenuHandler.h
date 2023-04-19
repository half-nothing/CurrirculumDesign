/**
 * @author Half_nothing
 * @date 2023.02
 * @details ����ʵ��Ŀ¼����
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
        cout << "���������Ա�˺ţ�" << endl;
        std::string account;
        cin >> account;
        cout << "���������룺" << endl;
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
        cout << "��ӭ��������Ա" << endl
             << "�������޸ĵ�¼����" << endl
             << "������ԭ���룺" << endl;
        std::string passwordOriginal;
        std::string passwordNew;
        char temp;
        while ((temp = (char) _getch()) != '\r') {
            passwordOriginal += temp;
            cout << "*";
        }
        cout << endl << "������������:" << endl;
        while ((temp = (char) _getch()) != '\r') {
            passwordNew += temp;
            cout << "*";
        }
        cout << endl;
        if (this->admin.password != passwordOriginal) {
            cout << "ԭ�����������" << endl;
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
        cout << "��ӭ��������Ա" << endl
             << "�������޸ĵ�¼�û���" << endl
             << "���������û���:" << endl;
        std::string username;
        cin >> username;
        if (username == this->admin.account) {
            cout << "��ԭ�û�����ͬ" << endl;
            PAUSE;
            return;
        }
        cout << "ȷ�Ͻ�[" << this->admin.account << "]�ĳ�[" << username << "]��?(y/n)" << endl;
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
        cout << "��ȡ��" << endl;
        PAUSE;
    }

    explicit AdminMenuHandler() : MenuBase("�����̨",
                                           {"1.�γ̹���", "2.ѧ������", "3.��ʦ����", "4.���۹���", "5.�޸��û�",
                                            "6.�޸�����",
                                            "7.�˳���¼", "8.�������˳�"}) {
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
