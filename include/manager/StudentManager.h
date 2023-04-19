/**
 * @author Half_nothing
 * @date 2023.02
 * @details ѧ������ĺ���ʵ��
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
        cout << "����¼��ѧ��(����0�˳�):" << endl
             << "������ѧ������:" << endl;
        cin >> temp.studentName;
        cin.ignore();
        if (string(temp.studentName) == "0") { return; }
        cout << "�������Ա�:" << endl;
        while (true) {
            cin >> temp.sex;
            cin.ignore();
            if (string(temp.sex) == "0") { return; }
            if (string(temp.sex) != "��" && string(temp.sex) != "Ů") {
                cout << "�Ա�ֻ�����л�Ů,����������" << endl;
                continue;
            }
            break;
        }
        cout << "������ѧ��:" << endl;
        while (true) {
            cin >> temp.studentId;
            cin.ignore();
            if (string(temp.studentId) == "0") { return; }
            if (this->checkStudentID(temp.studentId)) {
                cout << "��ѧ���ѱ�ռ��,����������" << endl;
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
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫɾ����ѧ��ѧ��" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getStudentIdByName(MenuBase(" �б� ", this->studentName, "").get());
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
        Student::printTableHeader();
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
        cout << "������Ҫ�޸ĵ�ѧ��ѧ��:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getStudentIdByName(MenuBase(" �б� ", this->studentName, "").get());
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

    static void passwordChangeHandler(std::vector<Student>::iterator target) {
        using namespace std;
        cout << "��ӭ����" << (*target).studentName << endl
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
        if ((*target).password != passwordOriginal) {
            cout << "ԭ�����������" << endl;
            PAUSE;
        } else {
            StudentManager::passwordChanger(target, passwordNew);
            PAUSE;
        }
    }

    void passwordChangeHandler(bool) {
        using namespace std;
        cout << "��ӭ��������Ա" << endl
             << "��ѡ��Ҫ�޸������Ŀ��" << endl;
        PAUSE;
        CLEAR;
        std::vector<Student>::iterator target;
        try {
            target = this->getIteratorById(this->getStudentIdByName(MenuBase(" �б� ", this->studentName, "").get()));
        } catch (TargetNotFoundException &_) {
            cout << "ȡ��ѡ��" << endl;
            PAUSE;
            return;
        }
        if (target == this->data.end()){
            cout << "Ŀ�겻����" << endl;
            PAUSE;
            return;
        }
        std::string password;
        cout << "Ҫ�޸ĵ�Ŀ��Ϊ" << endl;
        Student::printTableHeader();
        (*target).printInfo();
        cout << "������������" << endl;
        cin >> password;
        StudentManager::passwordChanger(target, password);
        PAUSE;
    }

private:
    std::vector<std::string> studentName;

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
            Student temp;
            getline(sin, field, ',');
            ::strcpy(temp.studentName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.studentId, field.c_str());
            if (this->checkStudentID(field)) {
                std::cout << "ѧ��[" << field << "]�ظ�" << std::endl
                          << "���ѧ��[" << temp.studentName << "]ʧ��" << std::endl;
                continue;
            }
            getline(sin, field);
            ::strcpy(temp.sex, field.c_str());
            if (field != "��" && field != "Ů") {
                std::cout << "�Ա�[" << temp.sex << "]ֻ����\"��\"��\"Ů\"" << std::endl
                          << "���ѧ��[" << temp.studentName << "]ʧ��" << std::endl;
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
        std::cout << "���ѧ��[" << target.studentName << "]�ɹ�" << std::endl;
    }

    void del(std::vector<Student>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "ɾ��ѧ��[" << (*target).studentName << "]�ɹ�" << std::endl;
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
            std::cout << "δ��ѯ����¼" << std::endl;
            PAUSE;
            return;
        }
        std::cout << "�ҵ����½��: " << std::endl
                  << "---------------------------------------" << std::endl;
        Student::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "�����" << this->data.size() << "������" << std::endl;
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
        this->studentName.emplace_back("ȡ��ѡ��");
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
        this->studentName.emplace_back("ȡ��ѡ��");
    }

    ~StudentManager() = default;

    class ChangeInfo : public ChangeBase<Student> {
    public:
        explicit ChangeInfo(std::vector<Student>::iterator target) : ChangeBase<Student>(target, "�޸��б�",
                                                                                         {"1.�޸�����",
                                                                                          "2.�޸��Ա�",
                                                                                          "3.�޸�ѧ��",
                                                                                          "4.�˳��޸�"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "����0������һ��" << endl;
            switch (this->line) {
                case 1: {
                    cout << "ԭ��������Ϊ��" << this->target->studentName << endl;
                    cout << "�����������֣�" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->studentName) {
                        cout << "��ԭ����������ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentName, target.c_str());
                    cout << "������Ϊ��" << this->target->studentName << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "ԭ�����Ա���:" << this->target->sex << endl;
                    cout << "�������Ա�" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target != "��" && target != "Ů") {
                        cout << "�Ա�ֻ�����л�Ů" << endl;
                        PAUSE;
                        break;
                    }
                    if (target == this->target->sex) {
                        cout << "��ԭ�Ա���ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->sex, target.c_str());
                    cout << "���Ա�Ϊ:" << this->target->sex << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "ԭ����ѧ����:" << this->target->studentId << endl;
                    cout << "��������ѧ��:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->studentId) {
                        cout << "��ԭѧ����ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    if (StudentManager::getInstance()->checkStudentID(target)) {
                        cout << "��ѧ���ѱ�ʹ�ã�����������" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentId, target.c_str());
                    cout << "��ѧ��Ϊ:" << this->target->studentId << endl;
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