#ifndef VECTOR3_H
#define VECTOR3_H

class Vector3 {

	private:
		float x;
		float y;
		float z;

	public:
		//constructors
		Vector3();
		Vector3(float, float, float);

		//get functions
		float getX() { return x; };
		float getY() { return y; };
		float getZ() { return z; };
		float getLength();

		//set functions
		void setX(float x_) { x = x_; };
		void setY(float y_) { y = y_; };
		void setZ(float z_) { z = z_; };

		//operators
		friend Vector3 operator +(const Vector3 &v1, const Vector3 &v2);
		friend Vector3 operator -(const Vector3 &v1, const Vector3 &v2);
		friend Vector3 operator *(const float &s, const Vector3 &v);
		friend Vector3 operator *(const Vector3 &v, const float &s);
		friend Vector3 operator /(const Vector3 &v, const float &s);

		//misc
		void print();

};

#endif
