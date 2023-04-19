/**
 * @author Half_nothing
 * @date 2023.02
 * @details 存放宏定义和类型定义
 */
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <windows.h>
#include <conio.h>
#include <cstring>
#include <algorithm>

// 定义列表每一页展示的最大数量
#define LINE_PER_PAGE 20

// 改变控制台背景色
#define IN_CYAN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define IN_DEFAULT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

// 方向键键位定义
#define UP_KEY 72
#define LEFT_KEY 75
#define RIGHT_KEY 77
#define DOWN_KEY 80

// 实现暂停等待输入
#define PAUSE system("pause")

// 清屏
#define CLEAR system("cls")

// 类型定义
typedef unsigned int uint;

// 最大长度宏定义
#define TEACHER_ID_LEN 10
#define ID_LEN 20
#define NAME_LEN 30
#define STUDENT_ID_LEN 20
#define PASSWORD_LEN 50
#define FULL_NAME_LEN 100
#define DESCRIPTION_LEN 200

class UserManager;

#endif
