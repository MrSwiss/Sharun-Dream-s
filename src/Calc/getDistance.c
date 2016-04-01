#include "Player.h"

float getDistance(float sX, float sY, float sZ, float tX, float tY, float tZ)
{
  float X = tX - sX;
  float Y = tY - sY;
  float Z = tZ - sZ;
  return sqrt(X * X + Y * Y + Z * Z)/25.5;
}

float getDistance_2D(float *s, float *t)
{
  float Dist = getDistance(s[0], s[1], 0, t[0], t[1], 0);
  return Dist;
}

float getDistance_3D(float *s, float *t)
{
  float Dist = getDistance(s[0], s[1], s[2], t[0], t[1], t[2]);
  return Dist;
}
