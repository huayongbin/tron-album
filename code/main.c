#include "common.h"
#include "toutch.h"
#include "bmp.h"
#include "manage.h"



int main(int argc, char **argv)
{
	//初始化头节点
	linklist head = init_node();
	//把目录中的文件信息放入链表
	head = manage(head, argc, argv);
	//定义tmp指向head的下一个
	linklist tmp = head->next;	
	//库参数化
	void *handle;			
	void (*display)(char *name);
	void (*freelcd)(void);
	
	
	
	while(1)
	{	
		if(tmp != head)
		{
			//链接库函数
			if(strstr(tmp->name, ".bmp"))
			{
				handle = dlopen("libbmp.so", RTLD_NOW);
			}
			if(strstr(tmp->name, ".jpg"))
			{
				handle = dlopen("libjpg.so", RTLD_NOW);
			}
			
			display = dlsym(handle, "display");
			freelcd = dlsym(handle, "freelcd");
		
			if(display == NULL )
			{
				perror("error");
				exit(0);
			}	
			//输出图片名称
			printf("%s\n",tmp->name);
			//显示图片
			display(tmp->name);
			
			//触屏操作
			enum finger_direction W;
			W=get_finger_direction();
			//右移
			if (W==MOVE_LEFT)
			{
				tmp= tmp->next;
				while(tmp==head)
				{
					tmp = tmp->next;
				}
			}
			//左移
			 if(W == MOVE_RIGHT) 
			{
				tmp=tmp->prev;
				while(tmp==head)
				{
					tmp = tmp->prev;
				}
			}
			//上下是关闭
			if(W==MOVE_UP || W==MOVE_DOWN) 
			{
				break;
			}
		}		
	} 
	//关闭lcd,触摸屏
	freelcd();
	
	
	return 0;
}