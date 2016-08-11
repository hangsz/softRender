
#include "vertex.h"

Vertex::Vertex(const Vertex& vertex){
	if (vertex.point())
		point_ = new Point(*vertex.point());
	if (vertex.texture())
		texture_ = new Texture(*vertex.texture());
	if (vertex.color())
		color_ = new Color(*vertex.color());
}

Vertex::~Vertex(){
	if( point_) delete point_; 
	if( texture_) delete texture_; 
	if( color_)  delete color_;
}

Vertex& Vertex::operator=(const Vertex& vertex){
	if (this != &vertex)  {
		if (vertex.point())	  {
			if (point_) *point_ = *(vertex.point());
			else  point_ = new Point(*vertex.point());
		}
		if (vertex.texture()) {
			if (texture_) *texture_ = *(vertex.texture());
			else texture_ = new Texture(*vertex.texture());
		}
		if (vertex.color())	 {
			if (color_) *color_ = *(vertex.color());
			else color_ = new Color(*vertex.color());
		}
	}
	return *this;

}

void Vertex::lerp(const Vertex& v1, const Vertex&v2, const Vertex&v3 ,float beta,float gamma){

	if (!point_) point_ = new Point;
	point_->lerp(*v1.point(), *v2.point(),*v3.point(),beta,gamma);

	if (!texture_) texture_ = new Texture;
	texture_->lerp(*v1.texture(), *v2.texture(), *v3.texture(),beta,gamma);

	if (!color_) color_ = new Color;
	color_->lerp(*v1.color(), *v2.color(), *v3.color(),beta,gamma);
}


