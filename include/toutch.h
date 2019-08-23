#ifndef __TOUTCH_H_
#define __TOUTCH_H_

#include "common.h"

enum finger_direction
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};

enum finger_direction   get_finger_direction();

#endif