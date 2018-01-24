#ifndef _RENDERER_H
#define _RENDERER_H

#ifndef _WINDOWS_H
#include <Windows.h>
#endif
#ifndef _VECTOR_H
#include <vector>
#endif

#ifndef _VECTOR2D_H
#include "Vector2D.h"
#endif
#ifndef _MATRIX2D_H
#include "Matrix2D.h"
#endif
#ifndef _RECT2D_H
#include "Rect2D.h"
#endif

#ifndef _COLOR_H
#include "color.h"
#endif

//Forward Declaration
//class Image;

class Renderer
{
public:
	Renderer();
	~Renderer();

	//Methods

	void setInterpolationMode(D2D1_BITMAP_INTERPOLATION_MODE i) { m_InterpolationMode = i; }

	void SetColor(const Color& c);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 0.5f);
	void DrawLine(const Vector2D& v1, const Vector2D& v2, float lineWidth = 0.5f);

	void DrawRect(double left, double top, double width, double height, float lineWidth = 0.5f);
	void DrawRect(const Vector2D& lefttop, const Vector2D& rightbottom, float lineWidth = 0.5f);
	void DrawRect(const Rect2D& rect, float lineWidth = 0.5f);

	void DrawCircle(double xcenter, double ycenter, double radius, float linewidth = 0.5f);
	void DrawCircle(const Vector2D& center, double radius, float linewidth = 0.5f);
	
	void DrawPolygon(const std::vector<Vector2D>& vecPoints, bool close = true, float linewidth = 0.5f);
	void DrawPolygon(Vector2D* points, int size, bool close, float linewidth = 0.5f);

	void FillRect(double left, double top, double width, double height);
	void FillRect(const Vector2D& lefttop, const Vector2D& rightbottom);
	void FillRect(const Rect2D& rect);

	void FillCircle(double xcenter, double ycenter, double radius);
	void FillCircle(const Vector2D& center, double radius);

	void FillPolygon(const std::vector<Vector2D>& vecPoints, bool close = true);
	void FillPolygon(Vector2D* points, int size, bool close);

private:
	//Members
	D2D1_BITMAP_INTERPOLATION_MODE m_InterpolationMode;
};

#endif // _RENDERER_H