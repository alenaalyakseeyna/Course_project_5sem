#include "AI.h"
#include "Level.h"
#include "../Utilities/Vector2D.hpp"
#include <time.h>
#include <stdlib.h>

class Level;

AI::~AI(void)
{
}

AI::AI()
{
	counter = 0;
	offset = 10;
	prevDX = 2;
}

CollisionPointGroup AI::CheckCollision(CMovableObject* object) const
{
	return level->CheckCollision(object);
}

bool AI::CheckCollisionWithEnemies(CMovableObject* object) const
{
	return level->CheckCollisionWithEnemies(object);
}

bool AI::WillFall(CMovableObject* object, int steps) const
{
	return level->WillFall(object, steps);
}

void AI::SetLevel(Level* level_p)
{
	level = level_p;
}

void AI::EnemyPatrol(CMovableObject* object) {
	
	/*
	��������� ��� ��������������:
1. ������� ������.
2. �������� 3 ���� � ���� �������, ����� ������������ � �������� ���� ���, ������������ � ��� �����.
3. �����������, ����� �� ���������� � ������ ��� ��������� �� ����, ��� �� ������ � ���������, ��� �� ������ ���������.
4. ������� ��������� ���������.
	*/

	offset = (rand()%30);
	
	int direction = object->movementVector.DX();
	UCHAR moveSpeed = 1;
	
	if(direction == 0){
		object->movementVector.SetDX(moveSpeed);
	}

	if(counter == 50+offset || counter == 80+offset){
		object->movementVector.SetDX(-direction);		
	}

	direction = object->movementVector.DX();
	CheckCollision(object);
	if(object->movementVector.DX() != direction || WillFall(object, 1)){
		object->movementVector.SetDX(-direction);
		genBool = true;
	}

	counter++;
	if(counter>110+offset) counter =0;
}

void AI::EnemyHopping(CMovableObject* object) 
{
	/*
��������� ������:
1. ������� ������.
2. �������� 3 ���� � ���� �������, ����� ������������, �������� � �������� ���� ���, ������������ � �������� � ��� �����.
3. �����������, ����� �� ���������� � ������ ��� ��������� �� ����, ��� �� ������ � ���������, ��� �� ������ ���������.
4. ������� ������������ ��������� ??������.
5. ������� ��������� ���������.
	*/

	int direction = object->movementVector.DX();

	if(direction == 0 && object->movementVector.DY() == 0){
		object->movementVector.SetDX(prevDX);
	}

	if(object->movementVector.DY() != 0 && object->movementVector.DX()!=0)
	{
		prevDX = object->movementVector.DX();
		object->movementVector.SetDX(0);
		genBool = true;
	}

	if(counter > 20+offset)
	{
		if(rand()%100 < 80)
			object->movementVector.SetDX(-direction);	
		object->movementVector.SetDY(-((rand()%6)+10)); // jumps in random distance
		counter = 0;
		offset = rand()%20;
	}
	else
	{
		counter++;	
	}
	
	direction = object->movementVector.DX();

	CheckCollision(object);
	if(object->movementVector.DX() != direction || WillFall(object, 1))
	{
		object->movementVector.SetDX(-direction);
	}
}

void AI::EnemyRam(CMovableObject *object)
{
	/*
	Ramming behavior:
1. ������� ������.
2. ������ � ������ ���� ���������, ��� ��� ������ ���������, ����� ������������ � ������ � ������� ���� ���������, ��� ��� ��������� � ������ ������, � ��� �����.
3. �� �������������� ��� ������������ � ������.
	*/

	int direction = object->movementVector.DX();

	if(counter==0)
	{
		direction = (int)((double)prevDX*1.5);
		prevDX = direction;

		if((direction >= 0  &&  direction > 14) || (direction < 0  && direction < -14))
		{
			if(direction > 0)
				direction = 14;
			else
				direction = -14;
			prevDX = direction;
		}
		object->movementVector.SetDX(direction);
	}
	else
	{
		counter--;
	}

	CheckCollision(object);
	if(object->movementVector.DX() != direction || WillFall(object, 1))
	{
		object->movementVector.SetDX(-direction);
		
		counter = rand()%100+50;
		if(object->movementVector.DX() > 0)
			prevDX = 2;
		else
			prevDX = -2;
		object->movementVector.SetDX(0);
		genBool = true;
	}

}

void AI::EnemyBoss(CMovableObject* object) 
{
	/*
��������� �����:
1. ������������� �� �������������� ���������.
2. ������������� �� �����������.
3. �������� ������������ ���������.
	*/

	if(switchBool)
		EnemyHopping(object);
	else
		EnemyRam(object);
	
	if(genBool)
	{
		switchBool = !switchBool;
		genBool = false;
	}
}

void AI::EnemyStop(CMovableObject* object) {
	object->movementVector.SetDX(0);
}

void AI::RemoveFromRepository(Enemy* e)
{
	level->RemoveFromRepository(e);
}

bool AI::BoundingBoxCollision(CMovableObject* o, int left2, int right2, int top2, int bottom2) const
{
	return level->BoundingBoxCollision(o, left2, right2, top2, bottom2);
}