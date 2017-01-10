#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INT_MAX_LEN 12	/* 2^32 = 4294967296, 最多10位数 */
#define SPLIT_CHAR ','

#define FREE_P(x) \
do { \
	if (NULL != (x)) \
	{ \
		free(x); \
		(x) = NULL; \
	} \
} while(0)


typedef struct tree_node
{
	int data;
	struct tree_node *left_child;
	struct tree_node *right_child;
} tree_node;

tree_node *create_binary_tree(char *line, unsigned int *node_num)
{
	int num = 0;
	int i = 0;
	int check_child = 1;
	char *p = NULL;
	char *q = NULL;
	char tmp[INT_MAX_LEN] = {0};
	tree_node *binary_tree = NULL;
	
	if (NULL == line)
	{
		return NULL;
	}

	p = line;
	while (p && *p != '\0')
	{
		if (*p == ',')
		{
			num = num + 1;
		}
		
		p++;
	}

	*node_num = num + 1;

	/* 申请二叉树所需空间 */
	binary_tree = (tree_node *)malloc(sizeof(tree_node) * (*node_num));
	memset(binary_tree, 0, sizeof(tree_node) * (*node_num));
	if (NULL == binary_tree)
	{
		return NULL;
	}

	q = p = line;
	while (p && '\0' != *p)
	{
		p = strchr(q, SPLIT_CHAR);
		if (p && '\0' != *p)
		{
			/* 这里根据题目描述信息，p-q的长度不应超过INT_MAX_LEN */
			memset(tmp, 0, INT_MAX_LEN);
			strncpy(tmp, q, p-q);
			tmp[p-q] = '\0';
			binary_tree[i].data = atoi(tmp);
		}
		else
		{
			binary_tree[i].data = atoi(q);
			break;
		}

		p++;
		q = p;
		i++;

		if (check_child)
		{
			/* 后续节点，必定没有左或右子树，修改检查标记 */
			if ((2 * i) > *node_num) 
			{
				check_child = 0;
			}			
			
			if ((2 * i) <= *node_num)
			{
				binary_tree[i - 1].left_child = &(binary_tree[2 * i - 1]);
			}

			if ((2 * i + 1) <= *node_num)
			{
				binary_tree[i - 1].right_child = &(binary_tree[2 * i]);
			}
		}	
	}

	return binary_tree;
}

int print_tree_path(tree_node *tree_root, int current_sum, int expect_sum, int *path)
{
	static int found = -1;
	static int level = 0;
	int i = 0;

	if (NULL == tree_root)
	{
		return found;
	}

	/* 叶子结点 */
	if (NULL == tree_root->left_child)
	{
		/* 路径和与期望值不符 */
		if (current_sum + tree_root->data != expect_sum)
		{
			return found;
		}

		/* 找到符合要求的路径，输出完整路径 */
		for (i = 0; i < level; i++)
		{
			printf("%d,", path[i]);
		}
		printf("%d\n", tree_root->data);

		found = 0;
		return found;
	}

	/* 非叶子结点，但路径和已大于等于期望和，直接返回 */
	if (current_sum + tree_root->data >= expect_sum)
	{
		return found;
	}
	/* 非叶子结点，递归寻找潜在路径 */
	else
	{
		path[level] = tree_root->data;
		current_sum += tree_root->data;
		level += 1;
		print_tree_path(tree_root->left_child, current_sum, expect_sum, path);
		print_tree_path(tree_root->right_child, current_sum, expect_sum, path);
		level -= 1;
		current_sum -= tree_root->data;
		path[level] = 0;
		
		return found;
	}

	/* 若走到这里，说明没有符合的路径，main函数中输出error */
	return found;
}

int main(int argc, char **argv)
{
	ssize_t read = -1;
	size_t len = 0;
	
	int expect_sum = 0;
	int found = 0;
	
	unsigned int node_num = 0;
	unsigned int level = 0;

	int *path = NULL;
	char *line = NULL;
	
	tree_node *binary_tree = NULL;

	/* 从标准输入中获取期望值，以及二叉树整数串 */
	if (EOF != scanf("%d", &expect_sum) && getchar())
	{
		read = getline(&line, &len, stdin);
		if (-1 == read)
		{
			return -1;
		}
		else if (0 >= expect_sum)
		{
			printf("error\n");
			FREE_P(line);
			return -1;
		}
	}

	/* 根据输入的数据，创建二叉树  */
	binary_tree = create_binary_tree(line, &node_num);
	FREE_P(line); /* 后续不再使用到line，故先释放空间 */
	
	if (NULL == binary_tree)
	{
		printf("error\n");	
		return -1;		
	}

	/* 计算二叉树的深度，申请合适的路径存储空间 */
	if (0 == ((node_num + 1) & (node_num)))
	{
		level = (unsigned int)log2(node_num + 1);
	}
	else
	{
		level = (unsigned int)log2(node_num + 1) + 1;
	}

	path = (int *)malloc(sizeof(int) * level);
	if (!path)
	{
		FREE_P(binary_tree);
		return -1;
	}

	/* 在二叉树中找出和为某一值的所有路径 */
	found = print_tree_path(&binary_tree[0], 0, expect_sum, path);
	if (-1 == found)
	{
		printf("error\n");
	}

	FREE_P(binary_tree);
	FREE_P(path);
	return 0;
}

