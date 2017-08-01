#include "stdafx.h"
#include "glPictureLoad.h"	

CglPictureLoad::CglPictureLoad()
{
	m_pPict = NULL;
	m_bInit=FALSE;
	m_iTransparence=-1;
}
VOID CglPictureLoad::Init()
{
	m_hdc=wglGetCurrentDC();
	int mx = GetDeviceCaps(m_hdc,HORZSIZE);
	int my = GetDeviceCaps(m_hdc,VERTSIZE);
	int px = GetDeviceCaps(m_hdc,HORZRES);
	int py = GetDeviceCaps(m_hdc,VERTRES);
	m_PixelToMillimeterX = float(mx)*100/px;
	m_PixelToMillimeterY = float(my)*100/py;
	m_bInit = TRUE;
}
CglPictureLoad::~CglPictureLoad()
{
	if(m_bInit)
	{
		UnloadPicture();
	}
}
//-----------------------------------------------------------------------------
// Does: Free The Allocated Memory That Holdes The IPicture Interface Data
// ~~~~ And Clear Picture Information
//
// Note: This Might Also Be Useful If U Only Need To Show The Picture Once
// ~~~~~ Or If U Copy The Picture To The Device Context, So It Can Still
// Remain On Screen - But IPicture Data Is Not Needed No More
//
//-----------------------------------------------------------------------------
void CglPictureLoad::UnloadPicture()
//=============================================================================
{
	if(m_pPict != NULL){
		m_pPict->Release();
		m_pPict = NULL;
	}
}
//-----------------------------------------------------------------------------
// Does: Open a Resource And Load It Into IPicture (Interface)
// ~~~~ (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// Note: When Adding a Bitmap Resource It Would Automatically Show On "Bitmap"
// ~~~~ This NOT Good Coz We Need To Load It From a Custom Resource "BMP"
// To Add a Custom Rresource: Import Resource -> Open As -> Custom
// (Both .BMP And .DIB Should Be Found Under "BMP")
//
// InPut: ResourceName - As a unsigned  Defined (Example: IDR_PICTURE_RESOURCE)
// ~~~~~ ResourceType - Type Name (Example: "JPG")
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL CglPictureLoad::Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType)
//=============================================================================
{
	HGLOBAL hGlobal = NULL;
	HRSRC hSource = NULL;
	LPVOID lpVoid = NULL;
	int nSize = 0;
	BOOL bResult=FALSE;
	if(m_pPict != NULL) UnloadPicture(); // Important - Avoid Leaks...
	
	hSource = FindResource(hInstance, lpszResourceName, ResourceType);
	
	if(hSource == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "FindResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	hGlobal = LoadResource(hInstance, hSource);
	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "LoadResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "LockResource() Failed\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	nSize = (unsigned )SizeofResource(hInstance, hSource);
	if(LoadPictureData((BYTE*)hGlobal, nSize)) bResult = TRUE;
	
	UnlockResource(hGlobal); // 16Bit Windows Needs This
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	return(bResult);
}
//-----------------------------------------------------------------------------
// Does: Open a File And Load It Into IPicture (Interface)
// ~~~~ (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// InPut: sFilePathName - Path And FileName Target To Save
// ~~~~~ 
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL CglPictureLoad::Load(CString sFilePathName)
//=============================================================================
{
	//	if(!PathFileExists(sFilePathName))return FALSE;
	BOOL bResult = FALSE;
	CFile PictureFile;
	CFileException e;
	int nSize = 0;
	
	if(m_pPict != NULL) UnloadPicture(); // Important - Avoid Leaks...
	
	if(PictureFile.Open(sFilePathName, CFile::modeRead | CFile::typeBinary, &e))
	{
		nSize = int(PictureFile.GetLength());
		BYTE* pBuffer = new BYTE[nSize];
		
		if(PictureFile.Read(pBuffer, nSize) > 0)
		{
			if(LoadPictureData(pBuffer, nSize)) bResult = TRUE;
		}
		
		PictureFile.Close();
		delete [] pBuffer;
	}
	else // Open Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
		//		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		//		MessageBoxEx(hWnd, szCause,NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}
	return(bResult);
}
//-----------------------------------------------------------------------------
// Does: Read The Picture Data From a Source (File / Resource)
// ~~~~ And Load It Into The Current IPicture Object In Use
//
// InPut: Buffer Of Data Source (File / Resource) And Its Size
// ~~~~~ 
//
// OutPut: Feed The IPicture Object With The Picture Data
// ~~~~~~ (Use Draw Functions To Show It On a Device Context)
// TRUE If Succeeded...
//-----------------------------------------------------------------------------
BOOL CglPictureLoad::LoadPictureData(BYTE *pBuffer, int nSize)
//=============================================================================
{	
	BOOL bResult = FALSE;
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	
	if(hGlobal == NULL)
	{
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "Can not allocate enough memory\t", NULL,/*ERROR_TITLE*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		return(FALSE);
	}
	
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, pBuffer, nSize);
	GlobalUnlock(hGlobal);
	
	IStream* pStream = NULL;
	
	if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		HRESULT hr;
		if((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPict)) == E_NOINTERFACE)
		{
			HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
			MessageBoxEx(hWnd, "IPicture interface is not supported\t", NULL, /*ERROR_TITLE, */MB_OK | MB_ICONSTOP, LANG_ENGLISH);
			return(FALSE);
		}
		else // S_OK
		{
			pStream->Release();
			pStream = NULL;
			bResult = TRUE;
		}
	}
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	CalculateWidthHeight();
	return(bResult);
}

//-----------------------------------------------------------------------------
// Does: Draw The Loaded Picture Direct To The Client DC
// ~~~~
//
// Note: Bigger OR Smaller Dimentions Than The Original Picture Size
// ~~~~ Will Draw The Picture Streached To Its New Given NEW Dimentions...
//
// InPut: pDC - Given DC To Draw On
// ~~~~~ pSrcRect- Dimentions Of The Picture To Draw From(As a Rectangle)
// DrawRect - Dimentions Of The Picture To Draw To(As a Rectangle)
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
//=============================================================================
void CglPictureLoad::Render(CDC* pDC, LPRECT pDrawRect, LPRECT pSrcRect/*=NULL*/,LPCRECT prcWBounds/*=NULL*/)
{
	if (pDC == NULL || m_pPict == NULL) return ;
	
	CRect recrDest(pDrawRect);
	
	long Width = 0;
	long Height = 0;
	m_pPict->get_Width(&Width);
	m_pPict->get_Height(&Height);
	
	CRect SrcRect(0,0,Width,Height);
	
	if(pSrcRect)
	{
		SrcRect=*pSrcRect;
	}
	CRect DrawRect(pDrawRect);
	HRESULT hrP = NULL;
	
	hrP = m_pPict->Render(pDC->m_hDC,
		DrawRect.left, // Left
		DrawRect.top, // Top
		DrawRect.Width(), // Right
		DrawRect.Height(), // Bottom
		SrcRect.left,
		SrcRect.bottom,
		SrcRect.Width(),
		-SrcRect.Height(),
		prcWBounds
		);
	if (SUCCEEDED(hrP)) return;
	AfxThrowMemoryException();
	return;
}
// Does: Saves The Picture That Is Stored In The IPicture Object As a Bitmap
// ~~~~ (Converts From Any Known Picture Type To a Bitmap / Icon File)
//
// InPut: sFilePathName - Path And FileName Target To Save
// ~~~~~
//
// OutPut: TRUE If Succeeded...
// ~~~~~~
//-----------------------------------------------------------------------------
BOOL CglPictureLoad::SaveAsBitmap(CString sFilePathName)
//=============================================================================
{
	BOOL bResult = FALSE;
	ILockBytes *Buffer = 0;
	IStorage *pStorage = 0;
	IStream *FileStream = 0;
	BYTE *BufferBytes;
	STATSTG BytesStatistics;
	DWORD OutData;
	long OutStream;
	CFile BitmapFile; 
	CFileException e;
	double SkipFloat = 0;
	DWORD ByteSkip = 0;
	_ULARGE_INTEGER RealData;
	
	CreateILockBytesOnHGlobal(NULL, TRUE, &Buffer); // Create ILockBytes Buffer
	
	HRESULT hr = ::StgCreateDocfileOnILockBytes(Buffer,
		STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &pStorage);
	
	hr = pStorage->CreateStream(L"PICTURE",
		STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, 0, &FileStream);
	
	m_pPict->SaveAsFile(FileStream, TRUE, &OutStream); // Copy Data Stream
	FileStream->Release();
	pStorage->Release();
	Buffer->Flush(); 
	
	// Get Statistics For Final Size Of Byte Array
	Buffer->Stat(&BytesStatistics, STATFLAG_NONAME);
	
	// Cut UnNeeded Data Coming From SaveAsFile() (Leave Only "Pure" Picture Data)
	SkipFloat = (double(OutStream) / 512); // Must Be In a 512 Blocks...
	if(SkipFloat > DWORD(SkipFloat)) 
		ByteSkip = (DWORD)SkipFloat + 1;
	else ByteSkip = (DWORD)SkipFloat;
	ByteSkip = ByteSkip * 512; // Must Be In a 512 Blocks...
	
	// Find Difference Between The Two Values
	ByteSkip = (DWORD)(BytesStatistics.cbSize.QuadPart - ByteSkip);
	
	// Allocate Only The "Pure" Picture Data
	RealData.LowPart = 0;
	RealData.HighPart = 0;
	RealData.QuadPart = ByteSkip;
	BufferBytes = (BYTE*)malloc(OutStream);
	if(BufferBytes == NULL)
	{
		Buffer->Release();
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, "Can not allocate enough memory\t",NULL, /*ERROR_TITLE,*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
	}
	
	Buffer->ReadAt(RealData, BufferBytes, OutStream, &OutData);
	
	if(BitmapFile.Open(sFilePathName, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite, &e))
	{
		BitmapFile.Write(BufferBytes, OutData);
		BitmapFile.Close();
		bResult = TRUE;
	}
	else // Write File Failed...
	{
		TCHAR szCause[255];
		e.GetErrorMessage(szCause, 255, NULL);
		HWND hWnd = AfxGetApp()->GetMainWnd()->m_hWnd;
		MessageBoxEx(hWnd, szCause, NULL, /*ERROR_TITLE,*/ MB_OK | MB_ICONSTOP, LANG_ENGLISH);
		bResult = FALSE;
	}
	
	Buffer->Release();
	free(BufferBytes);
	
	return(bResult);
}

LONG CglPictureLoad::get_Height()
{
	LONG nHeight = 0;
	if (m_pPict != NULL)
	{
		m_pPict->get_Height(&nHeight);
	}
	return nHeight;
}
int CglPictureLoad::GetHeight()
{
	return m_Height;
}
int CglPictureLoad::GetWidth()
{
	return m_Width;
}
VOID CglPictureLoad::CalculateWidthHeight()
{
	m_Height=int(get_Height()/m_PixelToMillimeterY);
	m_Width=int(get_Width()/m_PixelToMillimeterX);		
}
LONG CglPictureLoad::get_Width()
{
	LONG nWidth = 0;
	if (m_pPict != NULL)
	{
		m_pPict->get_Width(&nWidth);
	}
	return nWidth;
}

VOID * CglPictureLoad::GetTGAData(CString sFilePathName)
{
	CglImageData *pImageData;
	WORD width = 0, height = 0;			// The dimensions of the image
	byte length = 0;					// The length in bytes to the pixels
	byte imageType = 0;					// The image type (RLE, RGB, Alpha...)
	byte bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
	int i = 0;							// A counter
	if((pFile = fopen(sFilePathName, "rb")) == NULL) 
	{
		return 0;
	}
	pImageData = new CglImageData;
	fread(&length, sizeof(byte), 1, pFile);
	fseek(pFile,1,SEEK_CUR); 
	fread(&imageType, sizeof(byte), 1, pFile);
	// Skip past general information we don't care about
	fseek(pFile, 9, SEEK_CUR); 
	
	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(byte), 1, pFile);
	fseek(pFile, length + 1, SEEK_CUR); 
	
	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = 4;
			stride = channels * width;
			pImageData->m_pData = new unsigned char[stride * height];
			
			for(int y = 0; y < height; y++)
			{
				unsigned char *pLine = &(pImageData->m_pData[stride * y]);
				
				
				
				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for(i = 0; i < stride; i += channels)
				{
					fread(pLine+i, bits/8, 1, pFile);
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
					pLine[i+3]=m_cTransDegree;
					if(m_iTransparence!=-1)//带有透明色
					{
						if(unsigned int(m_iTransparence)==RGB(pLine[i],pLine[i+1],pLine[i+2]))
							pLine[i+3]=0;						
					}
					if(m_fAlpha<0.999)//需要纹理和光照合成
					{
						for(int j=0;j<3;j++)
							pLine[i+j] = unsigned char (pLine[i+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
					}
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;
			
			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 4;
			stride = channels * width;
			pImageData->m_pData = new unsigned char[stride * height];
			
			for(int i = 0; i < width*height; i++)
			{
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x3f) << 2;
				r = ((pixels >> 11) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->m_pData[i * 3 + 0] = r;
				pImageData->m_pData[i * 3 + 1] = g;
				pImageData->m_pData[i * 3 + 2] = b;
				
				pImageData->m_pData[i*3+3]=m_cTransDegree;
				if(m_iTransparence!=-1)
				{
					if(unsigned int(m_iTransparence)==RGB(pImageData->m_pData[i*3],pImageData->m_pData[i*3+1],pImageData->m_pData[i*3+2]))
						pImageData->m_pData[i*3+3]=0;						
				}
				if(m_fAlpha<0.999)//需要纹理和光照合成
				{
					for(int j=0;j<3;j++)
						pImageData->m_pData[i*3+j] = unsigned char (pImageData->m_pData[i*3+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
				}

			}
		}	
		else
		{
			return false;
		}
	}
	else
	{
		// Create some variables to hold the rleID, current colors read, channels, & stride.
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;
		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->m_pData = new unsigned char[stride * height];
		byte *pColors = new byte [channels];
		
		while(i < width*height)
		{
			fread(&rleID, sizeof(byte), 1, pFile);
			
			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				rleID++;
				
				while(rleID)
				{
					fread(pColors, sizeof(byte) * channels, 1, pFile);
					
					// Store the current pixel in our image array
					pImageData->m_pData[colorsRead + 0] = pColors[2];
					pImageData->m_pData[colorsRead + 1] = pColors[1];
					pImageData->m_pData[colorsRead + 2] = pColors[0];
					
					// If we have a 4 channel 32-bit image, assign one more for the alpha
					pImageData->m_pData[colorsRead + 3] = m_cTransDegree;
					if(m_fAlpha<0.999)//需要纹理和光照合成
					{
						for(int j=0;j<3;j++)
							pImageData->m_pData[colorsRead+j] = unsigned char (pImageData->m_pData[colorsRead+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
					}
					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;
				
				fread(pColors, sizeof(byte) * channels, 1, pFile);
				
				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->m_pData[colorsRead + 0] = pColors[2];
					pImageData->m_pData[colorsRead + 1] = pColors[1];
					pImageData->m_pData[colorsRead + 2] = pColors[0];
					
					// If we have a 4 channel 32-bit image, assign one more for the alpha
					pImageData->m_pData[colorsRead + 3] = m_cTransDegree;
					if(m_fAlpha<0.999)//需要纹理和光照合成
					{
						for(int j=0;j<3;j++)
							pImageData->m_pData[colorsRead+j] = unsigned char (pImageData->m_pData[colorsRead+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
					}

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
		}
		delete[] pColors;
	}
	
	fclose(pFile);
	
	// Fill in our tImageTGA structure to pass back
	pImageData->m_nColor = channels;
	pImageData->m_Width  = width;
	pImageData->m_Height = height;
	return pImageData;
}
VOID * CglPictureLoad::GetBMPData(CString sFilePathName)
{
	CglImageData *pImageData;
	FILE *fp = NULL;
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	fp=fopen(sFilePathName,"rb");
	if(!fp)
		return 0;	
	fread(&bfh,sizeof(bfh),1,fp);
	fread(&bih,sizeof(bih),1,fp);
	int cbRow = bih.biWidth*bih.biBitCount/8;
	int cbTrueRow = ((cbRow + 3) & ~3);
	int cbSize=bih.biWidth*bih.biHeight*4;
	int ColorByte = bih.biBitCount/8;
	
	pImageData = new CglImageData;
	pImageData->m_pData=new unsigned char[cbSize];
	pImageData->m_Width=bih.biWidth;
	pImageData->m_Height=bih.biHeight;
	pImageData->m_nColor=4;
	
	unsigned char *buf;
	buf = new unsigned char[cbTrueRow];
	fseek(fp,bfh.bfOffBits,SEEK_SET);
	int offset=0;
	for(int i=0;i< bih.biHeight;i++)
	{
		fread(buf,1,cbTrueRow,fp);//读入一行的数据
		int lineoffset=0;
		for(int j=0;j<bih.biWidth;j++)
		{
			pImageData->m_pData[offset+0]=buf[lineoffset+2];
			pImageData->m_pData[offset+1]=buf[lineoffset+1];
			pImageData->m_pData[offset+2]=buf[lineoffset+0];
			pImageData->m_pData[offset+3]=m_cTransDegree;
			if(m_iTransparence!=-1)
			{
				if(unsigned int(m_iTransparence)==RGB(buf[lineoffset+2],buf[lineoffset+1],buf[lineoffset]))
					pImageData->m_pData[offset+3]=0;
			}
			if(m_fAlpha<0.999)//需要纹理和光照合成
			{
				for(int j=0;j<3;j++)
					pImageData->m_pData[offset+j] = unsigned char (pImageData->m_pData[offset+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
			}

			offset+=4;lineoffset+=ColorByte;

		}
	}
	fclose(fp);
	delete buf;
	return pImageData;
}
VOID * CglPictureLoad::GetOtherData(CString sFilePathName)
{
	if(!Load(sFilePathName))
		return 0;
	CglImageData *pImageData;
	pImageData = new CglImageData;
	pImageData->m_Width=m_Width;
	pImageData->m_Height=m_Height;
	CRect rect(0,0,m_Width,m_Height);
	
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(CDC::FromHandle(m_hdc),m_Width,m_Height);
	CDC dc;
	dc.CreateCompatibleDC(CDC::FromHandle(m_hdc));
	dc.SelectObject(bitmap);
	Render(&dc,rect);
	BITMAP bm;
	bitmap.GetBitmap(&bm);
	int nColor = bm.bmBitsPixel/8;
	unsigned char *data;
	data = new unsigned char[m_Width * m_Height * nColor];
	bitmap.GetBitmapBits(m_Width * m_Height * nColor, data);
	
	if(nColor==3||nColor==4)
	{
		pImageData->m_nColor=4;
		pImageData->m_pData = new unsigned char[m_Width * m_Height * 4];
		int stride = nColor*m_Width;
		memcpy(pImageData->m_pData,data,m_Width * m_Height * nColor);
		for(int y=0;y<m_Height;y++)
		{
			int y2=m_Height-y-1;
			unsigned char *pLine = &(pImageData->m_pData[stride * y]);
			unsigned char *pLine2 = &(data[stride * y2]);
			for(int i = 0; i < stride; i += nColor)
			{
				
				pLine[i]=pLine2[i+2];
				pLine[i+1]=pLine2[i+1];
				pLine[i+2]=pLine2[i];
				pLine[i+3]=m_cTransDegree;
				if(m_iTransparence!=-1)
				{
					if(unsigned int(m_iTransparence) == RGB(pLine[i],pLine[i+1],pLine[i+2]))
						pLine[i+3]=0;
				}
				if(m_fAlpha<0.999)//需要纹理和光照合成
				{
					for(int j=0;j<3;j++)
						pLine[i+j] = unsigned char (pLine[i+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
				}
			}
		}
	}
	else if(nColor==2)
	{
		pImageData->m_nColor=4;
		int r=0, g=0, b=0;
		unsigned short pixels = 0;
		pImageData->m_pData = new unsigned char[m_Width * m_Height * 4];
		int stride1 = pImageData->m_nColor*m_Width;
		int stride2 = 2*m_Width;
		for(int y=0;y<m_Height;y++)
		{
			int y2=m_Height-y-1;
			unsigned char *pLine = &(pImageData->m_pData[stride1 * y]);
			unsigned char *pLine2 = &(data[stride2 * y2]);
			for(int i = 0, j = 0; i < stride1; i += pImageData->m_nColor, j+= 2)
			{
				pixels=MAKEWORD(pLine2[j],pLine2[j+1]);
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x3f) << 2;
				r = ((pixels >> 11) & 0x1f) << 3;
				
				pLine[i]=r;
				pLine[i+1]=g;
				pLine[i+2]=b;
				pLine[i+3]=m_cTransDegree;
				if(m_iTransparence!=-1)
				{
					if(unsigned int(m_iTransparence)==RGB(pLine[i],pLine[i+1],pLine[i+2]))
						pLine[i+3]=0;
				}
				if(m_fAlpha<0.999)//需要纹理和光照合成
				{
					for(int j=0;j<3;j++)
						pLine[i+j] = unsigned char (pLine[i+j]*m_fAlpha + m_diffuse[j]*(1-m_fAlpha));
				}
			}			
		}
	}
	delete data;
	return pImageData;
}
void * CglPictureLoad::GetData(CString sFilePathName,int transcolor,unsigned char transdegree,float fAlpha,float *diffuse)
{
	int i;
	for(i=0;i<3;i++)
		m_diffuse[i]=diffuse[i];
	m_fAlpha = fAlpha;
	if(!m_bInit)
		Init();	
	m_iTransparence= transcolor;	//透明色
	m_cTransDegree = transdegree;	//透明度
	sFilePathName.MakeLower();
	//得到扩展名.
	char extname[4];
	int endpos=sFilePathName.GetLength();
	extname[0]=sFilePathName.GetAt(endpos-3);
	extname[1]=sFilePathName.GetAt(endpos-2);
	extname[2]=sFilePathName.GetAt(endpos-1);
	extname[3]=0;
	if(!strcmp(extname,"bmp"))
	{
		return GetBMPData(sFilePathName);
	}
	else if(!strcmp(extname,"tga"))
	{
		return GetTGAData(sFilePathName);
	}
	else if(!strcmp(extname,"dib")||!strcmp(extname,"emf")||!strcmp(extname,"gif")||!strcmp(extname,"ico")||!strcmp(extname,"jpg")||!strcmp(extname,"wmf"))//.DIB .EMF .GIF .ICO .JPG .WMF
	{
		return GetOtherData(sFilePathName);
	}
	//	else return 0;	//如果找不到合适的结构，则采用BMP方法读取。
	else
	{
		return GetBMPData(sFilePathName);
	}
}