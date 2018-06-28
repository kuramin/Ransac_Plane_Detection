#pragma once
#include <math.h>

class my_Color
{
public:
	short R, G, B;
	int weight;

public:
	inline my_Color() {}

	inline my_Color(short R, short G, short B)
		: R(R), G(G), B(B), weight(pow(R-G, 2) + pow(G-B, 2) + pow(B-R, 2)) {}
};