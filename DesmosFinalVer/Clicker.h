#pragma once
#include "pch.h"

class Clicker
{
private:
	Rectangle* collider = new Rectangle;
public:
	Clicker(const float x, const float y, const float width, const float height) { *collider = { x, y, width, height }; }
	Clicker(const Rectangle& r) { *collider = r; }
	Clicker() { *collider = { 0.f,0.f,0.f,0.f }; }

	~Clicker()
	{
		if (!collider) return;

		delete this->collider;
		this->collider = nullptr;
	}

	const Rectangle GetCollider() const { return *this->collider; }
	void UpdatePostition(float x, float y);
};

