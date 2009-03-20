/*******************************************************************************
	CLList.h
	
	part of libLL
  
  Copyright (C) 2005-2007  winkey
  
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
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*******************************************************************************/

#ifndef CLList_h
#define CLList_h

typedef struct CLList_node_tab {
	struct CLList_node_tab *prev;
	struct CLList_node_tab *next;
	void *data;
} CLList_node;

typedef struct {
	size_t length;
	CLList_node *head;
	CLList_node *tail;
} CLList;

/*****************************************************************************//**
  type of function to be passed to the iterate function
  
 @param	the list being parsed
 @param	node  the current node
 @param	data  the data the current node holds
 @param	extra the extra pointer passed to the itterate function

 @return	null to continue the itterate loop
          non null that stops the itterate loop and is returned by the iterate
          function
*******************************************************************************/

typedef void *(*CLList_iterate_func) (
	CLList *list,
	CLList_node *node,
	void *data,
	void *extra);

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/


typedef void (*CLList_data_free_func) (
  void *data);

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

typedef void *(*CLList_data_copy_func) (
  void **dest,
  void *src);

/*****************************************************************************//**
	function to add a node to the tail of a circular linked list
	
 @param	list	the linked list
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error

  note:
        the new node becomes the tail

*******************************************************************************/

CLList_node *CLList_append (
	CLList * list,
	void *data);

/*****************************************************************************//**
	function to add a node after any node in a circular linked list
	
 @param	list	the linked list
 @param	node	the node you wish to place the new node after
 @param	data	the data you wish to store in the linked list
		
 @return	the new node
				NULL on error

  note:
        if node is null or the list is empty the new node will be appended to
        the list
        the new node becomes the tail

*******************************************************************************/

CLList_node *CLList_insert_after (
	CLList * list,
	CLList_node * node,
	void *data);

/*****************************************************************************//**
	function to add a node before any node in a circular linked list
	
 @param	list	the linked list
 @param	node	the node you wish to place the new node before
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error

note:
        if node is null or the list is empty the new node will be appended to
        the list
        the new node becomes the tail
				
*******************************************************************************/

CLList_node *CLList_insert_before (
	CLList *list,
	CLList_node * node,
	void *data);

/*****************************************************************************//**
	function to delete a node from a circular linked list
	
 @param	list	the linked list
 @param	node	the node you wish to delete
	
 @return	the data the node held
  
  note:
        the head becomes the next node

*******************************************************************************/

void *CLList_delete (
	CLList *list,
	CLList_node * node);

/*****************************************************************************//**
	function to delete the node after a node in a circular linked list
	
 @param	list	the linked list
 @param	node	the node before the node you wish to delete
	
 @return	the data the node held

  note:
        the node becomes the tail

*******************************************************************************/

void *CLList_delete_after (
	CLList *list,
	CLList_node * node);

/*****************************************************************************//**
	function to delete the node before a node in a circular linked list
	
 @param	list	the linked list
 @param	node	the node after the node you wish to delete
	
 @return	the data the node held

  note:
        the node becomes the head

*******************************************************************************/

void *CLList_delete_before (
	CLList *list,
	CLList_node * node);

/*****************************************************************************//**
	function to count the nodes in a circular linked list
	
 @param	list	the linked list
	
 @return	the number of nodes in the linked list

*******************************************************************************/

size_t CLList_length (
	CLList * list);

/*****************************************************************************//**
	function to iterate a circular linked list
	
 @param	list 		the linked list
 @param	function	the function to pass each node to for processing
 @param	extra		extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			iteration
			NULL if the end of the linked list was reached

  note:
      if the proccessing function does not return NULL the tail becomes the
      current node
      
*******************************************************************************/

void *CLList_iterate (
	CLList *list,
	CLList_iterate_func function,
	void *extra);

/*****************************************************************************//**
	function to delete all the nodes in a circular linked list
	
 @param	list		the linked list
 @param	function	the function to call to free the data

 @return	nothing

*******************************************************************************/

void CLList_delete_all (
	CLList * list,
	CLList_data_free_func function);

/*****************************************************************************//**
  function to rotate the Circular linked list
  
 @param	list      the linked list
 @param	nodes     the number of nodes to rotate the linked list
 @param	reverse   non zero value to reverse the rotation
        
 @return	nothing

  note:
        if the number of nodes is greater than or equal to the number of nodes
        in the list head, and tail will be simply swapped
*******************************************************************************/

void CLList_rotate (
  CLList *list,
  size_t nodes,
  int reverse);

/*****************************************************************************//**
	function to move another double linked list to the tail of a circular linked
	list
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
	
 @return	nothing

  notes:
        you can cast a QLList, or DQLList or DLList to CLList for the src 
        argument
        the head becomes the head of dest if dest is not empty, otherwise head
        becomes the head of src
*******************************************************************************/

void CLList_append_list (
	CLList * dest,
	CLList * src);

/*****************************************************************************//**
	function to move another double linked list to the middle of a circular linked
	list, after a particular node
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
 @param	node	the node you wish to insert src after

 @return	nothing
	
	notes:
        you can cast a QLList, or DQLList or DLList to CLList for the src 
        argument
        if node is null src is appended to dest
        the head becomes the node after node if node is not null, otherwise
        the head becomes the head of dest if dest is not empty, otherwise head
        becomes the head of src

*******************************************************************************/

void CLList_insert_list_after (
	CLList * dest,
	CLList * src,
	CLList_node * node);

/*****************************************************************************//**
	function to move another double linked list to the middle of a circular linked
	list, before a particular node
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
 @param	node	the node you wish to insert src before

 @return	nothing
	
	notes:
				you can cast a QLList, or DQLList or DLList to CLList for the src 
        argument
        if node is null src is appended to dest
        the head becomes node if node is not null, otherwise the head becomes
        the head of dest if dest is not empty, otherwise head becomes the head
        of src


*******************************************************************************/

void CLList_insert_list_before (
	CLList * dest,
	CLList * src,
	CLList_node * node);

/*****************************************************************************//**
	function to copy another list to the tail of a circular linked list
	
	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a SLList, stackLList , QLList, DQLList, or DLList to CLList
				for the src argument
        the head becomes the head of dest if dest is not empty, otherwise head
        becomes the head of src
        
*******************************************************************************/

CLList_node *CLList_append_list_copy (
	CLList * dest,
	CLList * src,
	CLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to copy another list to the middle of a circular linked list, after
	a particular node

	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	node		the dest node you wish to copy the data after
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a SLList, stackLList , QLList, DQLList, or DLList to CLList
				for the src argument
				the head becomes the node after node if node is not null, otherwise
        the head becomes the head of dest if dest is not empty, otherwise head
        becomes the head of src

*******************************************************************************/

CLList_node *CLList_insert_list_after_copy (
	CLList * dest,
	CLList * src,
	CLList_node * node,
	CLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to copy another list to the middle of a circular linked list, before
	a particular node

	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	node		the dest node you wish to copy the data before
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a SLList, stackLList , QLList, DQLList, or DLList to CLList
				for the src argument
        the head becomes node if node is not null, otherwise the head becomes
        the head of dest if dest is not empty, otherwise head becomes the head
        of src

*******************************************************************************/

CLList_node *CLList_insert_list_before_copy (
	CLList * dest,
	CLList * src,
	CLList_node * node,
	CLList_data_copy_func copy_func);


#endif
