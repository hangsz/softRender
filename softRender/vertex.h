#ifndef VERTEX_H	
#define VERTEX_H

#include "point.h"

struct Texture{
	
	Texture() = default;
    Texture(float uu, float vv){ u = uu; v = vv; }
	Texture(const Texture& texture){ u = texture.u; v = texture.v; }
	~Texture() = default;
	Texture& operator=(const Texture& texture){
		if (this != &texture){ u = texture.u; v = texture.v;}
		return *this;
	}
	void lerp(const Texture& t1, const Texture& t2, const Texture& t3,float beta,float gamma ){
		u = (1.0f - beta - gamma)*t1.u + beta*(t2.u - t1.u) + gamma*(t3.u - t1.u);
		v = (1.0f - beta - gamma)*t1.v + beta*(t2.v - t1.v) + gamma*(t3.v - t1.v);
	}

	float u, v; //texture coordinates.
};

struct Color{
	Color() = default;
	Color(float RR, float GG, float BB){ R = RR; G = GG; B = BB;}
	Color(const Color& color) { R = color.R; G = color.G; B = color.B; }
	~Color() = default;
	Color& operator=(const Color& color){
		if (this != &color) { R = color.R; G = color.G; B = color.B;}
		return *this;
	}
	void lerp(const Color& c1, const Color& c2,const Color& c3, float beta,float gamma){
		R = (1.0f - beta - gamma)*c1.R + beta*(c2.R - c1.R) + gamma*(c3.R - c1.R);
		G = (1.0f - beta - gamma)*c1.G + beta*(c2.G - c1.G) + gamma*(c3.G - c1.G);
		B = (1.0f - beta - gamma)*c1.B + beta*(c2.B - c1.B) + gamma*(c3.B - c1.B);
	}

	float   R = 0, G = 0, B = 0;

};


class Vertex{
public:

	Vertex() :point_(nullptr), texture_(nullptr), color_(nullptr){}
	Vertex(const Vertex& vertex);
	Vertex& operator=(const Vertex& vertex);
	~Vertex();
	
	const Point* point() const { return point_; }
	const Texture* texture() const { return texture_; }
	const Color* color() const{ return color_; }
	Point* point()  { return point_; }
	Texture* texture()  { return texture_; }
	Color* color() { return color_; }

	void point(const Point& point) {
		if (point_) delete point_;
		point_ = new Point(point);
	}
	void texture(const Texture& texture){
		if (texture_)  delete texture_;
		texture_ = new Texture(texture);
	}
	void color(const Color& color){
		if (color_) delete color_;
		color_ = new Color(color);
	}

	void lerp(const Vertex& v1, const Vertex&v2, const Vertex&v3, float beta, float gamma);


private:
	Point *point_;
	Texture *texture_;
	Color *color_;
};

#endif