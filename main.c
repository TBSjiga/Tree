#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PARENT 1
#define SIBLING 2
#define SELF 3
#define DESCENDANTS 4
#define FAMILY 5

/* Tree structure */
typedef struct node
{
    struct node *parent;
    struct node *right;
    struct node *left;
    int num;
} node;

node* create_root(int num);
int add_node(int num, node* current, int target, char type, int launchflag);
int maximal_depth(node *tree, int n);
int fisheap(node *tree, int curdepth);
int remove_element(node *tree, int numtos, char typedel);
int leavesum, internalnum, nodesnum, isheap, maxdepth;
void check_print(node *tree, int n);
void print_mas(node *tree);
void statistics(node *tree);
void write_node(node *tree, FILE *file);
void load_node(node *tree, FILE *file);
void remove_tree(node *tree);

/* add node */
int add_node(int num, node *current, int target, char type, int launchflag)
{
    static int flag;
    int ntmp = 0;
    node *tmp;

    if (flag) return 1;
    if (!current)
	{
        printf("Node is empty\n");
        return 0;
    }

    if (type == PARENT)
	{
        if (current->num == target)
		{
            if (current->left == 0)
			{
               	 tmp = (node*)malloc(sizeof(node));
              	 tmp->left = tmp->right = 0;
              	 tmp->parent = current;
              	 current->left = tmp;
               	 tmp->num = num;
              	 flag = ntmp = 1;
            }
            else if (current->right == 0)
			{
                tmp = (node*)malloc(sizeof(node));
                tmp->left = tmp->right = 0;
                tmp->parent = current;
                current->right = tmp;
                tmp->num = num;
                flag = ntmp = 1;
            }
        }

        if (!flag)
		{
            if (current->left)
			{
                if (current->right)
				{
                    ntmp = add_node(num, current->left, target, PARENT, 0);
                    if (!flag) ntmp = add_node(num, current->right, target, PARENT, 0);
                }
                else 
				{
                    ntmp = add_node(num, current->left, target, PARENT, 0);
                }
            }
            else 
			{
                if (current->right)
                ntmp = add_node(num, current->right, target, PARENT, 0);
            }
        }
    }

    else if (type == SIBLING)
	{
        if (current->left)
		{
            if (current->right)
			{
                ntmp = add_node(num, current->left, target, SIBLING, 0);
                if (!flag) ntmp = add_node(num, current->right, target, SIBLING, 0);
            }
            else
			{
                if (current->left->num == target)
				{
                    tmp = (node*)malloc(sizeof(node));
                    tmp->left = tmp->right = 0;
                    tmp->parent = current;
                    current->right = tmp;
                    tmp->num = num;
                    flag = ntmp = 1;
                }
                else
				{
                    ntmp = add_node(num, current->left, target, SIBLING, 0);
                }
            }
        }
        else if (current->right)
		{
            if (current->left)
			{
                ntmp = add_node(num, current->left, target, SIBLING, 0);
                if (!flag) ntmp = add_node(num, current->right, target, SIBLING, 0);
            }
            else 
			{
                if (current->right->num == target)
				{
                    tmp = (node*)malloc(sizeof(node));
                    tmp->left = tmp->right = 0;
                    tmp->parent = current;
                    current->left = tmp;
                    tmp->num = num;
                    flag = ntmp = 1;
                }
                else 
				{
                    ntmp = add_node(num, current->right, target, SIBLING, 0);
                }
            }
        }
        else
		{
       	return 0;
		}
    }
    else 
	{
        printf("%s", "Wrong 4th argument of add_node\n");
        return 0;
    }
    if ((flag)&&(launchflag)) flag = 0;
    return (ntmp);
}

/* create node */
node* create_root(int num)
{
    node *tmp;
    tmp = (node*)malloc(sizeof(node));
    tmp->left = tmp->right = tmp->parent = 0;
    tmp->num = num;
    return tmp;
}

/* remove element */
int remove_element(node *tree, int numtos, char typedel)
{
    static int flag;
    int tmp = 0;
    if (tree)
	{
        if (typedel == SELF)
		{
            if (!flag)
                if (tree->left)
				{
                    if (tree->left->num == numtos)
					{
                        if ((tree->left->left == 0)&&(tree->left->right ==0))
						{
                            tmp = flag = 1;
                            free(tree->left);
                            tree->left = 0;
                        }
                    }
                }
            if (!flag)
                if (tree->right)
				{
                    if (tree->right->num == numtos)
					{
                        if ((tree->right->left == 0)&&(tree->right->right == 0))
						{
                            tmp = flag = 1;
                            free(tree->right);
                            tree->right = 0;
                        }
                    }
                }
        }

        else if (typedel == SIBLING)
		{
            if (!flag)
                if (tree->left)
				{
                    if (tree->left->num == numtos)
					{
                        if (tree->right)
                            if ((tree->right->left == 0)&&(tree->right->right == 0))
							{
                                tmp = flag = 1;
                                free(tree->right);
                                tree->right = 0;
                            }
                    }
                }
            if (!flag)
                if (tree->right)
				{
                    if (tree->right->num == numtos)
					{
                        if (tree->left)
                            if ((tree->left->left == 0)&&(tree->left->right == 0))
							{
                                tmp = flag = 1;
                                free(tree->left);
                                tree->left = 0;
                            }
                    }
                }
        }

        else if (typedel == DESCENDANTS)
		{
            if (!flag)
                if (tree->left)
				{
                    if (tree->left->num == numtos)
					{
                        if ((tree->left->left))
						{
                            remove_tree(tree->left->left);
                            tree->left->left = 0;
                        }
                        if ((tree->left->right))
						{
                            remove_tree(tree->left->right);
                            tree->left->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
            if (!flag)
                if (tree->right)
				{
                    if (tree->right->num == numtos)
					{
                        if ((tree->right->left))
						{
                            remove_tree(tree->right->left);
                            tree->right->left = 0;
                        }
                        if ((tree->right->right))
						{
                            remove_tree(tree->right->right);
                            tree->right->right = 0;
                        }
                        tmp = flag = 1;
                    }
                }
        }

        else if (typedel == FAMILY)
		{
            if (!flag)
                if (tree->left)
				{
                    if (tree->left->num == numtos)
					{
                        remove_tree(tree->left);
                        tree->left = 0;
                        tmp = flag =1;
                    }
                }
            if (!flag)
                if (tree->right)
				{
                    if (tree->right->num == numtos)
					{
                        remove_tree(tree->right);
                        tree->right = 0;
                        tmp = flag = 1;
                    }
                }
        }

        else
            printf("Wrong 3rd parameter of remove_element\n");
    }
    else
        return 0;
    if (!flag) tmp = remove_element(tree->left, numtos, typedel);
    if (!flag) tmp = remove_element(tree->right, numtos, typedel);
    if (!tree->parent) flag =0;
    return tmp;
}

/* load tree */
void load_node(node *tree, FILE *file)
{
    int targetn, num;
    if (!fread(&targetn, sizeof(int), 1, file)) return;
    if (!fread(&num, sizeof(int), 1, file)) return;
    add_node(num, tree, targetn, PARENT, 1);
    load_node(tree, file);
}

/* remove tree */
void remove_tree(node *tree)
{
    if (!tree) return;
    if (tree->left)
        remove_tree(tree->left);
    if (tree->right)
        remove_tree(tree->right);
    free(tree);
}

/* save tree */
void write_node(node *tree, FILE *file)
{
    if (tree->parent)
	{
        fwrite(&tree->parent->num, sizeof(int), 1, file);
        fwrite(&tree->num, sizeof(int), 1, file);
    }
    else 
	{
        fwrite(&tree->num, sizeof(int), 1, file);
    }
    if (tree->left) write_node(tree->left, file);
    if (tree->right) write_node(tree->right, file);
}

int maximal_depth(node *tree, int n)
{
    int a = 0, b = 0;
    if (tree)
	{
        ++n;
        a = maximal_depth(tree->left, n);
        b = maximal_depth(tree->right, n);
        if (a>n) n=a;
        if (b>n) n=b;
    }
    return n;
}

void check_print(node *tree, int n)
{
    int i;
    if (tree)
	{
        check_print(tree->right, n+3);
        for (i=0; i<n; i++)
            putchar(' ');
        printf("%d\n", tree->num);
        check_print(tree->left, n+3);
    	}
}

void print_mas(node *tree)
{
    if (tree)
	{
        print_mas(tree->left);
        printf("%d", tree->num);
        print_mas(tree->right);
    }
}

int fisheap(node *tree, int curdepth){
    extern int maxdepth;
    static int flag, flage1;
    int tmp = 0;
    ++curdepth;
    if (tree)
	{
        if ((!flag))
            if (tree->parent)
                if ((tree->parent->num)<(tree->num))
                    flag = tmp = 1;
        if (!flag)
            if ((curdepth+1)<(maxdepth))
			{
                if (!((tree->left)&&(tree->right)))
                    tmp = flag = 1;
            }
        if (curdepth == maxdepth)
            if (flage1)
                tmp = flag = 1;
        if (!flag)
            tmp = fisheap(tree->left, curdepth);

        if (!flag)
            tmp = fisheap(tree->right, curdepth);
    }

    else
	{
        if (!flag)
            if (curdepth == maxdepth)
                flage1 = 1;
    }
    if (curdepth == 1)
	{
        flag = 0;
        flage1 = 0;
    }
    return tmp;
}

void statistics(node *tree)
{
    extern int leavesum, internalnum, nodesnum;
    if (tree)
	{
        ++nodesnum;
        if ((tree->right)||(tree->left))
            ++internalnum;
        else
            ++leavesum;
        if (tree->right)
            statistics(tree->right);
        if (tree->left)
            statistics(tree->left);
    }
}

int main(int argc, char *argv[])
{
    int i, numtow, numtos;
    node *tree = 0;
    FILE *file = 0;
    char command[500],flag = 8, *c=0;

	/* Open file */
    if (argc > 1)
	{
        if ((file = fopen(argv[1], "rb")) == 0)
            printf("Error opening\n");
        else 
		{
            fread(&numtow, sizeof(int), 1, file);
            tree = create_root(numtow);
            load_node(tree, file);
            fclose(file);
        }
    }

    else
        printf("No tree was loaded from command line arguments\n");

	/* Work with commands */
    while (flag)
	{
        leavesum = internalnum = nodesnum = maxdepth = 0;
        statistics(tree);
        maxdepth = maximal_depth(tree, 0);
        isheap = fisheap(tree, 0);
        scanf("%s", command);

        if (!strcmp(command, "add"))
		{
            scanf("%s", command);
            if (!strcmp(command, "root"))
			{
                if (tree == 0)
				{
                    flag = 10;
                    scanf("%s", command);
                    i=0;
                    while (command[i])
					{
                        if ((!(command[i]>='0'))||(!(command[i]<='9')))
                            flag = 2;
                        ++i;
                    }

                    if (flag == 2)
					{
                        printf("Not correct num\n");
                    }
                    else if (flag == 10)
					{
                        sscanf(command, "%d", &numtow);
                        tree = create_root(numtow);
                    }
                    flag = 8;
                }
                else
                    printf("Root already exists\n");
            }

            else if (!strcmp(command, "node"))
			{
                flag = 10;
                scanf("%s", command);
                i = 0;
                while (command[i])
				{
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag = 2;
                    ++i;
                }

                if (flag == 2)
				{
                    printf("Not correct numbers\n");
                }
                else if (flag == 10)
				{
                    sscanf(command, "%d", &numtow);
                    scanf("%s", command);
                    if (!strcmp(command, "parent"))
					{
                        scanf("%s", command);
                        if (!strcmp(command, "root"))
						{
                            if (tree)
							{
                                if (!tree->left)
								{
                                    tree->left = create_root(numtow);
                                    tree->left->parent = tree;
                                }
                                else if (!tree->right)
								{
                                    tree->right = create_root(numtow);
                                    tree->right->parent = tree;
                                }
                                else
                                    printf("%s", "Root has no free descendants\n");
                            }
                            else
                                printf("There is no root\n");
                        }
                        else{
                            flag = 10;
                            i = 0;
                            while (command[i])
							{
                                if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                    flag = 2;
                                ++i;
                            }
                            if (flag == 2)
							{
                                printf("Not correct num\n");
                            }
                            else if (flag == 10)
							{
                                sscanf(command, "%d", &numtos);
                                if (add_node(numtow, tree, numtos, PARENT, 1));
                                else printf("No element with numbers %d or it already has 2 descendants\n", numtos);
                            }
                        }
                    }
                    else if (!strcmp(command, "sibling"))
					{
                        scanf("%s", command);
                        flag = 10;
                        i = 0;
                        while (command[i])
						{
                            if ((!(command[i]>='0'))||(!(command[i]<='9')))
                                flag = 2;
                            ++i;
                        }
                        if (flag == 2)
						{
                            printf("Not correct numbers\n");
                        }
                        else if (flag == 10)
						{
                            sscanf(command, "%d", &numtos);
                            if (add_node(numtow, tree, numtos, SIBLING, 1));
                            else printf("No element with num %d or it already has a sibling\n", numtos);
                        }
                    }
                    else
                        printf("%s", "Wrong command\n");
                    flag = 8;
                }
                flag = 8;
            }
            else
                printf("%s", "Wrong command\n");
        }

        else if (!strcmp(command, "rem"))
		{
            scanf("%s", command);
            if (!strcmp(command, "whole"))
			{
                remove_tree(tree);
                tree = 0;
                printf("Tree was deleted\n");
            }
            else if (!strcmp(command, "node"))
			{
                flag = 10;
                scanf("%s", command);
                i = 0;
                while (command[i])
				{
                    if ((!(command[i]>='0'))||(!(command[i]<='9')))
                        flag = 2;
                    ++i;
                }
                if (flag == 2)
				{
                    printf("Not correct num");
                }
                else if (flag == 10)
				{
                    sscanf(command, "%d", &numtos);
                    scanf("%s", command);
                    if (!strcmp(command, "self"))
					{
                        if (!remove_element(tree, numtos, SELF)) printf("Element not found or has descendant\n");
                    }
                    else if (!strcmp(command, "sibling"))
					{
                        if (!remove_element(tree, numtos, SIBLING)) printf("Element not found, has no sibling\n");
                    }
                    else if (!strcmp(command, "descendants"))
					{
                        if (!remove_element(tree, numtos, DESCENDANTS)) printf("Element not found\n");
                    }
                    else if (!strcmp(command, "family"))
					{
                        if (!remove_element(tree, numtos, FAMILY)) printf("Element not found");
                    }
                    else
                        printf("%s", "Wrong command\n");
                }
                else
                    printf("UNKNOWN ERROR\n");
                flag = 8;
            }
            else printf("%s", "Wrong command\n");
        }
        else if (!strcmp(command, "save"))
		{
            fgets(command, sizeof(command), stdin);
            c = command;
            while((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "wb"))!= 0)
			{
                write_node(tree, file);
                fclose(file);
            }
            else
                printf("Error creating file\n");
        }
        else if (!strcmp(command, "load"))
		{
            fgets(command, sizeof(command), stdin);
            c = command;
            while ((*(c))!='\n') ++c;
            *c = '\0';
            if ((file = fopen(command, "rb"))!= 0)
			{
                if (tree)
				{
                    remove_tree(tree);
                    tree = 0;
                }
                fread(&numtow, sizeof(int), 1, file);
                tree = create_root(numtow);
                load_node(tree, file);
                statistics(tree);
                maxdepth = maximal_depth(tree, 0);
                fclose(file);
            }
            else
			{
                printf("File not\n");
                check_print(tree, 0);
                printf("Numbers of nodes: %d. \nNumbers of internal nodes: %d.\nNumber of leaves: %d.\nMaximal depth of tree: %d.\n", nodesnum,internalnum, leavesum, maxdepth);
			}
        }
        else if (!strcmp(command, "help"))
		{
            printf("HELP");
            fflush(stdin);
            getchar();
        }

        else if (!strcmp(command, "show"))
		{
            check_print(tree, 0);
            printf("Numbers of nodes: %d. \nNumbers of internal nodes: %d.\nNumber of leaves: %d.\nMaximal depth of tree: %d.\n", nodesnum,internalnum, leavesum, maxdepth);

        }
        else if (!strcmp(command, "exit"))
            return 0;
        else
		{
            printf("%s", "Wrong command\n");
        }
        fflush(stdin);
    }
    return 0;
}
