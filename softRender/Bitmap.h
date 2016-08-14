// only suitable for 256-color or 24-bit bitmap

#ifndef BITMAP_H

#define BITMAP_H


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;

typedef struct tagBITMAPFILEHEADER {

	WORD           bfType;
	DWORD          bfSize;
	WORD           bfReserved1;
	WORD           bfReserved2;
	DWORD          bfOffBits;

} BITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER{

	DWORD          biSize;
	LONG           biWidth;
	LONG           biHeight;
	WORD           biPlanes;
	WORD           biBitCount;
	DWORD          biCompression;
	DWORD          biSizeImage;
	LONG           biXPelsPerMeter;
	LONG           biYPelsPerMeter;
	DWORD          biClrUsed;
	DWORD          biClrImportant;

} BITMAPINFOHEADER;


typedef struct tagRGBQUAD {

	BYTE		  rgbBlue; 
	BYTE		  rgbGreen; 
	BYTE		  rgbRed;   
	BYTE		  rgbAlpha; 

} RGBQUAD;

typedef struct tagIMAGEDATA
{	
	BYTE blue;	
	BYTE green;
	BYTE red;
	
	//BYTE alpha;									

} IMAGEDATA;


#include <string>
#include <vector>

using namespace std;	 


enum struct imageType { BINARY = 1, GRAY = 2, RGB = 3};

class Bitmap{

public:
	Bitmap(const string& fileName, const imageType& iType);
	Bitmap(const vector<int>& image,const int width,const int height, const int channels);

	Bitmap() = delete;
	Bitmap(const Bitmap&) = delete;
	Bitmap& operator=(const Bitmap&) = delete;

	~Bitmap();

	bool getBmStates() const;

	void showBmFileHeader() const;
	void showBmInfoHeader() const;

	//void setBmFileHeader(BITMAPFILEHEADER* fileHeader);
	//void setBmInfoHeader(BITMAPINFOHEADER* infoHeader);

	void write(const string& filename) const;

	void getVector( vector<int>& image, int& width, int& height, int& channels) const;

	bool transform(const imageType type);//transform palette or bmBGR_ to gray or binary .

private:
	imageType imageType_ = imageType::RGB;

	BITMAPFILEHEADER *bmFileHeader_ = nullptr;
	BITMAPINFOHEADER *bmInfoHeader_ = nullptr;
	RGBQUAD *bmPalette_ = nullptr;
	BYTE *bmIndex_ = nullptr;
	IMAGEDATA *bmBGR_ = nullptr;

};


#endif