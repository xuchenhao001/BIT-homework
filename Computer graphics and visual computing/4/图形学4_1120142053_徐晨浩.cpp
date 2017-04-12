// 图形学4_1120142053_徐晨浩.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include "MyOpenGlClass.h"

using namespace std;

//读取test.txt文本到字符串数组operate数组里
void readTxt(string *operate, int &lineNum) {
	ifstream in("test.txt");
	string line;
	int i = 0;
	in.imbue(locale("chs"));
	if (in) {
		while (getline(in, line)) {
			operate[i] = line;
			i++;
		}
		lineNum = i;
	}
	else {
		printf("未找到test.txt文件！\n");
		system("Pause");
		exit(1);
	}
}

//将string数组operate中的字符串写入out.txt文本
void writeTxt(string *operate, int lineNum) {
	ofstream out("out.txt", ios::trunc);
	for (int i = 0; i < lineNum; i++) {
		out << operate[i] << endl;
	}
	out.close();
}

//从字符串中读取矩阵
void readMatrix(string value, CMatrix & matrix) {
	sscanf_s(value.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f, \
			%f,%f,%f,%f,%f,%f,%f,%f", \
		&matrix.m00, &matrix.m01, &matrix.m02, &matrix.m03, \
		&matrix.m10, &matrix.m11, &matrix.m12, &matrix.m13, \
		&matrix.m20, &matrix.m21, &matrix.m22, &matrix.m23, \
		&matrix.m30, &matrix.m31, &matrix.m32, &matrix.m33);
}

//向result字符串中写入矩阵
void writeMatrix(char* result, CMatrix & matrix) {
	snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f, \
			%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", \
		matrix.m00, matrix.m01, matrix.m02, matrix.m03, \
		matrix.m10, matrix.m11, matrix.m12, matrix.m13, \
		matrix.m20, matrix.m21, matrix.m22, matrix.m23, \
		matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

//计算矩阵、向量、欧拉角
string compute(string command, string value) {
	char result[50];
	float angle, t;
	CVector3 vect;
	CEuler euler;
	CMatrix matrix;
	CQuaternion quater, quater1, quater2;

	if (command == "欧拉角转换向量") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		vect = euler.ToVector3();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", vect.x, vect.y, vect.z);
	}
	else if (command == "向量转换欧拉角") {
		sscanf_s(value.c_str(), "%f,%f,%f", &vect.x, &vect.y, &vect.z);
		euler = vect.ToEuler();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "欧拉角转换四元数") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		quater = euler.ToQuaternion();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数转换欧拉角") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		euler = quater.ToEuler();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "欧拉角转换矩阵") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		matrix = euler.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "矩阵转换欧拉角") {
		readMatrix(value, matrix);
		euler = matrix.ToEuler();
		writeMatrix(result, matrix);
	}
	else if (command == "矩阵转换四元数") {
		readMatrix(value, matrix);
		matrix.ToQuaternion();
		writeMatrix(result, matrix);
	}
	else if (command == "四元数转换矩阵") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		matrix = quater.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "欧拉角标准化") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		euler.Normal();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "四元数标准化") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Normalize();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数相乘") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f",\
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater*quater1;
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "四元数求差") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater.Div(quater1);
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "四元数点乘") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		float dotResult = quater.dotMul(quater1);
		snprintf(result, 50, "\t%.2f", dotResult);
	}
	else if (command == "四元数求逆") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Inverse();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "四元数求角度和旋转轴") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.GetAngle(angle, vect);
		snprintf(result, 50, "\t%.2f\t%.2f,%.2f,%.2f", angle, vect.x, vect.y, vect.z);
	}
	else if (command == "四元数插值") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f\t%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w, &t);
		quater2 = quater.Slerp(quater1, t);
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	return value.append(result);
}

//调度输入输出和计算过程
void schedule() {
	string operate[50];
	int lineNum;
	readTxt(operate, lineNum);
	for (int i = 0; i < lineNum; i += 2) {
		operate[i + 1] = compute(operate[i], operate[i + 1]);
	}
	writeTxt(operate, lineNum);
}

int main()
{
	schedule();
    return 0;
}

