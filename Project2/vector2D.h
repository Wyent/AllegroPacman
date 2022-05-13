#include <tuple>

class Vector2D {
private:
	std::tuple <double, double> myTupe;
	double angle;
	Vector2D();
	Vector2D(double, double);
	Vector2D(const Vector2D&);
	Vector2D& operator = (const Vector2D&);
	Vector2D& operator += (const Vector2D&);

}