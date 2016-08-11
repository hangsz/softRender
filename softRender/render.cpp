#include "render.h"

#include <iostream>

Render::Render(int height, int width) :height_(height), width_(width){
	frameBuffer_ = new int*[height]{nullptr};
	for (int y = 0; y < height; ++y)
		frameBuffer_[y] = new int[width];

	zBuffer_ = new float*[height]{nullptr};
	for (int y = 0; y < height; ++y)
		zBuffer_[y] = new float[width]{1.0f};

};

void Render::backColor(const Color& color){
	std::cout << "backColor" << std::endl;
	int R, G, B;
	R = static_cast<int>(color.R*255.0f);
	G = static_cast<int>(color.G*255.0f);
	B = static_cast<int>(color.B*255.0f);

	for (int y = 0; y< height_; ++y)
	for (int x = 0; x < width_; ++x)
		frameBuffer_[y][x] = 255;
		//frameBuffer_[y][x] = (B << 24) | (G << 16) | (R << 8 );
}

void Render::vertexBuffer(int nv, const Point* point, const Texture* texture, const Color* color){
	std::cout << "vertexBuffer" << std::endl;
	if (vertexBuffer_)  delete[] vertexBuffer_;
	nv_ = nv;
	vertexBuffer_ = new Vertex[nv_];

	for (int i = 0; i < nv_; ++i)  {
		vertexBuffer_[i].point(point[i]);
		vertexBuffer_[i].texture(texture[i]);
		vertexBuffer_[i].color(color[i]);
	}
}

void Render::textureBuffer(){
	std::cout << "textureBuffer" << std::endl;
	if (textureBuffer_)  return;
	
	textureBuffer_ = new int*[height_];
	for (int y = 0; y < height_; ++y)
		textureBuffer_[y] = new int[width_];


	/*int inc = 32;
	int x, y;
	for (int m = 0; m < height_ / inc; ++m)
	for (int n = 0; n < width_ / inc; ++n){
		for (int j = 0; j < inc; ++j){
			y = m*inc + j;
			for (int i = 0; i < inc; ++i){
				x = n*inc + i;
				if ((m + n) & 1) textureBuffer_[y][x] = 200;
				else  textureBuffer_[y][x] = 0;
			}
		}
	}*/

	int color = 0;
	for (int y = 0; y < height_; ++y) {
		color = (color + 1) % 255;
		for (int x = 0; x < width_; ++x)
			textureBuffer_[y][x] = color;
	}
}

//from model coordinates to window coordinates
void Render::vertexShader(){
	std::cout << "vertexShader" << std::endl;
	
	Transform CT,world, view, perspective, screen;
	
	CT.identity();	
	
	world.world();	

	Point eye(0.0, 0.0, 3.0, 1.0f);
	Point at(0.0, 0.0, 0.0, 1.0f);
	Vector up(0.0, 1.0f, 0.0);
	view.view(eye, at, up);

	perspective.perspective(90.0f, 1.0f, -1.0f, -500.0f);

	screen.screen(320, 320);

	CT = perspective*view*world;

	Point p(1, 1, 1, 1);
	CT*p;
	//for (int i = 0; i < nv_; ++i)  
	//	CT * ( *vertexBuffer_[i].point() );

	primitive();
}


void Render::fragmentShader(){
	std::cout << "fragmentShader" << std::endl;
	
	int mode = 1; 
	// 0 color;1 texture

	Vertex *vXY = nullptr;
	for (int y = 0; y < height_; ++y)
	for (int x = 0; x < width_; ++x){
		if (np_>0){
			for (int k = 0; k < np_; ++k){
				std::cout << "k" << k << std::endl;
				vXY = inPrimitive( primitive_[k], y, x);
				colorXY(y, x, vXY, mode);  //mode 0:color
				delete vXY;
				vXY = nullptr;
			}
		}
		else{
			colorXY(y, x, vXY, mode);
		}
	}
}

void Render::primitive(){

	if (primitive_)  return;

	np_ = 12;
	
	primitive_ = new Primitive[np_];

	//look at -z;
	primitive_[0] = Primitive(0, 1, 2);
	primitive_[1] = Primitive(2, 3, 0);
	primitive_[2] = Primitive(4, 5, 6);
	primitive_[3] = Primitive(6, 7, 4);

	//look at -x
	primitive_[4] = Primitive(1, 5, 6);
	primitive_[5] = Primitive(6, 2, 1);
	primitive_[6] = Primitive(0, 4, 7);
	primitive_[7] = Primitive(7, 3, 0);

	//look at -y
	primitive_[8] = Primitive(3, 2, 6);
	primitive_[9] = Primitive(6, 7, 3);
	primitive_[10] = Primitive(0, 1, 5);
	primitive_[11] = Primitive(5, 4, 0);
}

bool Render::inCVV(const Point& point) const{
	int test = 0;	  
	if (point.w() < -point.z())  test |= 1;
	if (point.w() <  point.z())  test |= 2;
	if (point.w() < -point.x())  test |= 4;
	if (point.w() <  point.x())  test |= 8;
	if (point.w() < -point.y())  test |= 16;
	if (point.w() <  point.y())  test |= 32;
	
	if (0 == test) return true;
	else return false;
}

Vertex* Render::inPrimitive(const Primitive& primitive, int y, int x) const{
    
	float dx, dy, dx1, dy1, dx2, dy2;

	Vertex *v0 = vertexBuffer_ + primitive[0];
	Vertex *v1 = vertexBuffer_ + primitive[1];
	Vertex *v2 = vertexBuffer_ + primitive[2];

	dx = x - v0->point()->x() / v0->point()->w();
	dy = y - v0->point()->y() / v0->point()->w();

	dx1 = v1->point()->x() / v1->point()->w() - v0->point()->x() / v0->point()->w();
	dy1 = v1->point()->y() / v1->point()->w() - v0->point()->y() / v0->point()->w();

	dx2 = v2->point()->x() / v2->point()->w() - v0->point()->x() / v0->point()->w();
	dy2 = v2->point()->y() / v2->point()->w() - v0->point()->y() / v0->point()->w();


	//v  = beta*v1 + gamma*v2;
	float beta =  (dx*dy2 - dx2*dy) / (dx1*dy2 - dx2*dy1);
	float gamma = (dx*dy1 - dx1*dy) / (dx2*dy1 - dx1*dy2);

	std::cout << "beta: " << beta << "  gamma:  " << gamma << std::endl;
	if (beta<=0.0 || beta >=1.0f || gamma<=0.0 || gamma>=1.0f || (beta+gamma)>=1.0f)  return nullptr;

	float h0, h1, h2,d;
	float beta_w,gamma_w;
	
	h0 = v0->point()->w();
	h1 = v1->point()->w();
	h2 = v2->point()->w();
	
	d = h1*h2 + h2*beta*(h0-h1) + h1*gamma*(h0-h2);

	beta_w = h0*h2*beta/d;
	gamma_w = h0*h1*gamma/d;
	
	Vertex *v = new Vertex;
	v->lerp(*v0, *v1, *v2, beta_w, gamma_w);
	return v;
}

void Render::colorXY(int y, int x, const Vertex* vXY, int mode = 0){

	if (!vXY) {
		
		frameBuffer_[y][x] = textureBuffer_[y][x];
		return;
	}
	std::cout << "colorXY" << std::endl;


	if ( 0 == mode ){
		if (vXY->point()->z() / vXY->point()->w() < zBuffer_[y][x])
			frameBuffer_[y][x] = static_cast<int>((vXY->color()->B * 255.0f)) << 25 |
			                     static_cast<int>((vXY->color()->G * 255.0f)) << 16 |
			                     static_cast<int>((vXY->color()->R * 255.0f)) << 8;
	}
	if ( 1 == mode )	{	
		int ty = vXY->texture()->v*height_;
		int tx = vXY->texture()->u*width_;
		frameBuffer_[y][x] = textureBuffer_[ty][tx];
	}
}

void Render::image(std::vector<int>& image,int& height,int& width){
	for (int y = 0; y < height_; ++y)
	for (int x = 0; x < width_; ++x)
		image.push_back(frameBuffer_[y][x]);

	height = height_;
	width = width_;
}