/**
 * @author Half_nothing
 * @date 2023.02
 * @details �洢�ĵ�Ԫ�ṹ��
 * @namespace DataType
 */

#pragma once
#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "iomanip"

// Ϊ�˷������ͳһ����DataType�����ռ�
namespace DataType {

    typedef
    struct DataSize {
        static constexpr int storageId = 5;
        static constexpr int openTerm = 5;
        static constexpr int startData = 10;
        static constexpr int endData = 10;
        static constexpr int teacherName = 10;
        static constexpr int teacherId = 10;
        static constexpr int lessonId = 20;
        static constexpr int lessonName = 30;
        static constexpr int sex = 5;
        static constexpr int studentId = 20;
        static constexpr int studentName = 20;
        static constexpr int evaluated = 10;
        static constexpr int score = 5;
        static constexpr int weight = 5;
        static constexpr int evaluationID = 10;
        static constexpr int level = 5;
    } DataSize;

    typedef
    struct Admin {
        char account[NAME_LEN]{'\0'};
        char password[PASSWORD_LEN]{'\0'};

        Admin() = default;

        Admin(const std::string &accout, const std::string &password) {
            ::strcpy(this->account, accout.c_str());
            ::strcpy(this->password, password.c_str());
        }
    } Admin;

    typedef
    struct CourseInfo // ��ѧ����
    {
        uint storageId{};                           // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                         // �Ƿ�ɾ��
        char openTerm{};                            // ѧ��
        uint startData{};                           // ��ʼ���
        uint endData{};                             // �������
        char teacherName[NAME_LEN]{'\0'};       // �ڿν�ʦ��
        char teacherId[NAME_LEN]{'\0'};         // �ڿν�ʦID
        char lessonId[ID_LEN]{'\0'};            // �γ�ID
        char lessonName[NAME_LEN]{'\0'};        // �γ�����
        char className[FULL_NAME_LEN]{'\0'};    // ��ѧ��ȫ��

        // Ĭ�Ϲ��캯��
        CourseInfo() = default;

        // �������캯��������vector��push_back
        CourseInfo(const CourseInfo &target) {
            this->deepCopy(target);
        }

        // ����=�����������ֱ�Ӹ��ƿ���
        CourseInfo &operator=(const CourseInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        // ��������ȫ��
        void generateName() {
            std::string temp =
                    "(" + std::to_string(this->startData) + "-" + std::to_string(this->endData) + "-" + this->openTerm +
                    ")-" + this->lessonId + "-" +
                    this->teacherId;
            ::memset(this->className, '\0', sizeof(this->className));
            ::strcpy(this->className, temp.c_str());
        }

        // ��ӡ�����Ϣ
        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::lessonId) << std::left << this->lessonId;
            cout << setw(DataSize::lessonName) << std::left << this->lessonName;
            cout << setw(DataSize::teacherName) << std::left << this->teacherName;
            cout << setw(DataSize::startData) << std::left << this->startData;
            cout << setw(DataSize::endData) << std::left << this->endData;
            cout << setw(DataSize::openTerm) << std::left << this->openTerm;
            cout << std::left << this->className << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::lessonId) << std::left << "�γ�ID";
            cout << setw(DataSize::lessonName) << std::left << "�γ�����";
            cout << setw(DataSize::teacherName) << std::left << "�ڿν�ʦ";
            cout << setw(DataSize::startData) << std::left << "��ʼʱ��";
            cout << setw(DataSize::endData) << std::left << "����ʱ��";
            cout << setw(DataSize::openTerm) << std::left << "ѧ��";
            cout << std::left << "��ѧ����Ϣ" << endl;
        }

    private:
        // ˽�е���Ʒ���
        void deepCopy(const CourseInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->openTerm = target.openTerm;
            this->startData = target.startData;
            this->endData = target.endData;
            ::memset(this->teacherName, '\0', sizeof(this->teacherName));
            ::memset(this->teacherId, '\0', sizeof(this->teacherId));
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->className, '\0', sizeof(this->className));
            ::strcpy(this->teacherName, target.teacherName);
            ::strcpy(this->teacherId, target.teacherId);
            ::strcpy(this->lessonId, target.lessonId);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->className, target.className);
        }
    } CourseInfo;

    typedef
    struct TeacherInfo // ��ʦ��Ϣ
    {
        uint storageId{};                           // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                         // �Ƿ�ɾ��
        char sex[4]{'\0'};                      // �Ա�
        char teacherName[NAME_LEN]{'\0'};       // ����
        char teacherId[TEACHER_ID_LEN]{'\0'};   // ID

        TeacherInfo() = default;

        TeacherInfo(const TeacherInfo &target) {
            this->deepCopy(target);
        }

        TeacherInfo &operator=(const TeacherInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::teacherId) << left << this->teacherId;
            cout << setw(DataSize::teacherName) << left << this->teacherName;
            cout << setw(DataSize::sex) << left << this->sex << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::teacherId) << left << "��ʦ��";
            cout << setw(DataSize::teacherName) << left << "��ʦ��";
            cout << setw(DataSize::sex) << left << "�Ա�" << endl;
        }

    private:
        void deepCopy(const TeacherInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            ::memset(this->teacherName, '\0', sizeof(this->teacherName));
            ::memset(this->teacherId, '\0', sizeof(this->teacherId));
            ::memset(this->sex, '\0', sizeof(this->sex));
            ::strcpy(this->sex, target.sex);
            ::strcpy(this->teacherName, target.teacherName);
            ::strcpy(this->teacherId, target.teacherId);
        }
    } TeacherInfo;

    typedef
    struct StudentInfo // ѧ����Ϣ
    {
        uint storageId{};                           // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                         // �Ƿ�ɾ��
        char sex[4]{'\0'};                      // �Ա�
        char studentName[NAME_LEN]{'\0'};       // ����
        char studentId[STUDENT_ID_LEN]{'\0'};   // ѧ��
        char password[PASSWORD_LEN]{'\0'};

        StudentInfo() = default;

        StudentInfo(const StudentInfo &target) {
            this->deepCopy(target);
        }

        StudentInfo &operator=(const StudentInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::sex) << left << this->sex << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::studentId) << left << "ѧ��";
            cout << setw(DataSize::studentName) << left << "ѧ������";
            cout << setw(DataSize::sex) << left << "�Ա�" << endl;
        }

    private:
        void deepCopy(const StudentInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->sex, '\0', sizeof(this->sex));
            ::memset(this->password, '\0', sizeof(this->password));
            ::strcpy(this->password, target.password);
            ::strcpy(this->sex, target.sex);
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->studentId, target.studentId);
        }
    } StudentInfo;

    typedef
    struct StudentCourseInfo // ѧ���γ��޶���Ϣ
    {
        uint storageId{};                           // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                         // �Ƿ�ɾ��
        bool evaluated{false};                      // �Ƿ��Ѿ�����
        double score{};                             // ѧ�����۵ķ���
        char lessonId[FULL_NAME_LEN]{'\0'};     // �γ�ID
        char lessonName[NAME_LEN]{'\0'};        // �γ�����
        char studentId[STUDENT_ID_LEN]{'\0'};   // ѧ��ѧ��
        char studentName[NAME_LEN]{'\0'};       // ѧ������

        StudentCourseInfo() = default;

        StudentCourseInfo(const StudentCourseInfo &target) {
            this->deepCopy(target);
        }

        StudentCourseInfo &operator=(const StudentCourseInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::lessonId) << left << this->lessonId;
            cout << setw(DataSize::lessonName) << left << this->lessonName;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::evaluated) << left << (this->evaluated ? "��" : "��");
            if (this->evaluated)
                cout << setw(DataSize::score) << left << this->score << endl;
            else
                cout << setw(DataSize::score) << left << "δ����" << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::lessonId) << left << "�γ�ID";
            cout << setw(DataSize::lessonName) << left << "�γ�����";
            cout << setw(DataSize::studentId) << left << "ѧ��";
            cout << setw(DataSize::studentName) << left << "ѧ������";
            cout << setw(DataSize::evaluated) << left << "�Ƿ�����";
            cout << setw(DataSize::score) << left << "���۷���" << endl;
        }

    private:
        void deepCopy(const StudentCourseInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->evaluated = target.evaluated;
            this->score = target.score;
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->lessonId, target.lessonId);
            ::strcpy(this->studentId, target.studentId);
        }
    } StudentCourseInfo;

    typedef
    struct EvaluationIndex // ����ָ��
    {
        uint storageId{};                               // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                             // �Ƿ�ɾ��
        double weight{};                                // ����Ȩ��
        char evaluationID[ID_LEN]{'\0'};            // ����ID
        char description[DESCRIPTION_LEN]{'\0'};    // ��������

        EvaluationIndex() = default;

        EvaluationIndex(const EvaluationIndex &target) {
            this->deepCopy(target);
        }

        EvaluationIndex &operator=(const EvaluationIndex &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::evaluationID) << left << this->evaluationID;
            cout << setw(DataSize::weight) << left << this->weight;
            cout << left << this->description << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::evaluationID) << left << "��׼ID";
            cout << setw(DataSize::weight) << left << "Ȩ��";
            cout << left << "��ϸ����" << endl;
        }

    private:
        void deepCopy(const EvaluationIndex &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->weight = target.weight;
            ::memset(this->description, '\0', sizeof(this->description));
            ::memset(this->evaluationID, '\0', sizeof(this->evaluationID));
            ::strcpy(this->evaluationID, target.evaluationID);
            ::strcpy(this->description, target.description);
        }
    } EvaluationIndex;

    typedef struct EvaluationData // ��������
    {
        uint storageId{};                           // �洢id������ʶ��Ͷ�λ
        bool usageStatus{};                         // �Ƿ�ɾ��
        char studentName[NAME_LEN]{'\0'};       // ѧ������
        char evaluationID[ID_LEN]{'\0'};        // ����ID
        char studentId[STUDENT_ID_LEN]{'\0'};   // ѧ��id
        char lessonName[NAME_LEN]{'\0'};        // �γ���Ϣ
        char lessonId[FULL_NAME_LEN]{'\0'};     // �γ�id
        double weight{};                            // ����Ȩ��
        double score{};                    // ���۵ȼ�

        EvaluationData() = default;

        EvaluationData(const EvaluationData &target) {
            this->deepCopy(target);
        }

        EvaluationData &operator=(const EvaluationData &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::storageId) << left << this->storageId;
            cout << setw(DataSize::evaluationID) << left << this->evaluationID;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::lessonId) << left << this->lessonId;
            cout << setw(DataSize::lessonName) << left << this->lessonName;
            cout << setw(DataSize::level) << left << this->score << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::storageId) << left << "ID";
            cout << setw(DataSize::evaluationID) << left << "����ID";
            cout << setw(DataSize::studentId) << left << "ѧ��";
            cout << setw(DataSize::studentName) << left << "ѧ������";
            cout << setw(DataSize::lessonId) << left << "�γ�ID";
            cout << setw(DataSize::lessonName) << left << "�γ�����";
            cout << setw(DataSize::level) << left << "���۷���" << endl;
        }

    private:
        void deepCopy(const EvaluationData &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->score = target.score;
            this->weight = target.weight;
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::memset(this->evaluationID, '\0', sizeof(this->evaluationID));
            ::strcpy(this->evaluationID, target.evaluationID);
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->studentId, target.studentId);
            ::strcpy(this->lessonId, target.lessonId);
        }
    } EvaluationData;


}

// Ϊ���ֽṹ��Ԫ�������
using Course = DataType::CourseInfo;
using Student = DataType::StudentInfo;
using Teacher = DataType::TeacherInfo;
using StudentCourse = DataType::StudentCourseInfo;
using EvaluationIndex = DataType::EvaluationIndex;
using Evaluation = DataType::EvaluationData;
using Admin = DataType::Admin;

#endif