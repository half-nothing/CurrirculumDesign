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
        res.emplace_back("ȡ��ѡ��");
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
                 MenuBase("��ѡ��γ�", CourseManager::getInstance()->getCourseName(), "").get().c_str());
        cout << "����¼��ѧ���γ��޶���Ϣ(����0�˳�):" << endl;
        cout << "������γ�����:" << endl << target.lessonName << endl;
        try {
            ::strcpy(target.lessonId,
                     std::string(CourseManager::getInstance()->getCourseIdByName(target.lessonName)).c_str());
        } catch (TargetNotFoundException &_) {
            std::cout << "ȡ��ѡ��" << std::endl;
            PAUSE;
            return;
        }
        cout << "��ѯ����ѧ���:" << endl << target.lessonId << endl;
        cout << "������ѧ��ѧ��(����listչ��ѡ��˵�):" << endl;
        while (true) {
            cin >> target.studentId;
            if (string(target.studentId) == "0") { return; }
            if (string(target.studentId) == "list") {
                try {
                    CLEAR;
                    ::strcpy(target.studentName, MenuBase("��ѡ��ѧ��", student->getStudentName(), "").get().c_str());
                    ::strcpy(target.studentId, student->getStudentIdByName(target.studentName).c_str());
                } catch (TargetNotFoundException &_) {
                    cout << "��ȡ��" << endl;
                    PAUSE;
                    continue;
                }
            }
            if (!student->checkStudentID(target.studentId)) {
                cout << "δ��ѯ��ѧ��������������" << endl;
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
        cout << "����q���˳�" << endl;
        cout << "��ѡ��Ҫɾ�����޶���Ϣ" << endl;
        PAUSE;
        CLEAR;
        auto it = this->getIteratorById(this->inUse[MenuBase(" �б� ", this->studentCourse, "").get(true)] + 1);
        if (it == this->data.end()) {
            cout << "�޷��ҵ���Ӧ����Ŀ" << endl;
            PAUSE;
            return;
        }
        cout << "��ѯ����Ŀ:" << endl;
        StudentCourse::printTableHeader();
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
        cout << "��ѡ��Ҫ�޸ĵ��޶���Ϣ:" << endl;
        PAUSE;
        CLEAR;
        auto it = this->getIteratorById(this->inUse[MenuBase(" �б� ", this->studentCourse, "").get(true)]);
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
            std::cout << "�ļ�������" << std::endl;
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
                std::cout << "ѧ��[" << field << "]������" << std::endl
                          << "���ѧ���޶���Ϣʧ��" << std::endl;
                continue;
            } else {
                ::strcpy(temp.studentName, StudentManager::getInstance()->getStudentNameById(field).c_str());
            }
            getline(sin, field);
            ::strcpy(temp.lessonName, field.c_str());
            if (!CourseManager::getInstance()->checkLessonId(field)) {
                std::cout << "�γ�[" << field << "]������" << std::endl
                          << "���ѧ���޶���Ϣʧ��" << std::endl;
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
        std::cout << "ɾ����" << this->data.size() << "����¼" << std::endl;
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
        std::cout << "���ѧ���޶���Ϣ�ɹ�" << std::endl;
    }

    void del(std::vector<StudentCourse>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "ɾ���޶���Ϣ�ɹ�" << std::endl;
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
            std::cout << "δ��ѯ����¼" << std::endl;
            PAUSE;
            return;
        }
        std::cout << "�ҵ����½��: " << std::endl
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
        this->studentCourse.emplace_back("ȡ��ѡ��");
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
        this->studentCourse.emplace_back("ȡ��ѡ��");
    }

    class ChangeInfo : public ChangeBase<StudentCourse> {
    public:
        explicit ChangeInfo(std::vector<StudentCourse>::iterator target) :
                ChangeBase<StudentCourse>(target, "�޸��б�",
                                          {"1.�޸Ľ�ѧ��",
                                           "2.�޸�ѧ��",
                                           "3.�˳��޸�"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            switch (this->line) {
                case 1: {
                    cout << "ԭ���Ŀγ�Ϊ��" << this->target->lessonName << endl;
                    cout << "��ѡ���¿γ�����" << endl;
                    PAUSE;
                    CLEAR;
                    string temp = MenuBase("��ѡ��γ�", CourseManager::getInstance()->getCourseName(), "").get();
                    if (temp == "ȡ��ѡ��") { break; }
                    if (temp == this->target->lessonName) {
                        cout << "��ԭ�γ�����ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonName, temp.c_str());
                    ::strcpy(this->target->lessonId, CourseManager::getInstance()->getCourseIdByName(temp).c_str());
                    cout << "�¿γ���Ϊ��" << temp << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "ԭ����ѧ������Ϊ:" << this->target->studentName << endl;
                    cout << "��ѡ����ѧ��������" << endl;
                    PAUSE;
                    CLEAR;
                    string temp = MenuBase("��ѡ��ѧ��", StudentManager::getInstance()->getStudentName(), "").get();
                    if (temp == "ȡ��ѡ��") { break; }
                    if (temp == this->target->studentName) {
                        cout << "��ԭѧ������ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    std::vector<StudentCourse> data_ = StudentCourseManager::getInstance()->getAllStudentCourseByStudentName(temp);
                    auto it = std::find_if(data_.begin(), data_.end(), [this](const StudentCourse &item) {
                        return item.usageStatus && std::string(item.lessonName) == std::string(this->target->lessonName);
                    });
                    if (it != data_.end() && std::string(it->lessonId) == std::string(this->target->lessonId)) {
                        cout << "ѧ��[" << temp << "]���޶��γ�[" << this->target->lessonName << "]" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->studentName, temp.c_str());
                    ::strcpy(this->target->studentId, StudentManager::getInstance()->getStudentIdByName(temp).c_str());
                    cout << "��ѧ����Ϊ��" << temp << endl;
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
