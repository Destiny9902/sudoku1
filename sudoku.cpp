// sudoku.cpp: 定义控制台应用程序的入口点。
//


#include "stdafx.h"

#include <stdio.h>

#include <string.h>

#include <time.h>

#include <iostream>

#include <stdlib.h>



using namespace std;

int Board[30][9][9];
int Try_List[9];

//读取输入的字符串
int read_write_in(char str[]) {



	int num = 0, len = strlen(str);

	for (int i = 0; i < len; i++) {

		if (str[i]>'9' || str[i] < '0')    //输入的是字符型

			return 0;

		num *= 10;

		num += str[i] - '0';               //将字符转为数字

		if (num > 1000000 || num == 0)     //输入的数字不在题目要求范围内

			return 0;

	}

	return num;

}
//交换两个数
void swap(int &a, int &b) {



	int temp = a;

	a = b;

	b = temp;



}
//随机生成全排列
int init_random(int num[]) {



	for (int i = 0; i < 9; i++)    //顺序填入1~9

		num[i] = i + 1;

	for (int i = 0; i < 20; i++) {

		int j = rand() % 9;

		swap(num[j], num[8]);    //将随机生成的数字与最后一个数字交换

	}



	return 0;

}
//判断填充是否合法
int legal(int s, int x, int y, int num) {

	int i = 0;
	int gongge_x = x - x % 3, gongge_y = y - y % 3;    //计算所处宫格左上角坐标

	for (i = 0; i < 9; i++) {

		if (Board[s][x][i] == num)  //当前行合法

			return 0;

		if (Board[s][i][y] == num) //当前列合法

			return 0;

	}


	for (int i = gongge_x; i < gongge_x + 3; i++)    //当前小宫格合法

		for (int j = gongge_y; j < gongge_y + 3; j++)

			if (Board[s][i][j] == num)

				return 0;



	return 1;

}
//回溯用于填充格子
int fill(int s, int x, int y) {

	int init = Board[s][x][y];    //记录初值

	int next_x = x + (y + 1) / 9, next_y = (y + 1) % 9;    //下一个小格子坐标



	if (x >= 9)    //全部完成

		return 1;

	if (Board[s][x][y]) {    //当前格已填充

		if (fill(s, next_x, next_y))

			return 1;

	}

	else {    //当前为空格

		for (int i = 0; i < 9; i++) {

			int try_num = Try_List[i];    //当前尝试数字

			if (legal(s, x, y, try_num)) {    //合法的判断

				Board[s][x][y] = try_num;

				if (fill(s, next_x, next_y))

					return 1;

			}

		}

	}

	Board[s][x][y] = init;    //回溯还原

	return 0;

}
//行随机交换
void swap_row(int s, int beg, int end) {



	for (int i = 0; i < 5; i++) {

		int j = rand() % (end - beg + 1) + beg;

		for (int k = 0; k < 9; k++)

			swap(Board[s][j][k], Board[s][end][k]);

	}



}
//列随机交换
void swap_col(int s, int beg, int end) {



	for (int i = 0; i < 5; i++) {

		int j = rand() % (end - beg + 1) + beg;

		for (int k = 0; k < 9; k++)

			swap(Board[s][k][j], Board[s][k][end]);

	}



}
//生成终局
int generate(int sudoku_num) {



	freopen("sudoku.txt", "w", stdout);

	srand((unsigned)time(NULL));    //时间种子

	while (sudoku_num>0) {

		memset(Board, 0, sizeof(Board));    //清空宫格

		for (int i = 0; i < 20; i++) {    //每轮生成20个种子矩阵

			init_random(Board[i][0]);    //随机初始化第一行数字

			for (int j = 0; j < 9; j++)

				if (Board[i][0][j] == 7) {    //学号后两位15 (1+5)%9+1=7

					swap(Board[i][0][0], Board[i][0][j]);

					break;

				}

			init_random(Try_List);    //随机初始化数字尝试顺序

			fill(i, 1, 0);    //开始填充

		}

		int extend = 1000;

		while (extend--) {    //扩展1000个终局

			if (sudoku_num-- == 0)

				break;

			int sand = rand() % 20;

			swap_col(sand, 1, 2), swap_row(sand, 1, 2);

			swap_col(sand, 3, 5), swap_row(sand, 3, 5);

			swap_col(sand, 6, 8), swap_row(sand, 6, 8);

			for (int i = 0; i < 9; i++)   //打印

				for (int j = 0; j < 10; j++) {

					if (j == 9)

						putchar('\n');

					else {

						putchar(Board[sand][i][j] + '0');

						putchar(' ');

					}

				}

			if (sudoku_num)

				putchar('\n');

		}

	}

	fclose(stdout);



	return 0;

}
//求解数独
int solve(char File[]) {



	freopen(File, "r", stdin);

	freopen("sudoku.txt", "w", stdout);

	srand((unsigned)time(NULL));

	init_random(Try_List);

	int i = 0, j = 0;

	while (~scanf_s("%d", &Board[0][i][j])) {

		i += (j + 1) / 9;    //下一数字坐标

		j = (j + 1) % 9;

		if (i == 9) {    //读入完毕 求解数独

			fill(0, 0, 0);

			i = j = 0;

			for (int i = 0; i < 9; i++)   //打印

				for (int j = 0; j < 10; j++) {

					if (j == 9)

						putchar('\n');

					else {

						putchar(Board[0][i][j] + '0');

						putchar(' ');

					}

				}

			putchar('\n');

		}

	}

	fclose(stdin);

	fclose(stdout);



	return 0;

}

int main(int argc, char *argv[]) {



	if (argc < 2) {    //参数异常处理

		printf("请输入参数\n");

		return 0;

	}

	if (strcmp(argv[1], "-c") == 0) {    //生成终局命令

		if (int sudoku_num = read_write_in(argv[2]))

			generate(sudoku_num);    //生成终局

		else

			printf("请输入1-1000000之间的数字\n");    //异常处理

	}

	else if (strcmp(argv[1], "-s") == 0) {    //求解残局命令

		solve(argv[2]);

	}



	return 0;

}