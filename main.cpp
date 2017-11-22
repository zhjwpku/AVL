#include <iostream>
#include <stdio.h>
#include "AVL.h"

int main(int argc, char* argv[])
{
	AVL* root = NULL;
	int vector[] = {15,6,18,3,7,17,20,2,4,13,9};
	const int length = sizeof(vector)/sizeof(int);
	for(int i = 0; i< length;i++)
		root = Insert(root, vector[i]);
	
	printf("\nInOrder: \n");
	printBST(root, 1, 0);

	int order, value;
	
	while(1)
	{
        printf("\nplease input the order and the value(0 -> add / 1 -> delete): ");
        scanf("%d %d",&order, &value);
        if (order == 0) {
            root = Insert(root, value);
        } else {
		    root = Delete(root, value);
        }
		printf("\nAfter your order:\n");
	    printBST(root, 1, 0);
	}
	printf("\n");
	return 0;
}
