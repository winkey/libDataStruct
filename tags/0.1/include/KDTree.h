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
 
#ifndef _KDTREE_H
#define _KDTREE_H

/*****************************************************************************//**
  structure for a kd tree node
  
 @param	point		the point
 @param	parent  the parent node
 @param	left    the left child node
 @param	right   the right child node
 @param	data    the data the node holds
*******************************************************************************/

typedef struct KDTree_node_tab {
	double *point;
  struct KDTree_node_tab *parent;
  struct KDTree_node_tab *left;
  struct KDTree_node_tab *right;
  void *data;
} KDTree_node;

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/

typedef void (*KDTree_data_free_func) (void *data);

/*****************************************************************************//**
  type of function to copy the data
  
 @param	dest  pointer to the new data pointer
 @param	src   the data pointer in the node to be copyed
  
 @return	the new new copyed data
        NULL on error

  note:
        that unfortunatly strcpy() CAN NOT be used here. you need to allocate
        memory and then use strcpy()
*******************************************************************************/
  
typedef void *(*KDTree_data_copy_func) (void **dest, void *src);

/*****************************************************************************//**
  structure for a binary search tree
  
 @param	length  the number of nodes in the tree
 @param	root    the root node of the tree
 @param	dims		the number of dimensions in the dataset
 @param	cmp     function to compare the data in the nodes
 @param	free    function to free the data contained in the nodes
 @param	copy    function to copy the data contained in the nodes
*******************************************************************************/

typedef struct {
  size_t length;
  KDTree_node *root;
	int dims;
  KDTree_data_free_func free;
  KDTree_data_copy_func copy;
} KDTree;

/*****************************************************************************//**
  type of function to be passed to the traverse function
  
 @param	tree  the tree being parsed
 @param	node  the current node
 @param	point the point
 @param	data  the data the current node holds
 @param	extra the extra pointer passed to the traverse function

 @return	null to continue the traverse loop
          non null that stops the traverse loop and is returned by the traverse
          function
*******************************************************************************/

typedef void *(*KDTree_traverse_function) (
	KDTree *tree,
	KDTree_node *node,
	double *point,
	void *data,
	void *extra);


/*****************************************************************************//**
  function to add a node to a kd tree
  
 @param	tree  the tree to add the node to
 @param	point	the point (spacial)
 @param	data  the data the node is to hold
   
 @return	NULL if malloc fails
        the new node
*******************************************************************************/

KDTree_node *KDTree_insert (
  KDTree *tree,
	double *point,
  void *data);

/*****************************************************************************//**
  funtion to delete a node from a kd tree
  
 @param	tree  the tree to delete the node from
 @param	node  the node to delete
  
 @return	the data the node held
*******************************************************************************/

void *KDTree_delete (
  KDTree *tree,
  KDTree_node *node);

/*****************************************************************************//**
	function to count the nodes in a kd tree
	
 @param	tree	the tree
	
	returns:
				the number of nodes in the tree

*******************************************************************************/

size_t KDTree_length (
	KDTree * tree);

/*****************************************************************************//**
  function to traverse a kd tree inorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_inorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a binary search tree preorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_preorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a kd tree postorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_postorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a kd tree levelorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *KDTree_levelorder (
  KDTree *tree,
  int converse,
  KDTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to delete all the nodes in a kd tree

 @param	tree        the tree to delete all the nodes in
  
 @return	nothing
*******************************************************************************/

void KDTree_delete_all(
  KDTree *tree);

/*****************************************************************************//**
	function to find the nearest points to a point in a kd tree

 @param	tree      the tree to delete all the nodes in
 @param	point			the point to look near
 @param	range			the distance to look away from the point
 @param	func  	  the function to pass each found node to for processing
 @param	extra		  extra data to pass to/from the proccessing function

 @return	the number of nodes found

*******************************************************************************/

int KDTree_find_nearest(
  KDTree *tree,
	double *point,
	double range,
	KDTree_traverse_function func,
  void *extra);

#endif /* _KDTREE_H */

 
