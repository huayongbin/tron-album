#include "common.h"


enum finger_direction
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN
};



//枚举类型  手指方向
enum finger_direction   get_finger_direction()
{
	//打开触摸屏
	int fd = open("/dev/input/event0", O_RDONLY);

	if ( fd == -1)
	{
		perror("open error");
		return -1;
	}

	int x1 = -1, y1 = -1; //一次滑动过程中，起始点的坐标
	int x2, y2; //一次滑动过程中，终点的坐标


	struct input_event buf;

	while (1)
	{

		int r_ret = read(fd, &buf, sizeof(buf));
		if (r_ret != sizeof(buf))
		{
			
			continue;
		}

		//来的是一个X轴坐标事件
		if (buf.type == EV_ABS &&
			buf.code == ABS_X		)
		{
			if (x1 == -1)
			{
				x1 = buf.value;
			}

			x2 = buf.value;
		}

		//来的是一个Y轴坐标事件
		if (buf.type == EV_ABS &&
			buf.code == ABS_Y		)
		{
			if (y1 == -1)
			{
				y1 = buf.value;
			}

			y2 = buf.value;
		}


		//当手指弹起，
		if (buf.type == EV_KEY &&
			buf.code == BTN_TOUCH &&
			buf.value == 0)
		{

			int delta_x = abs(x2 - x1);
			int delta_y = abs(y2 - y1);

			if (delta_x > 2*delta_y)
			{
				//考虑　左　右

				if (x2 > x1)
				{
					close(fd);
					return MOVE_RIGHT;
				}
				else
				{
					close(fd);
					return MOVE_LEFT;
				}
			}
			else if(2*delta_x < delta_y)
			{	
				//上下
				if (y2 > y1)
				{
					close(fd);
					return MOVE_UP;
				}
				else
				{
					close(fd);
					return MOVE_DOWN;
				}				
			}
			else
			{
				x1 = -1;
				y1 = -1;
				continue;
			}
		}
	}
	close(fd);

}



