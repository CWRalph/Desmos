#include "pch.h"
#include "Clicker.h"

void Clicker::UpdatePostition(float x, float y)
{
	(*this->collider).x += x;
	(*this->collider).y += y;
}

