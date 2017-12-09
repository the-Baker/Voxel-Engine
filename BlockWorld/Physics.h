#pragma once
#include <glm\glm.hpp>

#define GRAVITY -9.81f
#define FRICTION 20.0f
#define AIR_RESISTANCE 5.0f

bool AABB3DTest(glm::vec3 box1Position, glm::vec3 box1Dimensions, glm::vec3 box2Position, glm::vec3 box2Dimensions)
{
	float box1East   = box1Position.x;
	float box1West   = box1Position.x + box1Dimensions.x;
	float box1North  = box1Position.z + box1Dimensions.z;
	float box1South  = box1Position.z;
	float box1Top    = box1Position.y + box1Dimensions.y;
	float box1Bottom = box1Position.y;

	float box2East   = box2Position.x;
	float box2West   = box2Position.x + box2Dimensions.x;
	float box2North  = box2Position.z + box2Dimensions.z;
	float box2South  = box2Position.z;	   
	float box2Top    = box2Position.y + box2Dimensions.y;
	float box2Bottom = box2Position.y;

	return (box1East    < box2West  && box1West  > box2East
		 && box1North   > box2South && box1South < box2North
		 && box1Bottom  < box2Top   && box1Top   > box1Bottom);
}