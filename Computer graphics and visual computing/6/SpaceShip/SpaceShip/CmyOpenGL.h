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
	//更新所有绘图所需参数，之后一起画
	virtual void Update();
	//SkyBox mybox;
private:
	//CglPlane airPlane;
	virtual bool OnKey(unsigned int nChar, bool bDown);
	virtual void DrawModel();
};

//============
//循环队列定义
//============
template <class T>
class cycleQueue {
private:
	unsigned int m_size;
	int m_front;
	int m_now;//定义遍历变量
	int m_rear;
	int now_size;
	T*  m_data;
public:
	//构造函数
	cycleQueue(unsigned size) :m_size(size), m_front(0), m_rear(0) {
		m_data = new T[size];
	}

	//析构函数
	~cycleQueue() {
		delete[] m_data;
	}

	//初始化迭代器
	void reset() {
		m_now = m_front;
	}

	//当前迭代的元素
	T now() {
		if (!this->isEmpty()) {
			return m_data[m_now];
		}
		return NULL;
	}

	//继续迭代下一个元素
	T next() {
		if (m_now != m_rear) {
			m_now = (m_now + 1) % m_size;
			return m_data[m_now];
		}
		return m_data[m_now];
	}

	//当前队列中元素数量
	int nowSize() {
		return now_size;
	}

	//队列总容量
	int size() {
		return m_size;
	}

	//循环队列是否空
	bool isEmpty() {
		return m_front == m_rear;
	}

	//循环队列是否满
	bool isFull() {
		return m_front == (m_rear + 1) % m_size;
	}

	//队尾加入元素
	void push(T ele) {
		if (!isFull()) {
			now_size++;
			m_data[m_rear] = ele;
			m_rear = (m_rear + 1) % m_size;
		}
	}

	//队头取出并删除元素
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
