/**
 * @author Half_nothing
 * @date 2023.02
 * @details ��ź궨������Ͷ���
 */
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <windows.h>
#include <conio.h>
#include <cstring>
#include <algorithm>

// �����б�ÿһҳչʾ���������
#define LINE_PER_PAGE 20

// �ı����̨����ɫ
#define IN_CYAN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define IN_DEFAULT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

// �������λ����
#define UP_KEY 72
#define LEFT_KEY 75
#define RIGHT_KEY 77
#define DOWN_KEY 80

// ʵ����ͣ�ȴ�����
#define PAUSE system("pause")

// ����
#define CLEAR system("cls")

// ���Ͷ���
typedef unsigned int uint;

// ��󳤶Ⱥ궨��
#define TEACHER_ID_LEN 10
#define ID_LEN 20
#define NAME_LEN 30
#define STUDENT_ID_LEN 20
#define PASSWORD_LEN 50
#define FULL_NAME_LEN 100
#define DESCRIPTION_LEN 200

class UserManager;

#endif
