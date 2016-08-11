#ifndef RENDER_H
#define RENDER_H

#include "vertex.h"
#include "transform.h"

#include <vector>

#define eps 1.0E-2

class Primitive{
public:
	Primitive() = default;
	explicit Primitive(int p1, int p2, int p3){
		index[0] = p1; index[1] = p2; index[2] = p3;
	}
	~Primitive() = default;
	const int operator[](int at) const{  return index[at];}

	Primitive& operator=(const Primitive& primitive){
		if (this != &primitive){
			index[0] = primitive[0];
			index[1] = primitive[1];
			index[2] = primitive[2];
		}
		return *this;
	}
private:
	int index[3];
};



class Render{

public:
	Render(int height, int width);

	void backColor(const Color& color);

	void vertexBuffer(int nv, const Point* point,const Texture* texture,const Color* color);

	void textureBuffer();

	//from model coordinates to window coordinates
    void vertexShader();
	
	//generate pixel
	void fragmentShader();

	void image(std::vector<int>& image,int& height,int& width);

private:
	void primitive();
	bool inCVV(const Point& point) const;
	Vertex* inPrimitive(const Primitive& primitive,int x, int y) const;
	void colorXY(int x,int y, const Vertex* vXY,int mode);



	int height_, width_;//window size,image size

	int nv_ = 0;
	Vertex *vertexBuffer_ = nullptr;
	int **textureBuffer_ = nullptr;
	
	int np_ = 0;
	Primitive *primitive_ = nullptr;

	int **frameBuffer_= nullptr; 
	float **zBuffer_ = nullptr;

	
};

#endif