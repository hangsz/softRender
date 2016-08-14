#include "Bitmap.h"

//#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

Bitmap::Bitmap(const string& fileName,const imageType& iType):imageType_(iType) {
	ifstream ifs(fileName, ios::binary);

	if (!ifs) {
		cout << "file open error!" << endl;
		exit(-1);
	}
	//-------------------------------------
	//read file header. 
	if (!bmFileHeader_) bmFileHeader_ = new BITMAPFILEHEADER;

	//read bm type first.
	ifs.read((char*)(&(bmFileHeader_->bfType)), sizeof(bmFileHeader_->bfType));

	if (0x4d42 != bmFileHeader_->bfType){
		cout << "The file is not a bmp !" << endl;
		exit(-1);
	}

	//read other parts of bmp file header.  
	ifs.read((char*)(&(bmFileHeader_->bfSize)), sizeof(bmFileHeader_->bfSize));
	ifs.read((char*)(&(bmFileHeader_->bfReserved1)), sizeof(bmFileHeader_->bfReserved1));
	ifs.read((char*)(&(bmFileHeader_->bfReserved2)), sizeof(bmFileHeader_->bfReserved2));
	ifs.read((char*)(&(bmFileHeader_->bfOffBits)), sizeof(bmFileHeader_->bfOffBits));

	//showBmFileHeader();
	//---------------------------------

	//read bmp info header
	if (!bmInfoHeader_) bmInfoHeader_ = new BITMAPINFOHEADER;	
	

	ifs.read((char *)(bmInfoHeader_), 40);	
	
	//showBmInfoHeader();

	if (8 != bmInfoHeader_->biBitCount && 24 != bmInfoHeader_->biBitCount){
		cout << "Can't deal " << bmInfoHeader_->biBitCount << "-bit Bitmap";
		exit(-1);
	}

	//-----------------------------------------------
	//read palette   and index

	const unsigned int height = bmInfoHeader_->biHeight;
	const unsigned int width = bmInfoHeader_->biWidth;
	const unsigned int wxh = width*height;
	const unsigned int lineBytes = ((width * bmInfoHeader_->biBitCount + 31) >> 5) << 2;

	const unsigned int pixelBytes = bmInfoHeader_->biBitCount / 8;
	const unsigned int interval = lineBytes - width*pixelBytes;


	bmInfoHeader_->biSizeImage = lineBytes* bmInfoHeader_->biHeight;

	if (8 == bmInfoHeader_->biBitCount){

		// read palette
		int colorNum = bmInfoHeader_->biClrUsed;
		if (0 == colorNum)
			colorNum = bmInfoHeader_->biClrUsed = 256;

		if (!bmPalette_) bmPalette_ = new RGBQUAD[colorNum];

		ifs.read((char*)bmPalette_, 4 * colorNum);

		// read index
		if (!bmIndex_) bmIndex_ = new BYTE[wxh];
		for (unsigned int j = 0; j < height; j++){
			ifs.read((char *)(&(bmIndex_[j*height])), width);
			ifs.seekg(interval, ios::cur);
		}

		//judge bitmap type
		if (imageType::RGB == imageType_)  {
			unsigned int i;
			for (i = 0; i < wxh; i++)
			if ((bmPalette_[i].rgbBlue == bmPalette_[i].rgbGreen) && (bmPalette_[i].rgbBlue == bmPalette_[i].rgbRed)) continue;
			else break;

			if (wxh == i) imageType_ = imageType::GRAY;
		}

			// store rgb color
		if (!bmBGR_) bmBGR_ = new IMAGEDATA[wxh];

		for (int i = 0; i != wxh; i++){
			bmBGR_[i].blue  = bmPalette_[ static_cast<unsigned int>(bmIndex_[i])].rgbBlue;
			bmBGR_[i].green = bmPalette_[ static_cast<unsigned int>(bmIndex_[i])].rgbGreen;
			bmBGR_[i].red   = bmPalette_[ static_cast<unsigned int>(bmIndex_[i])].rgbRed;
			//bmBGR_[i].alpha = bmPalette_[(unsigned int)bmIndex_[i]].rgbAlpha;
		}

	}
	//read rgb directly
	if (24 == bmInfoHeader_->biBitCount) {

		if (!bmBGR_) bmBGR_ = new IMAGEDATA[wxh];

		for (unsigned int j = 0; j < height; j++)	  {
			ifs.read((char *)(&(bmBGR_[j*width])), width*pixelBytes);
			ifs.seekg(interval, ios::cur);
		}

		//judge bitmap type
		if (imageType::RGB == imageType_)  {
			unsigned int i;
			for (i = 0; i < wxh; i++)
			if ((bmBGR_[i].blue == bmBGR_[i].green) && (bmBGR_[i].blue == bmBGR_[i].red)) continue;
			else break;
			if (wxh == i) imageType_ = imageType::GRAY;
		}

	}
	ifs.close();

}

// only can construct 24-bit bitmap
Bitmap::Bitmap(const vector<int>& image, const int width, const int height,const int channels){
	//cout << "Bitmap constructor: vector" << endl;
	
	if (image.size() != width*height*channels) {
		cout << "Image size error." << endl;
		exit(-1);
	}

	if ((1 > width) || (1 > height) || ((1 != channels) && (3 != channels))) {
		cout << "Parameters region error." << endl;
		exit(-1);
	}

	const int wxh = width*height;
	const int lineBytes = ((width * 24 + 31) >> 5) << 2;
	const int pixelBytes = 3;
	const int interval = lineBytes - width*pixelBytes;

	//construct file header.
	bmFileHeader_ = new BITMAPFILEHEADER;
	bmFileHeader_->bfType = 0x4d42;
	bmFileHeader_->bfSize = 54 + lineBytes*height;
	bmFileHeader_->bfReserved1 = 0;
	bmFileHeader_->bfReserved2 = 0;
	bmFileHeader_->bfOffBits = 54;
	
	//construct info header.
	bmInfoHeader_ = new BITMAPINFOHEADER;
	bmInfoHeader_->biSize = 40;
	bmInfoHeader_->biWidth = width;
	bmInfoHeader_->biHeight = height;
	bmInfoHeader_->biPlanes = 1;
	bmInfoHeader_->biBitCount = 24;
	bmInfoHeader_->biCompression = 0;
	bmInfoHeader_->biSizeImage = lineBytes*height;
	bmInfoHeader_->biXPelsPerMeter = 3780;
	bmInfoHeader_->biYPelsPerMeter = 3780;
	bmInfoHeader_->biClrUsed = 0;
	bmInfoHeader_->biClrImportant = 0;

	//construct IMAGEDATA

	//judge bitmap type
	if (1 == channels)	   imageType_ = imageType::GRAY;
	if (3 == channels)	{

		int i;
		for (i=0; i <wxh; i++)
		if ( (image[3 * i] == image[3 * i + 1]) && (image[3 * i] == image[3 * i + 2]))
			continue;
		else 
			break;
	
		if ( wxh == i) imageType_ = imageType::GRAY;
		else imageType_ = imageType::RGB;
	}

	

	bmBGR_ = new IMAGEDATA[wxh];

	if ( 1 == channels ) {
		for (int i = 0; i < wxh; i++){
			bmBGR_[i].blue =
			bmBGR_[i].green =
			bmBGR_[i].red = (unsigned char)image[i];
		}
	}
	if (3 == channels)	 {
		for (int i = 0; i < wxh; i++){
			bmBGR_[i].blue = (unsigned char)image[i*channels];
			bmBGR_[i].green = (unsigned char)image[i*channels + 1];
			bmBGR_[i].red = (unsigned char)image[i*channels + 2];
		}
	}

	
}


Bitmap::~Bitmap(){
	 delete bmFileHeader_;
	 delete bmInfoHeader_;
	 delete[] bmPalette_;
	 delete[] bmIndex_;
	 delete[] bmBGR_;
}


void Bitmap::showBmFileHeader() const{
	if (!bmFileHeader_) {
		cout << "No file header." << endl;
		return;
	}
	cout << "  Bitmap File Header   " << endl;
	cout << "bfType: " << bmFileHeader_->bfType << endl;
	cout << "bfSize: " << bmFileHeader_->bfSize << endl;
	cout << "bfReversed1: " << bmFileHeader_->bfReserved1 << endl;
	cout << "bfReversed2: " << bmFileHeader_->bfReserved2 << endl;
	cout << "bfOffBits: " << bmFileHeader_->bfOffBits << endl << endl;
}

void Bitmap::showBmInfoHeader() const{

	if (!bmInfoHeader_) {
		cout << "No info header." << endl;
		return;
	}
	cout << "   Bitmap Information Header   " << endl;
	cout << "biSize: " << bmInfoHeader_->biSize << endl;
	cout << "biWidth: " << bmInfoHeader_->biWidth << endl;
	cout << "biHeight: " << bmInfoHeader_->biHeight << endl;
	cout << "biPlanes: " << bmInfoHeader_->biPlanes << endl;
	cout << "biBitCount: " << bmInfoHeader_->biBitCount << endl;
	cout << "biCompression: " << bmInfoHeader_->biCompression << endl;
	cout << "biSizeImage: " << bmInfoHeader_->biSizeImage << endl;
	cout << "biXPelsPerMeter:" << bmInfoHeader_->biXPelsPerMeter << endl;
	cout << "biYPelsPerMeter:" << bmInfoHeader_->biYPelsPerMeter << endl;
	cout << "biClrUsed:" << bmInfoHeader_->biClrUsed << endl;
	cout << "biClrImportant:" << bmInfoHeader_->biClrImportant << endl;
}


//output bitmap 
void Bitmap::write(const string& fileName) const{
	// transform color first
	
	ofstream ofs(fileName, ios::binary);
	if (!ofs) {
		cout << "file open error!" << endl;
		return;
	}

	//-------------------------------------
	//write file header. 
	ofs.write((char*)(&(bmFileHeader_->bfType)), sizeof(bmFileHeader_->bfType));
	ofs.write((char*)(&(bmFileHeader_->bfSize)), sizeof(bmFileHeader_->bfSize));
	ofs.write((char*)(&(bmFileHeader_->bfReserved1)), sizeof(bmFileHeader_->bfReserved1));
	ofs.write((char*)(&(bmFileHeader_->bfReserved2)), sizeof(bmFileHeader_->bfReserved2));
	ofs.write((char*)(&(bmFileHeader_->bfOffBits)), sizeof(bmFileHeader_->bfOffBits));
	//---------------------------------
	//write bmp info header

	ofs.write((char *)(bmInfoHeader_), 40);
	//-----------------------------------------------
	// write palette and index	

	const unsigned int height = bmInfoHeader_->biHeight;
	const unsigned int width = bmInfoHeader_->biWidth;
	const unsigned int wxh = width*height;
	const unsigned int lineBytes = ((width * bmInfoHeader_->biBitCount + 31) >> 5) << 2;

	const unsigned int pixelBytes = bmInfoHeader_->biBitCount / 8;
	const unsigned int interval = lineBytes - width*pixelBytes;

	char *zero = new char[interval];
	for (unsigned int i = 0; i < interval; i++)
		zero[i] = 0;

	if (8 == bmInfoHeader_->biBitCount){
		// write palette
		ofs.write((char*)bmPalette_, 4 * bmInfoHeader_->biClrUsed);

		// write index
		for (unsigned int j = 0; j < height; j++) {
			ofs.write((char *)(&(bmIndex_[j*width])), width);
			ofs.write(zero, interval);
			//ofs.seekp(interval, ios::cur);
		}

	}
	//write real color bitmap
	if (24 == bmInfoHeader_->biBitCount) {

		for (unsigned int j = 0; j < height; j++) {
			ofs.write((char *)(&(bmBGR_[j*width])), width*pixelBytes);
			ofs.write(zero, interval);
		}
	}
	ofs.close();

}


//transform RGB to GRAY or BINARY
bool Bitmap::transform(const imageType type){

	if (imageType_ == type)  return true;
	if (imageType::GRAY == imageType_ && imageType::RGB == type)  {
		cout << "can't transform from GRAY to RGB." << endl;
		return false;
	}
	if (imageType::BINARY == imageType_ && (imageType::GRAY == type || imageType::RGB == type))  {
		cout << "can't transform from BINARY to GRAY or RGB." << endl;
		return false;
	}

	imageType_ = type;

	if (8 == bmInfoHeader_->biBitCount){
		int gray;
		int colorNum = bmInfoHeader_->biClrUsed;
		for (int i = 0; i < colorNum; i++) {
			gray = (bmPalette_[i].rgbRed * 30 + bmPalette_[i].rgbGreen * 59 + bmPalette_[i].rgbBlue * 11) / 100;

			if (imageType::BINARY == type)
			if (gray < 128)	 gray = 0;
			else gray = 255;

			bmPalette_[i].rgbRed   =
			bmPalette_[i].rgbGreen =
			bmPalette_[i].rgbBlue  = gray;
		}
	}

	if (24 == bmInfoHeader_->biBitCount) {
		int gray;
		const unsigned int wxh = bmInfoHeader_->biWidth*bmInfoHeader_->biHeight;
		for (unsigned int i = 0; i < wxh; i++) {
			gray = (bmBGR_[i].red * 30 + bmBGR_[i].green * 59 + bmBGR_[i].blue * 11) / 100;

			if (imageType::BINARY == type)
			if (gray < 128)	 gray = 0;
			else gray = 255;

			bmBGR_[i].red   =
			bmBGR_[i].green =
			bmBGR_[i].blue  = gray;
		}
	}

	return true;
}

// bitmap to vector
void Bitmap::getVector(vector<int>& image, int& width, int& height, int& channels) const{

	width = bmInfoHeader_->biWidth;
	height = bmInfoHeader_->biHeight;

	if (imageType::BINARY == imageType_ || imageType::GRAY == imageType_) channels = 1;
	else channels = 3;

	const int wxh = width*height;

	image.clear();
	for (int i = 0; i < wxh; i++){
		image.push_back((int)bmBGR_[i].blue);
		if (3 == channels){
			image.push_back((int)bmBGR_[i].green);
			image.push_back((int)bmBGR_[i].red);
		}
	}
}







