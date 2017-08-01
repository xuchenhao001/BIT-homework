#pragma once


class AFX_API_EXPORT CglImageData {
public:
	int m_width, m_height, m_bits;
	unsigned char *m_pData;

	CglImageData() {
		m_width = 0;
		m_height = 0;
		m_bits = 0;
		m_pData = 0;
	}
	~CglImageData() {
		m_width = 0;
		m_height = 0;
		m_bits = 0;
		delete m_pData;
		m_pData = 0;
	}
};

class AFX_API_EXPORT CglPicLoad {
public:
	CglPicLoad();
	~CglPicLoad();

	CglImageData* ReadBMP(char *filename);
};

