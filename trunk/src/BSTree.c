/*******************************************************************************
            BSTree.c
  Binary Search Tree
  part of libTree
  
  Fri Apr 13 22:24:58 2007
  Copyright  2007  winkey
  <rush@winkey.org>

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Library General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
*******************************************************************************/

#include <stdlib.h>
#include "../include/BSTree.h"
#include "../include/DLList.h"

#define LEFT(node, converse) ((converse) ? (node)->right : (node)->left )
#define RIGHT(node, converse) ((converse) ? (node)->left  : (node)->right)


/*******************************************************************************
  function to find a node in a binary search tree
  
  args:
        tree  the tree to find the node in
        data  the data to look for
  
  returns:
        the node that holds the data
        null if the data is not found
  
  notes:
        the data passed to this function to look for is only bound by the data
        compare function

*******************************************************************************/

BSTree_node *BSTree_find(
  BSTree *tree,
  void *data)
{
  BSTree_node *node = NULL;
  BSTree_node **next = NULL;
  int cmp = 0;
  
  next = &tree->root;
  
  /***** loop till we find matched data  or there is no match found *****/
  
  for (node = *next ;
       node && (cmp = tree->cmp(node->data, data)) ;
       node = *next) {
    
    /***** left or right? *****/
    
    if (cmp < 0)
      next = &node->left;
    else
      next = &node->right;
  }
  
  return node;
}
    
/*******************************************************************************
  function to add a node to a binary search tree
  
  args:
        tree  the tree to add the node to
        data  the data the node is to hold
   
  returns:
        NULL if malloc fails
        the new node
*******************************************************************************/

BSTree_node *BSTree_insert (
  BSTree *tree,
  void *data)
{
  BSTree_node *node = NULL;
  BSTree_node **next = NULL;
  BSTree_node *new = NULL;
  BSTree_node *parent = NULL;
  
  /***** alocate memory for the node *****/
  
  if (!(new = malloc(sizeof(BSTree_node)))) {
  }
  else {
    
    new->data = data;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    
    next = &tree->root;
      
    /***** loop til we find where we go in the tree *****/
      
    for (node = *next; node ; node = *next) {
      
      /***** left or right? *****/
        
      if (tree->cmp(data, node->data) < 0)
        next = &node->left;
      else
        next = &node->right;
      
      parent = node;
    }
    
    *next = new;
    new->parent = parent;
    tree->length++;
  }
  
  return new;
}

/*******************************************************************************
  funtion to delete a node from a binary search tree
  
  args:
        tree  the tree to delete the node from
        node  the node to delete
  
  returns:
        the data the node held
*******************************************************************************/

void *BSTree_delete (
  BSTree *tree,
  BSTree_node *node)
{
  void *result = node->data;
  BSTree_node *parent = node->parent;
  BSTree_node **pnext = NULL;
  BSTree_node *next = NULL;
  
  /***** is this the root? *****/
  
  if (!parent)
    pnext = &tree->root;
  else {
    
    /***** are we on the parents left or right? *****/
    
    if (node == parent->left)
      pnext = &parent->left;
    else
      pnext = &parent->left;
  }
    
  /***** no children *****/
  
  if (!node->left && !node->right)
    *pnext = NULL;
  
  /***** no right child *****/
  
  else if (!node->right) {
    node->left->parent = parent;
    *pnext = node->left;
  }
  
  /***** no left child *****/
  
  else if (!node->left) {
    node->right->parent = parent;
    *pnext = node->right;
  }
  
  /***** right child has no left child *****/
  
  else if (!node->right->left) {
    node->right->parent = parent;
    *pnext = node->right;
  }
    
  /***** right child has a left child *****/
  
  else {
    
    /***** loop till we find the first node with no left child *****/
    
    for (next = node->right->left ; next->left ; next = next->left);
    
    /***** replace that node with its right child *****/
    
    next->parent->left = next->right;
    if (next->right)
      next->right->parent = next->parent;
    
    /***** replace the node were deleteing with that node *****/
    
    *pnext = next;
    next->right = node->right;
    next->left = node->left;
  }
  
  free(node);
  tree->length--;
  
  return result;
}

/*******************************************************************************
	function to count the nodes in a binary search tree
	
	Arguments:
				tree	the tree
	
	returns:
				the number of nodes in the tree

*******************************************************************************/

size_t BSTree_length (
	BSTree * tree)
{
  size_t result = tree->length;
  
  return result;
}

/*******************************************************************************
  function to traverse a binary search tree inorder
  
  Arguments:
				tree 		  the binary tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *BSTree_inorder (
  BSTree *tree,
  int converse,
  BSTree_traverse_function func,
  void *extra)
{
  BSTree_node *node = tree->root;
  BSTree_node *prev = NULL;
  void *result = NULL;
  
  if (node) {
    
    /***** find the smallest node *****/

    while (LEFT(node, converse))
      node = LEFT(node, converse);
    
    /***** loop while we have nodes *****/
    
    while (node && !result) {
      
      /***** call the proccessing function *****/
      
      result = func(tree, node, node->data, extra);
      
      /***** does node have a right child? *****/
      
      if (RIGHT(node, converse)) {
        node = RIGHT(node, converse);
        
        /***** find the smallest node in this branch *****/
        
        while (LEFT(node, converse))
          node = LEFT(node, converse);
      }
      else {
        
        /***** move up the branch until we are *****/
        /***** the parent of the left child    *****/
        
        do {
          prev = node;
          node = node->parent;
        } while (node && LEFT(node, converse) != prev);
      }
      
      
    }
  }
  
  return result;
}

/*******************************************************************************
  function to traverse a binary search tree preorder
  
  Arguments:
				tree 		  the binary tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *BSTree_preorder (
  BSTree *tree,
  int converse,
  BSTree_traverse_function func,
  void *extra)
{
  BSTree_node *node = tree->root;
  BSTree_node *prev = NULL;
  void *result = NULL;
  
  while (node && !result) {
      
    /***** call the proccessing function *****/
      
    result = func(tree, node, node->data, extra);
    
    /***** does the node have a left child? *****/
    
    if (LEFT(node, converse))
      node = LEFT(node, converse);
    
    /***** does the node have a right child? *****/
    
    else if (RIGHT(node, converse))
      node = RIGHT(node, converse);
    
    else {
      
      /***** move up the branch until the node has a right  *****/
      /***** child that we did not previously come from     *****/
      
      do {
        prev = node;
        node = node->parent;
      } while (node && (!RIGHT(node, converse) ||
                        RIGHT(node, converse) == prev));
      
      /***** go right ****/
      
      if (node)
        node = RIGHT(node, converse);
    }
  }
  
  return result;
}

/*******************************************************************************
  function to traverse a binary search tree postorder
  
  Arguments:
				tree 		  the binary tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *BSTree_postorder (
  BSTree *tree,
  int converse,
  BSTree_traverse_function func,
  void *extra)
{
  BSTree_node *node = tree->root;
  BSTree_node *prev = NULL;
  void *result = NULL;
  
  /***** find the smallest node *****/
  
  while (node && LEFT(node, converse))
    node = LEFT(node, converse);
  
  /***** loop while we have nodes *****/
  
  while (node && !result) {
    
    /***** branch right *****/
    
    if (RIGHT(node, converse) && RIGHT(node, converse) != prev) {
      node = RIGHT(node, converse);
      
      /***** find the smallest node *****/
      
      if (LEFT(node, converse)) {
        while (LEFT(node, converse))
          node = LEFT(node, converse);
        continue;
      }
      
      /***** if theres a right node repeat *****/
      
      else if (RIGHT(node, converse))
        continue;
    }
        
    /***** move up the branch *****/
    
    prev = node;
    node = node->parent;
    
    /***** call the proccessing function *****/
    
    result = func(tree, prev, prev->data, extra);
  }
  
  return result;
}

/*******************************************************************************
  function to traverse a binary search tree levelorder
  
    Arguments:
				tree 		  the binary tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *BSTree_levelorder (
  BSTree *tree,
  int converse,
  BSTree_traverse_function func,
  void *extra)
{
  size_t l = 0;
  size_t cl = 1;
  size_t visits = 1;
  BSTree_node *node = NULL;
  BSTree_node *prev = NULL;
  void *result = NULL;
  
  for (cl = 1; visits && !result ; cl++) {
    l = 1;
    visits = 0;
    
    node = tree->root;
    
    if (node) {
    
      /***** find the smallest node *****/

      while (LEFT(node, converse) && l < cl) {
        node = LEFT(node, converse);
        l++;
      }
    
      /***** loop while we have nodes *****/
    
      while (node && !result) {
      
        /***** call the proccessing function *****/
      
        if (cl == l) {
          result = func(tree, node, node->data, extra);
          visits++;
        }
      
        /***** does node have a right child? *****/
      
        if (RIGHT(node, converse) && l < cl) {
          node = RIGHT(node, converse);
          l++;
        
        
          /***** find the smallest node in this branch *****/
        
          while (LEFT(node, converse) && l < cl) {
            node = LEFT(node, converse);
            l++;
          }
        }
        else {
        
          /***** move up the branch until we are *****/
          /***** the parent of the left child    *****/
        
          do {
            prev = node;
            node = node->parent;
            l--;
          } while (node && LEFT(node, converse) != prev);
        }
      }
    }
  }
  
  return result;
}

/*******************************************************************************
  slave function to delete all the nodes
*******************************************************************************/

static void *BSTree_delete_all_traverse (
	BSTree * tree,
	BSTree_node * node,
	void *data,
	void *extra)
{

	tree->free (node->data);
  
  if (node->parent && node->parent->left == node)
    node->parent->left = NULL;
  else if (node->parent && node->parent->right == node)
    node->parent->right = NULL;
  
  free (node);
  
	return NULL;
}

/*******************************************************************************
  function to delete all the nodes in a binary search tree

  args:
        tree        the tree to delete all the nodes in
   
  returns
        nothing
*******************************************************************************/

void BSTree_delete_all(
  BSTree *tree)
{
  BSTree_postorder(tree, 0, BSTree_delete_all_traverse, NULL);
  
  return;
}

/*******************************************************************************
  function to rotate a node in a binary search tree left
  
  args:
          tree  the tree
          x     the node to rotate
  
  returns:
          nothing
*******************************************************************************/

void BSTree_rotate_left(
  BSTree *tree,
  BSTree_node *x)
{
  BSTree_node *y = x->right;
  
  /***** make x's right, y's left *****/
  
  x->right = y->left;
  y->left->parent = x;
  y->parent = x->parent;
  
  /***** is x root? *****/
  
  if (x == tree->root)
    tree->root = y;
  
  /***** is x a left child? *****/
  
  else if (x == x->parent->left)
    x->parent->left = y;
  
  /***** x is a right child *****/
  
  else
    x->parent->right = y;
  
  y->left = x;
  x->parent = y;
  
  return;
}

/*******************************************************************************
  function to rotate a node in a binary search tree right
  
  args:
          tree  the tree
          y     the node to rotate
  
  returns:
          nothing
*******************************************************************************/

void BSTree_rotate_right(
  BSTree *tree,
  BSTree_node *y)
{
  BSTree_node *x = y->left;
  
  /***** make y's left, x's right *****/
  
  y->left = x->right;
  x->left->parent = y;
  x->parent = y->parent;
  
  /***** is y root? *****/
  
  if (y == tree->root)
    tree->root = x;
  
  /***** is y a right child? *****/
  
  else if (y == y->parent->right)
    y->parent->right = x;
  
  /***** y is a left child *****/
  
  else
    y->parent->left = x;
  
  y->right = y;
  y->parent = x;
  
  return;
}

/*******************************************************************************
  slave function to flatten a tree
*******************************************************************************/

void *BSTree_balance_traverse (
  BSTree *tree,
  BSTree_node *node,
  void *data,
  void *extra)
{
  BSTree_node *p = NULL;
  BSTree_node *r = NULL;
  
  struct {
    BSTree_node *last;
  } * le = extra;
  
  BSTree_node *last = le->last;
    
  /***** skip the first node *****/
  
  if (!last) {
  }
  
  /***** if we are the parent of the last node, skip *****/
  
  else if (last->parent == node) {
  }
  
  else {
    
    /**** last has a right node? *****/
    
    if (last->right) {
    
      /***** are we to the right of the last node? *****/
    
      if (last->right == node)
        last->right = NULL;
      
      else {
        
        node->parent->left = NULL;
        
        /***** do we have a node to our right? *****/
        
        if (node->right) {
          
          /***** find the furthest right node *****/
          
          for (r = node->right ; r->right ; r = r->right);
          r->right = last->right;
          last->right->parent = r;
        }
        
        else {
          node->right = last->right;
          node->right->parent = node;
        }
      }  
      last->right = NULL;
    }
    
    p = last->parent;
    last->parent = node;
    node->parent = p;
    node->left = last;
  }
      
  /***** new head? *****/
  
  if (last == tree->root)
    tree->root = node;
  
  le->last = node;
  
  return NULL;
}

/*******************************************************************************
  function to balance a binary search tree
  
  args:
        tree    the tree to balance
  
  returns:
        nothing
*******************************************************************************/

void BSTree_balance (
  BSTree *tree)
{
  BSTree_node *node = NULL;
  int i = 0;
  int j = 0;
  struct {
    BSTree_node *last;
  } last;
  last.last = NULL;
  
  
  /***** flatten *****/
  
  BSTree_inorder (tree, 0, &BSTree_balance_traverse, &last);
  
  /***** remake tree *****/
  
  for (j = tree->length / 2 ; j >= 1; j /= 2)  {
    node = tree->root;
    
    /**** loop thought the nodes 2 at a time *****/
    
    for (i = 0, node = node->left ; i < j && node; i++, node = node->left) {
      BSTree_rotate_right(tree, node);
      node = node->left;
    }
  }
  
  return;
}

/*******************************************************************************
  slave function to move a binary tree
*******************************************************************************/

void *BSTree_move_traverse (
  BSTree *tree,
  BSTree_node *node,
  void *data,
  void *extra)
{
  BSTree *dest = extra;
  
  BSTree_insert(dest, data);
  
  if (node->parent && node->parent->left == node)
    node->parent->left = NULL;
  else if (node->parent && node->parent->right == node)
    node->parent->right = NULL;
  
  free (node);
  
  return NULL;
}


/*******************************************************************************
  function to move a binary search tree to another binary search tree

  args:
        dest    the tree to move the nodes to
        src     the tree to move the nodes from
        branch  the node in the src tree to move

  returns:
        nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void BSTree_move (
  BSTree *dest,
  BSTree *src,
  BSTree_node *branch)
{
  BSTree_postorder(src, 0, &BSTree_move_traverse, dest);
  
  return;
}

/*******************************************************************************
  slave function to copy a binary tree
*******************************************************************************/

void * BSTree_copy_traverse (
  BSTree *tree,
  BSTree_node *node,
  void *data,
  void *extra)
{
  BSTree *dest = extra;
  void *newdata = NULL;
  
  tree->copy(&newdata, data);
  
  BSTree_insert(dest, newdata);
  
  return NULL;
}


/*******************************************************************************
  function to copy a binary search tree to another binary search tree

  args:
        dest    the tree to copy the nodes to
        src     the tree to move the nodes from
        branch  the node in the src tree to copy
        func    function to copy the data in the nodes

  returns:
        nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void BSTree_copy (
  BSTree *dest,
  BSTree *src,
  BSTree_node *branch)
{

  BSTree_postorder(src, 0, &BSTree_copy_traverse, dest);
  
  return;
}
  
