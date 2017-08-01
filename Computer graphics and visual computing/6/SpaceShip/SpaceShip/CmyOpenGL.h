#pragma once
#include "opengl/OpenGL.h"
#pragma comment(lib,"opengl/MyOpenGL")
#include "CglPlane.h"
#include"SkyBox.h"
#include"CglGarbage.h"
#include"CglPatch.h"
#include <list>
#include <vector>

class CmyOpenGL :public COpenGL {
public:
	CmyOpenGL();
	~CmyOpenGL();

	virtual void PostInit();
	virtual void InDraw();
	//�������л�ͼ���������֮��һ��
	virtual void Update();
	//SkyBox mybox;
private:
	//CglPlane airPlane;
	virtual bool OnKey(unsigned int nChar, bool bDown);
	virtual void DrawModel();
};

//============
//ѭ�����ж���
//============
template <class T>
class cycleQueue {
private:
	unsigned int m_size;
	int m_front;
	int m_now;//�����������
	int m_rear;
	int now_size;
	T*  m_data;
public:
	//���캯��
	cycleQueue(unsigned size) :m_size(size), m_front(0), m_rear(0) {
		m_data = new T[size];
	}

	//��������
	~cycleQueue() {
		delete[] m_data;
	}

	//��ʼ��������
	void reset() {
		m_now = m_front;
	}

	//��ǰ������Ԫ��
	T now() {
		if (!this->isEmpty()) {
			return m_data[m_now];
		}
		return NULL;
	}

	//����������һ��Ԫ��
	T next() {
		if (m_now != m_rear) {
			m_now = (m_now + 1) % m_size;
			return m_data[m_now];
		}
		return m_data[m_now];
	}

	//��ǰ������Ԫ������
	int nowSize() {
		return now_size;
	}

	//����������
	int size() {
		return m_size;
	}

	//ѭ�������Ƿ��
	bool isEmpty() {
		return m_front == m_rear;
	}

	//ѭ�������Ƿ���
	bool isFull() {
		return m_front == (m_rear + 1) % m_size;
	}

	//��β����Ԫ��
	void push(T ele) {
		if (!isFull()) {
			now_size++;
			m_data[m_rear] = ele;
			m_rear = (m_rear + 1) % m_size;
		}
	}

	//��ͷȡ����ɾ��Ԫ��
	T pop() {
		if (!isEmpty()) {
			now_size--;
			T tmp = m_data[m_front];
			m_front = (m_front + 1) % m_size;
			return tmp;
		}
		return NULL;
	}
};
