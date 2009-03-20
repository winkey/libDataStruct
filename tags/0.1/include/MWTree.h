/*
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
 
#ifndef _MWTREE_H
#define _MWTREE_H

/*****************************************************************************//**
  structure for a multi way tree node
  
 @param	parent  	the parent node
 @param	children	linked list of children
 @param	data    	the data the node holds
*******************************************************************************/

typedef struct MWTree_node_tab {
	struct MWTree_node_tab *parent;
	DLList children;
	void *data;
} MWTree_node;

/*****************************************************************************//**
  type of function to pass to compare data
  
	note
        if data is a char pointer strcmp() can be passed

 @param	data1   the first data pointer to be compared
 @param	data2   the second data pointer to be compared
  
 @return	less than 0 if data1 is less than data2
0 if data1 is equal to data2
greater than 0 if data1 is greater than data2
        
*******************************************************************************/

typedef int (*MWTree_data_cmp_func) (
  void *data1,
  void *data2);

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/

typedef void (*MWTree_data_free_func) (void *data);

/*****************************************************************************//**
  type of function to pass to the copy functions
  
 @param	dest  pointer to the new data pointer
 @param	src   the data pointer in the node to be copyed
  
 @return	the new new copyed data
NULL on error

  note:
        that unfortunatly strcpy() CAN NOT be used here. you need to allocate
        memory and then use strcpy()
*******************************************************************************/
  
typedef void *(*MWTree_data_copy_func) (void **dest, void *src);

/*****************************************************************************//**
  structure for a multi way tree
  
 @param	length  the number of nodes in the tree
 @param	root    the root node of the tree
 @param	cmp     function to compare the data in the nodes
 @param	free    function to free the data contained in the nodes
 @param	copy    function to copy the data contained in the nodes
*******************************************************************************/

typedef struct {
  size_t length;
  MWTree_node *root;
  MWTree_data_cmp_func cmp;
  MWTree_data_free_func free;
  MWTree_data_copy_func copy;
} MWTree;

/*******************************************************************************
  type of function to be passed to the traverse function
  
 @param	tree  the tree being parsed
 @param	node  the current node
 @param	data  the data the current node holds
 @param	extra the extra pointer passed to the traverse function

 @return	null to continue the traverse loop
          non null that stops the traverse loop and is returned by the traverse
          function
*******************************************************************************/

typedef void *(*MWTree_traverse_function) (
	MWTree *tree,
	MWTree_node *node,
	void *data,
	void *extra);

/*****************************************************************************//**
  function to find a node in a multi way tree
  
 @param	tree  the tree to find the node in
 @param	data  the data to look for
  
 @return	the node that holds the data
        null if the data is not found
  
  notes:
        the data passed to this function to look for is only bound by the data
        compare function

*******************************************************************************/

MWTree_node *MWTree_find(
  MWTree *tree,
  void *data);

/*****************************************************************************//**
  function to add a node to a binary search tree
  
 @param	tree  the tree to add the node to
 @param	data  the data the node is to hold
  
 @return	NULL if malloc fails
        the new node
*******************************************************************************/

MWTree_node *MWTree_insert (
  MWTree *tree,
  void *data);

/*****************************************************************************//**
  funtion to delete a node from a multi way tree
  
 @param	tree  the tree to delete the node from
 @param	node  the node to delete
  
 @return	the data the node held
*******************************************************************************/

void *MWTree_delete (
  MWTree *tree,
  MWTree_node *node);

/*****************************************************************************//**
	function to count the nodes in a multi way tree
	
 @param	tree	the tree
	
 @return	the number of nodes in the tree

*******************************************************************************/

size_t MWTree_length (
	MWTree * tree);

/*****************************************************************************//**
  function to traverse a multi way tree inorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_inorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a multi way tree preorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_preorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a multi way tree postorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_postorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to traverse a multi way tree levelorder
  
 @param	tree 		  the tree to traverse
 @param	converse  if true swap left and right
 @param	func  	  the function to pass each node to for processing
 @param	extra		  extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_levelorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra);

/*****************************************************************************//**
  function to delete all the nodes in a multi way tree

 @param	tree        the tree to delete all the nodes in
          
 @return	nothing
*******************************************************************************/

void MWTree_delete_all(
  MWTree *tree);

/*****************************************************************************//**
  function to move a multi way tree to another multi way tree

 @param	dest    the tree to move the nodes to
 @param	src     the tree to move the nodes from
 @param	branch  the node in the src tree to move

 @return	nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void MWTree_move (
  MWTree *dest,
  MWTree *src,
  MWTree_node *branch);

/*****************************************************************************//**
  function to copy a multi way tree to another multi way tree

 @param	dest    the tree to copy the nodes to
 @param	src     the tree to move the nodes from
 @param	branch  the node in the src tree to copy

 @return	nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void MWTree_copy (
  MWTree *dest,
  MWTree *src,
  MWTree_node *branch);


#endif /* _MWTREE_H */

 
