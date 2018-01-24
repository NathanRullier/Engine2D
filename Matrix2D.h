#ifndef _MATRIX2D_H
#define _MATRIX2D_H

#ifndef _VECTOR2D_H
	#include "Vector2D.h"
#endif
#ifndef _FSTREAM_H
//	#include "fstream.h"
#endif
#ifndef _2DUTILL_H
	#include "d2dutill.h"
#endif

#include <iostream>
using namespace std;



//-----------------------------------------------------------------
// Matrix2D Struct
//-----------------------------------------------------------------
struct Matrix2D
{
	// -------------------------
	// Friend declarations 
	// -------------------------
	friend Matrix2D operator*(const Matrix2D& f1, const Matrix2D& f2); 				// Multiply or transform matrix f1 by matrix f2 (operator form)
	friend std::ostream& operator<<(std::ostream& os, const Matrix2D& right);		// Print to a stream.
	friend std::wostream& operator<<(std::wostream& wos, const Matrix2D& right);	// Print to a wstream.

	// -------------------------
	// Constructors 
	// -------------------------
	//! Default constructor results in a unity matrix
	Matrix2D(Vector2D dirX = Vector2D(1, 0), Vector2D dirY = Vector2D(0, 1), Vector2D orig = Vector2D(0, 0));
	//! Constructor, using doubles, all required
	Matrix2D(double e1X, double e1Y, double e2X, double e2Y, double oX, double oY);
	//! Copy constructor
	Matrix2D(const Matrix2D& orig);
	//! Construct from Direct2D, not for students
	Matrix2D(const D2D1::Matrix3x2F& mat);

	// -------------------------
	// Operators 
	// -------------------------
	//! Are two matrices exactly equal?				p == q
	bool operator==(const Matrix2D& other) const;
	//! Are two matrices exactly unequal?			p != q
	bool operator!=(const Matrix2D& other) const;

	// -------------------------
	// General Methods 
	// -------------------------
	//! Transform a vector by this matrix, no translation			
	//! Vector2D w = b.Transform(v);
	Vector2D				transformVector(const Vector2D& v) const;
	//! Transform a point by this matrix, including translation			
	//! Vector2D q = f.Transform(p);
	Vector2D				transformPoint(const Vector2D& p) const;
	//! Calculate the determinant
	double					determinant() const;
	//! Calculate the inverse matrix
	Matrix2D				inverse() const;
	//! Are two matrices equal within a threshold?	p.Equals(q)
	bool					equals(const Matrix2D& p) const;
	//! Convert to Direct2D
	const D2D1::Matrix3x2F	toMatrix3x2F() const;

	//! Converts this matrix into a Identity matrix
	void setAsIdentity();
	//! Converts this matrix into a Rotate matrix
	void setAsRotate(double radians);
	//! Converts this matrix into a Translation matrix
	void setAsTranslate(double tx, double ty);
	//! Converts this matrix into a Translation matrix
	void setAsTranslate(Vector2D pt);
	//! Converts this matrix into a Scale matrix
	void setAsScale(double sx, double sy);
	//! Converts this matrix into a Scale matrix
	void setAsScale(double s);

	// -------------------------
	// Static Methods 
	// -------------------------
	//! Instantiate a rotation matrix: 
	//! Matrix r = Matrix::Rotation(M_PI/2);
	static Matrix2D createRotationMatrix(double radians);
	//! Instantiate an identity matrix: 
	//! Matrix2D m = Matrix2D::Identity();
	static Matrix2D createIdentityMatrix();
	//! Instantiate a scale matrix: 
	//! Matrix s = Matrix::Scaling(2);
	static Matrix2D createScalingMatrix(double scale);
	//! Instantiate a scale matrix: 
	//! Matrix s = Matrix::Scaling(2,-3);
	static Matrix2D createScalingMatrix(double scaleX, double scaleY);
	//! Instantiate a scale matrix: 
	//! Matrix s = Matrix::Scaling( Vector2D(2,-3) );
	static Matrix2D createScalingMatrix(Vector2D scaleXY);
	//! Instantiate a translation matrix: 
	//! Matrix t = Matrix::Translation( Point2(2,3) );
	static Matrix2D createTranslationMatrix(Vector2D origin);
	//! Instantiate a translation matrix: 
	//! Matrix t = Matrix::Translation(2,3);
	static Matrix2D createTranslationMatrix(double tx, double ty);


	// -------------------------
	// Data members
	// -------------------------
	Vector2D dirX;	//! The first matrix vector (the "x-axis")
	Vector2D dirY;	//! The second matrix vector (the "y-axis")
	Vector2D orig; 	//! The origin of  the coordinate matrix (the "translation")
};

#endif

