
#include "vector.h"
#include "point.h"
#include "transform.h"
#include "vertex.h"

#include "render.h"

#include "bitmap.h"

#include <cstdlib>

#define NV 8


int main(){
	Point p[NV] = { { -1.0f, -1.0f, 1.0f, 1.0f }, { 1.0f, -1.0f, 1.0f, 1.0f },
					{ 1.0f, 1.0f, 1.0f, 1.0f }, { -1.0f, 1.0f, 1.0f, 1.0f },
					{ -1.0f, -1.0f, -1.0f, 1.0f }, { 1.0f, -1.0f, -1.0f, 1.0f },
					{ 1.0f, 1.0f, -1.0f, 1.0f }, { -1.0f, 1.0f, -1.0f, 1.0f } };

	Texture t[NV] = { { 0.0, 0.0 }, { 1.0f, 0.0 }, { 1.0f, 1.0f }, { 0.0, 1.0f },
					{ 0.0, 0.0 }, { 1.0f, 0.0 }, { 1.0f, 1.0f }, { 0.0, 1.0f } };

	Color c[NV] = { { 1.0f, 0.2f, 0.2f }, { 0.2f, 1.0f, 0.2f },
					{ 0.2f, 0.2f, 1.0f }, { 1.0f, 0.2f, 1.0f },
					{ 1.0f, 1.0f, 0.2f }, { 0.2f, 1.0f, 1.0f },
					{ 1.0f, 0.3f, 0.3f }, { 0.2f, 1.0f, 0.3f } };

	
	Render render(640,640);

	render.vertexBuffer(NV, p, t, c);	
	

	//render.backColor(Color(0.1f, 0.1f, 0.1f));

	
	render.textureBuffer();
	
	render.vertexShader();

	render.fragmentShader();

	vector<int> image;
	int width, height,channels = 3;

	render.image(image, height, width);

	Bitmap bmp(image, width, height, 1);


	string workDir = "c:\\users\\hang\\desktop";

	bmp.write(workDir + "\\Render\\1.bmp");

}



	