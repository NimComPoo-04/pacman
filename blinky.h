#ifndef _BLINKY_H_
#define _BLINKY_H_

#include "entity.h"

enum
{
	SCATTER,
	CHASE,
	FRIGHTENED,
	ANGRY
};

typedef struct
{
	entity_t entity;
	int state;
} blinky_t;


#endif
