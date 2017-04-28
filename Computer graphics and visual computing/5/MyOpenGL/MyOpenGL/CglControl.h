#pragma once

class COpenGL;
class CglCamera;

class AFX_API_EXPORT CglControl
{
public:
	//按键状态；
	enum KEYSTATUS {
		KS_M_RIGHT, KS_M_LEFT, KS_M_UP, KS_M_DOWN, KS_M_BACK, KS_M_FRONT, \
		KS_R_LEFT, KS_R_RIGHT, KS_R_UP, KS_R_DOWN, KS_R_FRONT, KS_R_BACK, \
		KS_M_STEP_N, KS_M_STEP_P, KS_R_STEP_N, KS_R_STEP_P, \
		KS_RESTORE, KS_ALL
	}KeyVal;
	bool m_keyStatus[KS_ALL];

	//鼠标按键状态
	bool m_bMouseLDown;
	bool m_bMouseRDown;
	CPoint m_MousePos;

	COpenGL *m_pOpenGL;
	CglCamera *m_pCamera;

	double m_fStep[2];//移动和旋转步长缩放倍率
	float m_fKeyStep;//键盘按下一个运动命令代表移动步长

	CglControl();
	~CglControl();

	void SetSpeed(float ms, float rs);
	bool KeyboardCtrlView();

	bool PreTranslateMessage(unsigned int message, unsigned int wParam, unsigned int lParam);
	CglControl(COpenGL *pGL);

private:

	bool SetKeyStatus(unsigned int nChar, bool bVal);
	bool MouseCtrlView(int message, CPoint pt);
};

