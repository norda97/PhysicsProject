#include "Utils.h"

glm::vec2 toScreenSpace(const glm::vec3 & v, sf::RenderWindow & window, bool viewXY)
{
	float x = v.x / virtSize.x * (float)window.getSize().x * (ONE_VIEW ? 1.f : 0.5f);
	
	if (viewXY)
	{
		float y = (virtSize.y - v.y) / virtSize.y * (float)window.getSize().y;
		return glm::vec2(x, y);
	}
	else
	{
		float z = (virtSize.z - v.z) / virtSize.z * (float)window.getSize().y;
		return glm::vec2(x + (float)window.getSize().x*0.5f, z-(float)window.getSize().y*.5f);
	}
}

float toScreenSpace(float v, sf::RenderWindow & window, unsigned int sideIndex)
{
	glm::vec2 w(window.getSize().x * (ONE_VIEW ? 1.f : 0.5f), window.getSize().y);
	return v / virtSize[sideIndex] * (float)(w[sideIndex == 2 ? 1 : sideIndex]);
}

void rotate(glm::vec3& v, const glm::vec3& angle)
{
	float x = angle.x;
	float y = angle.y;
	float z = angle.z;
	float sinX = (float)sin(x);
	float cosX = (float)cos(x);
	float sinY = (float)sin(y);
	float cosY = (float)cos(y);
	float sinZ = (float)sin(z);
	float cosZ = (float)cos(z);

	glm::mat4 m, mx, my, mz;
	mx[0][0] = 1;		mx[1][0] = 0;		mx[2][0] = 0;		mx[3][0] = 0;
	mx[0][1] = 0;		mx[1][1] = cosX;	mx[2][1] = -sinX;	mx[3][1] = 0;
	mx[0][2] = 0;		mx[1][2] = sinX;	mx[2][2] = cosX;	mx[3][2] = 0;
	mx[0][3] = 0;		mx[1][3] = 0;		mx[2][3] = 0;		mx[3][3] = 1;

	my[0][0] = cosY;	my[1][0] = 0;		my[2][0] = sinY;	my[3][0] = 0;
	my[0][1] = 0;		my[1][1] = 1;		my[2][1] = 0;		my[3][1] = 0;
	my[0][2] = -sinY;	my[1][2] = 0;		my[2][2] = cosY;	my[3][2] = 0;
	my[0][3] = 0;		my[1][3] = 0;		my[2][3] = 0;		my[3][3] = 1;

	mz[0][0] = cosZ;	mz[1][0] = -sinZ;	mz[2][0] = 0;		mz[3][0] = 0;
	mz[0][1] = sinZ;	mz[1][1] = cosZ;	mz[2][1] = 0;		mz[3][1] = 0;
	mz[0][2] = 0;		mz[1][2] = 0;		mz[2][2] = 1;		mz[3][2] = 0;
	mz[0][3] = 0;		mz[1][3] = 0;		mz[2][3] = 0;		mz[3][3] = 1;

	m = mz * my * mx;
	glm::vec4 v2(v.x, v.y, v.z, 0.0f);
	v2 = m * v2;
	v.x = v2.x;
	v.y = v2.y;
	v.z = v2.z;
}

float lerp(float a, float b, float t)
{
	return a*t + b*(1.0f - t);
}

sf::Color mix(const sf::Color & a, const sf::Color & b, float t)
{
	return sf::Color(lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t), 255);
}
