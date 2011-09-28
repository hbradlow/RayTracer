/*
 The Vector3D class: represents a 3 dimensional vector. Only has 3 variables: x, y, and z for displacement.
 */
#ifndef _VECTOR3D_h
#include <math.h>
#include <vector>
#define _VECTOR3D_h
using namespace std;

class Vector3D {
public:
    
    /*Variables:
     I didn't make x, y, and z private, as the programmer should already know the inner workings of a vector,
     and would figure that there would be an x, y, and z variables. Plus, I hate the look of setX(int x) and getX() :D .
     */
    float x, y, z;
    
    /*Constructors:
     Vector3D v: x, y, and z are all set to 0.
     Vector3D v2(1, 2, 3): x, y, and z are set to 1, 2, and 3, respectivly.
     Vector3D v3(Vector3D v4): v3.x, v3.y, and v3.z are all set to v4.x, v4.y, and v4.z, respectivly.
     */
    Vector3D() {
        x = y = z = 0;
    }
    
    Vector3D(float a, float b, float c) {
        x = a;
        y = b;
        z = c;
    }
    
    Vector3D(const Vector3D &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    
    /*Operator overloadings:
     Vector3D operator+(Vector v): x, y, and z are added to v.x, v.y, and v.z, respectivly.
     Vector3D operator-(Vector3D v): x, y, and z are subtracted from v.x, v.y, and v.z, respectivly.
     Vector3D operator*(float w): x, y, and z are all multiplied by w.
     Vector3D operator/(float w): x, y, and z are all divided by w.
     Vector3D operator=(Vector3D v): this vector is set to v.
     */
    Vector3D operator+(Vector3D v);
    Vector3D operator-(Vector3D v);
    Vector3D operator*(float w);
    Vector3D operator/(float w);
    Vector3D operator=(Vector3D v);
    
    /*Other functions:
     void negate(): negate the vector - x, y, and z are set to -x, -y, and -z, respectivly
     float length(): returns the length of the vector.
     void normalize(): normalize the vector, vector / length of vector
     */
    void negate();
    float length();
    void normalize();
    Vector3D cross(Vector3D a);
    float dot(Vector3D a);
};

Vector3D Vector3D::operator+(Vector3D v) {
    return Vector3D((x + v.x), (y + v.y), (z + v.z));
}

Vector3D Vector3D::operator-(Vector3D v) {
    return Vector3D((x - v.x), (y - v.y), (z - v.z));
}

Vector3D Vector3D::operator*(float w) {
    return Vector3D((x * w), (y * w), (z * w));
}

Vector3D Vector3D::operator/(float w) {
    return Vector3D((x / w), (y / w), (z / w));
}

Vector3D Vector3D::operator=(Vector3D v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

void Vector3D::negate() {
    x = -x;
    y = -y;
    z = -z;
}

float Vector3D::length() {
    return sqrt((x * x) + (y * y) + (z * z));
}

void Vector3D::normalize() {
    x /= length();
    y /= length();
    z /= length();
}
Vector3D Vector3D::cross( Vector3D a )
{
    
	float x1, y1, z1;
    
	x1 = ( y * a.z ) - ( z * a.y );
	y1 = ( z * a.x ) - ( x * a.z );
	z1 = ( x * a.y ) - ( y * a.x );
    
    return Vector3D(x1,y1,z1);
}

float Vector3D::dot( Vector3D a )
{
    
	return ( x * a.x ) + ( y * a.y ) + ( z * a.z );
}

struct Ray{
    Vector3D *direction;
    Vector3D *location;
};
#endif