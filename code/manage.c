#include "common.h"
#include "bmp.h"
#include "toutch.h"
#include "manage.h"

/* typedef struct node 
{
	struct node *prev;
	struct node *next;
	char *name;
}listnode, *linklist; */


linklist init_node(void)
{
	struct node *head = calloc(1, sizeof(struct node));
	
	if(head != NULL)
	{
		head->prev = head->next = head;
	}
	
	return head;
}


struct node *new_node(char *name)
{
	struct node* new = calloc(1, sizeof(struct node));
	
	if(new != NULL)
	{
		new->prev = new->next = new;
		new->name = name;
	}
	
	return new;
}

void list_add_tail(struct node *head, struct node  *new)
{
	new->prev = head->prev;
	new->next = head;
	head->prev->next = new;
	head->prev = new;
	
}



linklist manage(linklist head,int argc, char **argv)
{
	if(argc > 2)
	{
		printf("参数有误\n");
		exit(0);
	}
	
	// 如果无指定文件的情况下，默认列出当前目录
	char *target;
	target = (argc == 1) ? "." : argv[1];
	
	// 判断要处理的文件的类型
	struct stat *info = calloc(1, sizeof(struct stat));
	stat(target, info);
	
	
	//目录
	if(S_ISDIR(info->st_mode))
	{
		DIR *dp = opendir(target);
		
		chdir(target); // 进入目录
		
		struct dirent *ep;
		while(1)
		{
			ep = readdir(dp);
			
			if(ep == NULL)
				break;
			if(ep->d_name[0] == '.')
				continue;
						
			if(strstr(ep->d_name, ".bmp" ) 
		    || strstr(ep->d_name, ".jpg"))
			{
				printf("%s\n",ep->d_name);
						
				struct node *new = new_node(ep->d_name);
				
				list_add_tail(head, new);				
			}
		}					
	}
	else
	{
		exit(0);
	}
	return head; 
}