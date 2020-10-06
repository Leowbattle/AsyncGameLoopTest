#ifndef VEC2_HPP
#define VEC2_HPP

struct vec2 {
	float x;
	float y;

	vec2 operator+(vec2 v);
	vec2 operator-(vec2 v);

	vec2& operator+=(vec2 v);
	vec2& operator-=(vec2 v);

	vec2 operator*(float f);
	vec2 operator/(float f);

	float length();

	vec2 normalised();
};

#endif