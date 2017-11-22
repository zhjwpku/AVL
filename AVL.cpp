/**
 @brief  An implementation of AVL Tree, including delete, insert, search(skipped cause AVL is one kind of BST)
 */

#include "AVL.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <deque>
#include <cmath>
#include <iomanip>

using namespace std;

AVL* New_Node(KEY_TYPE key, AVL* lchild, AVL* rchild, int height)
{
	AVL* p_avl = new AVL;
	p_avl->key = key;
	p_avl->lchild = lchild;
	p_avl->rchild = rchild;
	p_avl->height = height;
	return p_avl;
}

inline int getHeight(AVL* node) {
	return (node==NULL)? -1:node->height;
}

inline int max(int a, int b) {
	return a>b?a:b;
}

/* LL(t23 rotates to the right):

        X                    Z
       /  \                 /  \
      Z   t1     ==>       t3   X
     / \                       /  \
    t4  t23                  t23  t1
*/
/*
 Return which the root pointer(at a higher level) should point to
 */
AVL* LL_Rotate(AVL* X)
{
	AVL* Z = X->lchild;
	X->lchild = Z->rchild;
	Z->rchild = X;
	X->height = max(getHeight(X->lchild), getHeight(X->rchild)) + 1;
	Z->height = max(getHeight(Z->lchild), X->height) + 1;
	return Z;
}

/* RR(t23 rotates to the left):

         X                       Z
       /  \                     /  \
      t1    Z         ==>      X   t4
          /  \                /  \
         t23  t4             t1   t23
 */
AVL* RR_Rotate(AVL* X)
{
	AVL* Z = X->rchild;
	X->rchild = Z->lchild;
	Z->lchild = X;
	X->height = max(getHeight(X->lchild), getHeight(X->rchild)) + 1;
	Z->height = max(getHeight(Z->rchild), X->height) + 1;
	return Z;
}


/* LR(t2 rotates to the left, then t3 rotates to the right):

      X                          X                        Y
     /  \                       /  \                     /  \
    Z    t4                    Y   t4                   Z    X
   /  \         ==>           /  \        ==>          / \   / \
  t1    Y                    Z    t3                  t1 t2 t3  t4
      /  \                  /  \
     t2    t3              t1   t2

*/
/*
 Return which the root pointer should point to
 */
AVL* LR_Rotate(AVL* X)
{
	X->lchild = RR_Rotate(X->lchild);
	return LL_Rotate(X);
}


/* RL(t3 rotates to the right, then t2 rotates to the left):

       X                          X                           Y
      /  \                       /  \                        /  \
     t1    Z                    t1    Y                     X    Z 
         /  \       ==>             /  \         ==>       /  \  / \
        Y   t4                     t2    Z                t1  t2 t3  t4
       /  \                            /  \
      t2   t3                         t3   t4 

 */
AVL* RL_Rotate(AVL* X)
{
	X->rchild = LL_Rotate(X->rchild);
	return RR_Rotate(X);
}

/* return which the root pointer(at an outer/higher level) should point to,
   the root_node of AVL tree may change frequently during delete/insert,
   so the Root pointer should point to the REAL root node.
 */
AVL* Insert(AVL* root, KEY_TYPE key)
{
	if(root == NULL)
		return (root = New_Node(key, NULL, NULL));
	else if(key < root->key)
		root->lchild = Insert(root->lchild, key);
	else //key >= root->key
		root->rchild = Insert(root->rchild, key);
	
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
	if(getHeight(root->lchild) - getHeight(root->rchild) == 2)
	{
		if(key < root->lchild->key)     // Left Left
			root = LL_Rotate(root);
		else                            // Left Right
			root = LR_Rotate(root);
	}
	else if(getHeight(root->rchild) - getHeight(root->lchild) == 2)
	{
		if(key < root->rchild->key)     // Right Left
			root = RL_Rotate(root);
		else                            // Right Right
			root = RR_Rotate(root);
	}
	return root;
}


/* return which the root pointer(at an outer/higher level) should pointer to,
   cause the root_node of AVL tree may change frequently during delete/insert,
   so the Root pointer should point to the REAL root node.
 */
AVL* Delete(AVL* root, KEY_TYPE key)
{
	if(!root)
		return NULL;
	if(key == root->key)
	{
		if(root->rchild == NULL)
		{
			AVL* temp = root;
			root = root->lchild;
			delete(temp);
			return root;
		}
		else
		{
			AVL* temp = root->rchild;
			while(temp->lchild)
				temp = temp->lchild;
			/* replace the value */
			root->key = temp->key;
			/* Delete the node (successor node) that should be really deleted */
			root->rchild = Delete(root->rchild, temp->key);
		}
	}
	else if(key < root->key)
		root->lchild = Delete(root->lchild, key);
	else
		root->rchild = Delete(root->rchild, key);
	
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
	if(getHeight(root->rchild) - getHeight(root->lchild) == 2)
	{
		if(getHeight(root->rchild->rchild) >= getHeight(root->rchild->lchild))
			root = RR_Rotate(root);
		else
			root = LR_Rotate(root);
	}
	else if(getHeight(root->lchild) - getHeight(root->rchild) == 2)
	{
		if(getHeight(root->lchild->lchild) >= getHeight(root->lchild->rchild))
			root = LL_Rotate(root);
		else
			root = RL_Rotate(root);
	}
	return root;
}

void InOrder(AVL* root)
{
	if(root)
	{
		InOrder(root->lchild);
		printf("key: %d height: %d ", root->key, root->height);
		if(root->lchild)
			printf("left child: %d ", root->lchild->key);
		if(root->rchild)
			printf("right child: %d ", root->rchild->key);
		printf("\n");
		InOrder(root->rchild);
	}
}

// Convert an integer to string
string intToString(int val) {
    ostringstream ss;
    ss << val;
    return ss.str();
}

// Print the arm branches (eg, / \ ) on a line
void printBranches(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<AVL*>& nodesQueue) {
    deque<AVL*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel / 2; i++) {
        cout << ((i == 0) ? setw(startLen - 1) : setw(nodeSpaceLen - 2)) << "" << ((*iter++) ? "/" : " ");
        cout << setw(2 * branchLen + 2) << "" << ((*iter++) ? "\\" : " ");
    }
    cout << endl;
}

// Print the branches and node (eg, ___10___ )
void printNodes(int branchLen, int nodeSpaceLen, int startLen, int nodesInThisLevel, const deque<AVL*>& nodesQueue) {
    deque<AVL*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        cout << ((i == 0) ? setw(startLen) :setw(nodeSpaceLen)) << "" << ((*iter && (*iter)->lchild) ? setfill('_') : setfill(' '));
        cout << setw(branchLen + 2) << ((*iter) ? intToString((*iter)->key) : "");
        cout << ((*iter && (*iter)->rchild) ? setfill('_') : setfill(' ')) << setw(branchLen) << "" << setfill(' ');
    }
    cout << endl;
}

// Print the leaves only (just for the bottom row)
void printLeaves(int indentSpace, int level, int nodesInThisLevel, const deque<AVL*>& nodesQueue) {
    deque<AVL*>::const_iterator iter = nodesQueue.begin();
    for (int i = 0; i < nodesInThisLevel; i++, iter++) {
        cout << ((i == 0) ? setw(indentSpace + 2) : setw(2 * level + 2)) << ((*iter) ? intToString((*iter)->key) : "");
    }
    cout << endl;
}

void printBST(AVL* root, int level, int indentSpace) {
    int h = root->height + 1;
    int nodesInThisLevel = 1;

    int branchLen = 2 * ((int)pow(2.0, h) -1 ) - (3 - level) * (int)pow(2.0, h-1);   // eq of the length of branch for each node of each level
    int nodeSpaceLen = 2 + (level + 1) * (int)pow(2.0, h);  // distance between left neighbor node's right arm and right neighbor node's left arm
    int startLen = branchLen + (3-level) + indentSpace;     // starting space to the first node to print of each level (for the left most node of each level only)

    deque<AVL*> nodesQueue;
    nodesQueue.push_back(root);
    for (int r = 1; r < h; r++) {
        printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue);
        branchLen = branchLen / 2 - 1;
        nodeSpaceLen = nodeSpaceLen / 2 + 1;
        startLen = branchLen + (3 - level) + indentSpace;
        printNodes(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue);

        for (int i = 0; i < nodesInThisLevel; i++) {
            AVL *currNode = nodesQueue.front();
            nodesQueue.pop_front();
            if (currNode) {
                nodesQueue.push_back(currNode->lchild);
                nodesQueue.push_back(currNode->rchild);
            } else {
                nodesQueue.push_back(NULL);
                nodesQueue.push_back(NULL);
            }
        }

        nodesInThisLevel *= 2;
    }
    printBranches(branchLen, nodeSpaceLen, startLen, nodesInThisLevel, nodesQueue);
    printLeaves(indentSpace, level, nodesInThisLevel, nodesQueue);
}
