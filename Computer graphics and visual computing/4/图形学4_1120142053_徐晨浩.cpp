// ͼ��ѧ4_1120142053_�쳿��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include "MyOpenGlClass.h"

using namespace std;

//��ȡtest.txt�ı����ַ�������operate������
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
		printf("δ�ҵ�test.txt�ļ���\n");
		system("Pause");
		exit(1);
	}
}

//��string����operate�е��ַ���д��out.txt�ı�
void writeTxt(string *operate, int lineNum) {
	ofstream out("out.txt", ios::trunc);
	for (int i = 0; i < lineNum; i++) {
		out << operate[i] << endl;
	}
	out.close();
}

//���ַ����ж�ȡ����
void readMatrix(string value, CMatrix & matrix) {
	sscanf_s(value.c_str(), "%f,%f,%f,%f,%f,%f,%f,%f, \
			%f,%f,%f,%f,%f,%f,%f,%f", \
		&matrix.m00, &matrix.m01, &matrix.m02, &matrix.m03, \
		&matrix.m10, &matrix.m11, &matrix.m12, &matrix.m13, \
		&matrix.m20, &matrix.m21, &matrix.m22, &matrix.m23, \
		&matrix.m30, &matrix.m31, &matrix.m32, &matrix.m33);
}

//��result�ַ�����д�����
void writeMatrix(char* result, CMatrix & matrix) {
	snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f, \
			%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", \
		matrix.m00, matrix.m01, matrix.m02, matrix.m03, \
		matrix.m10, matrix.m11, matrix.m12, matrix.m13, \
		matrix.m20, matrix.m21, matrix.m22, matrix.m23, \
		matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

//�������������ŷ����
string compute(string command, string value) {
	char result[50];
	float angle, t;
	CVector3 vect;
	CEuler euler;
	CMatrix matrix;
	CQuaternion quater, quater1, quater2;

	if (command == "ŷ����ת������") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		vect = euler.ToVector3();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", vect.x, vect.y, vect.z);
	}
	else if (command == "����ת��ŷ����") {
		sscanf_s(value.c_str(), "%f,%f,%f", &vect.x, &vect.y, &vect.z);
		euler = vect.ToEuler();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "ŷ����ת����Ԫ��") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		quater = euler.ToQuaternion();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "��Ԫ��ת��ŷ����") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		euler = quater.ToEuler();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "ŷ����ת������") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		matrix = euler.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "����ת��ŷ����") {
		readMatrix(value, matrix);
		euler = matrix.ToEuler();
		writeMatrix(result, matrix);
	}
	else if (command == "����ת����Ԫ��") {
		readMatrix(value, matrix);
		matrix.ToQuaternion();
		writeMatrix(result, matrix);
	}
	else if (command == "��Ԫ��ת������") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		matrix = quater.ToMatrix();
		writeMatrix(result, matrix);
	}
	else if (command == "ŷ���Ǳ�׼��") {
		sscanf_s(value.c_str(), "%f,%f,%f", &euler.h, &euler.p, &euler.b);
		euler.Normal();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f", euler.h, euler.p, euler.b);
	}
	else if (command == "��Ԫ����׼��") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Normalize();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "��Ԫ�����") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f",\
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater*quater1;
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "��Ԫ�����") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		quater2 = quater.Div(quater1);
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	else if (command == "��Ԫ�����") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w);
		float dotResult = quater.dotMul(quater1);
		snprintf(result, 50, "\t%.2f", dotResult);
	}
	else if (command == "��Ԫ������") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.Inverse();
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater.x, quater.y, quater.z, quater.w);
	}
	else if (command == "��Ԫ����ǶȺ���ת��") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f", &quater.x, &quater.y, &quater.z, &quater.w);
		quater.GetAngle(angle, vect);
		snprintf(result, 50, "\t%.2f\t%.2f,%.2f,%.2f", angle, vect.x, vect.y, vect.z);
	}
	else if (command == "��Ԫ����ֵ") {
		sscanf_s(value.c_str(), "%f,%f,%f,%f\t%f,%f,%f,%f\t%f", \
			&quater.x, &quater.y, &quater.z, &quater.w, \
			&quater1.x, &quater1.y, &quater1.z, &quater1.w, &t);
		quater2 = quater.Slerp(quater1, t);
		snprintf(result, 50, "\t%.2f,%.2f,%.2f,%.2f", quater2.x, quater2.y, quater2.z, quater2.w);
	}
	return value.append(result);
}

//������������ͼ������
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

