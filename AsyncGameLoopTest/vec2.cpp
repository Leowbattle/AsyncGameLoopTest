#include "vec2.hpp"

#include <math.h>

vec2 vec2::operator+(vec2 v) {
	return { .x = x + v.x, .y = y + v.y };
}

vec2 vec2::operator-(vec2 v) {
	return { .x = x - v.x, .y = y - v.y };
}

vec2& vec2::operator+=(vec2 v) {
	return *this = *this + v;
}

vec2& vec2::operator-=(vec2 v) {
	return *this = *this - v;
}

vec2 vec2::operator*(float f) {
	return { .x = x * f, .y = y * f };
}

vec2 vec2::operator/(float f) {
	return { .x = x / f, .y = y / f };
}

float vec2::length() {
	return hypotf(x, y);
}

vec2 vec2::normalised() {
	return *this / length();
}