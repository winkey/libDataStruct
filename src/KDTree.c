/*
 * libKDTree
 * Copyright (C) Brian Case 2008 <rush@gisweather.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <stdlib.h>
#include <string.h>
#include "../include/KDTree.h"

#define LEFT(node, converse) ((converse) ? (node)->right : (node)->left )
#define RIGHT(node, converse) ((converse) ? (node)->left  : (node)->right)

double *pointcopy(
	double **dest,
	double *src,
	int dims)
{
	if ((*dest = malloc(dims * sizeof (double))))
		memcpy(*dest, src, dims * sizeof (double));
	
	return *dest;
}

/*******************************************************************************
  function to add a node to a kd tree
  
  args:
        tree  the tree to add the node to
				point	the point (spacial)
        data  the data the node is to hold
   
  returns:
        NULL if malloc fails
        the new node
*******************************************************************************/

KDTree_node *KDTree_insert (
  KDTree *tree,
	double *point,
  void *data)
{
  KDTree_node *node = NULL;
  KDTree_node **next = NULL;
  KDTree_node *new = NULL;
  KDTree_node *parent = NULL;
  int axis = 0;
	
  /***** alocate memory for the node *****/
  
  if (!(new = malloc(sizeof(KDTree_node)))) {
  }
	
	/***** copy the point *****/
	
	else if (!pointcopy(&(new->point), point, tree->dims)) {
		free(new);
		new = NULL;
	}
	
  else {
    new->data = data;
    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    next = &tree->root;
      
    /***** loop til we find where we go in the tree *****/
      
    for (node = *next, axis = 0;
				 node ;
				 node = *next, axis = (axis + 1) % tree->dims) {
      
      /***** left or right? *****/
			
      if (point[axis] < node->point[axis])
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
  funtion to delete a node from a kd tree
  
  args:
        tree  the tree to delete the node from
        node  the node to delete
  
  returns:
        the data the node held
*******************************************************************************/

void *KDTree_delete (
  KDTree *tree,
  KDTree_node *node)
{
  void *result = node->data;
  KDTree_node *parent = node->parent;
  KDTree_node **pnext = NULL;
  KDTree_node *next = NULL;
  
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
  
	free(node->point);
  free(node);
  tree->length--;
  
  return result;
}

/*******************************************************************************
	function to count the nodes in a kd tree
	
	Arguments:
				tree	the tree
	
	returns:
				the number of nodes in the tree

*******************************************************************************/

size_t KDTree_length (
	KDTree * tree)
{
  size_t result = tree->length;
  
  return result;
}

/*******************************************************************************
  function to traverse a kd tree inorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_inorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra)
{
  KDTree_node *node = tree->root;
  KDTree_node *prev = NULL;
  void *result = NULL;
  
  if (node) {
    
    /***** find the smallest node *****/

    while (LEFT(node, converse))
      node = LEFT(node, converse);
    
    /***** loop while we have nodes *****/
    
    while (node && !result) {
      
      /***** call the proccessing function *****/
      
      result = func(tree, node, node->point, node->data, extra);
      
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
  function to traverse a kd tree preorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_preorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra)
{
  KDTree_node *node = tree->root;
  KDTree_node *prev = NULL;
  void *result = NULL;
  
  while (node && !result) {
      
    /***** call the proccessing function *****/
      
    result = func(tree, node, node->point, node->data, extra);
    
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
  function to traverse a kd tree postorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_postorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra)
{
  KDTree_node *node = tree->root;
  KDTree_node *prev = NULL;
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
    
    result = func(tree, prev, prev->point, prev->data, extra);
  }
  
  return result;
}

/*******************************************************************************
  function to traverse a kd tree levelorder
  
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

void *KDTree_levelorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra)
{
  size_t l = 0;
  size_t cl = 1;
  size_t visits = 1;
  KDTree_node *node = NULL;
  KDTree_node *prev = NULL;
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
          result = func(tree, node, node->point, node->data, extra);
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

static void *KDTree_delete_all_traverse (
	KDTree * tree,
	KDTree_node * node,
	double *point,
	void *data,
	void *extra)
{

	free(node->point);
  tree->free (node->data);
	
  if (node->parent && node->parent->left == node)
    node->parent->left = NULL;
  else if (node->parent && node->parent->right == node)
    node->parent->right = NULL;
  
  free (node);
  
	return NULL;
}

/*******************************************************************************
  function to delete all the nodes in a kd tree

  args:
        tree        the tree to delete all the nodes in
   
  returns
        nothing
*******************************************************************************/

void KDTree_delete_all(
  KDTree *tree)
{
  KDTree_postorder(tree, 0, KDTree_delete_all_traverse, NULL);
  
  return;
}

/*******************************************************************************
	function to find the nearest points to a point in a kd tree

	args:
        tree      the tree to delete all the nodes in
				point			the point to look near
				range			the distance to look away from the point
				func  	  the function to pass each found node to for processing
				extra		  extra data to pass to/from the proccessing function

	returns:
				the number of nodes found

*******************************************************************************/

int KDTree_find_nearest(
  KDTree *tree,
	double *point,
	double range,
	KDTree_traverse_function func,
  void *extra)
{
	KDTree_node *node = tree->root;
	int found = 0;
	int axis;
	double sq;
	int i;
	double range_sq;
		
	range_sq = range * range;
	
  for (axis = 0; node ; axis = (axis + 1) % tree->dims) {
		
		for (i = 0, sq = 0; i < tree->dims ; i++)
			sq += (node->point[i] - point[i]) * (node->point[i] - point[i]);
		
		if (sq <= range_sq) {
			func(tree, node, node->point, node->data, extra);
			found++;
			
			if (point[axis] <= node->point[axis])
				node = node->left;
			else
				node = node->right;
		}
		else {
			if (point[axis] <= node->point[axis])
				node = node->right;
			else
				node = node->left;
		}
			
	}

	return found;
}

