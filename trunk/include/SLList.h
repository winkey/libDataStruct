/*******************************************************************************
	SLList.h
	
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

#ifndef SLList_h
#define SLList_h

/*****************************************************************************//**
  structure for a single linked list node
  
 @param	next  the next node in the list
 @param	data  the data the node holds
*******************************************************************************/

typedef struct SLList_tab {
	struct SLList_tab *next;
	void *data;
} SLList_node;

/*****************************************************************************//**
  structure for a single linked list
  
 @param	length  the number of nodes in the list
 @param	head    the first node in the list
 @param	tail    the last node in the list
*******************************************************************************/

typedef struct {
	size_t length;
	SLList_node *head;
	SLList_node *tail;
} SLList;

/*****************************************************************************//**
  type of function to be passed to the iterate function
  
 @param	list  the list being parsed
 @param	node  the current node
 @param	data  the data the current node holds
 @param	extra the extra pointer passed to the itterate function

 @return	null to continue the itterate loop
          non null that stops the itterate loop and is returned by the iterate
          function
*******************************************************************************/

typedef void *(*SLList_iterate_func) (
	SLList *list,
	SLList_node *node,
  void *data,
  void *extra);

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	 nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/


typedef void (*SLList_data_free_func) (void *data);

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
  
typedef void *(*SLList_data_copy_func) (void **dest, void *src);

/*****************************************************************************//**
  type of function to pass to the mergesort to compare the data
  
 @param	data1   the first data pointer to be compared
 @param	data2   the second data pointer to be compared
  
 @return	less than 0 if data1 is less than data2
        0 if data1 is equal to data2
        greater than 0 if data1 is greater than data2
        
   note
        if data is a char pointer strcmp() can be passed
*******************************************************************************/

typedef int (*SLList_data_cmp_func) (void *data1, void *data2);

/*****************************************************************************//**
	function to add a node to the head of a single linked list
	
 @param	list	the linked list
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error
				
*******************************************************************************/

SLList_node *SLList_prepend (
	SLList * list,
	void *data);

/*****************************************************************************//**
	function to add a node to the tail of a single linked list
	
 @param	list	the linked list
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error
				
*******************************************************************************/

SLList_node *SLList_append (
	SLList * list,
	void *data);

/*****************************************************************************//**
	function to add a node after any node in a single linked list
	
 @param	list	the linked list
 @param	node	the node you wish to place the new node after
 @param	data	the data you wish to store in the linked list
		
 @return	the new node
				NULL on error
				
  note:
        if node is null or the list is empty the new node will be appended to
        the list

*******************************************************************************/

SLList_node *SLList_insert_after (
	SLList * list,
	SLList_node * node,
	void *data);

/*****************************************************************************//**
	function to add a node before any node in a single linked list
	
 @param	list	the linked list
 @param	node	the node you wish to place the new node before
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error
  
  note:
        if node is null or the list is empty the new node will be prepended to
        the list

*******************************************************************************/

SLList_node *SLList_insert_before (
	SLList * list,
	SLList_node * node,
	void *data);

/*****************************************************************************//**
	function to delete a node from a single linked list
	
 @param	list	the linked list
 @param	node	the node you wish to delete
	
 @return	the data the node held
				
*******************************************************************************/

void *SLList_delete (
	SLList * list,
	SLList_node * node);

/*****************************************************************************//**
	function to delete the node after a node in a single linked list
	
 @param	list	the linked list
 @param	node	the node before the node you wish to delete
	
 @return	the data the node held
				
*******************************************************************************/

void *SLList_delete_after (
	SLList * list,
	SLList_node * node);

/*****************************************************************************//**
	function to delete the node before a node in a single linked list
	
 @param	list	the linked list
 @param	node	the node after the node you wish to delete
	
 @return	the data the node held
				
*******************************************************************************/

void *SLList_delete_before (
	SLList * list,
	SLList_node * node);

/*****************************************************************************//**
	function to count the nodes in a single linked list
	
 @param	list	the linked list
	
 @return	the number of nodes in the linked list

*******************************************************************************/

size_t SLList_length (
	SLList * list);

/*****************************************************************************//**
	function to iterate a single linked list
	
 @param	list 		the linked list
 @param	function	the function to pass each node to for processing
 @param	extra		extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			iteration
			NULL if the end of the linked list was reached

*******************************************************************************/

void *SLList_iterate (
	SLList * list,
	SLList_iterate_func function,
	void *extra);

/*****************************************************************************//**
	function to delete all the nodes in a single linked list
	
 @param	list		the linked list
 @param	function	the function to call to free the data

 @return	nothing

*******************************************************************************/

void SLList_delete_all (
	SLList * list,
	SLList_data_free_func function);

/*****************************************************************************//**
	function to move another single linked list to the head of a single linked
	list
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
	
 @return	nothing

*******************************************************************************/

void SLList_prepend_list (
	SLList * dest,
	SLList * src);

/*****************************************************************************//**
	function to move another list to the tail of a single linked list
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
	
 @return	nothing

*******************************************************************************/

void SLList_append_list (
	SLList * dest,
	SLList * src);

/*****************************************************************************//**
	function to move another single linked list to the middle of a single linked
	list, after a particular node
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
 @param	node	the node you wish to insert src after
 
 @return	nothing
	
	if node is null src is appended to dest

*******************************************************************************/

void SLList_insert_list_after (
	SLList * dest,
	SLList * src,
	SLList_node * node);

/*****************************************************************************//**
	function to move another single linked list to the middle of a single linked
	list, before a particular node
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
 @param	node	the node you wish to insert src before

 @return	nothing
	
	notes:
				if node is null src is appended to dest

*******************************************************************************/

void SLList_insert_list_before (
	SLList * dest,
	SLList * src,
	SLList_node * node);

/*****************************************************************************//**
	function to copy another list to the head of a single linked list
	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_prepend_list_copy (
	SLList * dest,
	SLList * src,
	SLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to copy another list to the tail of a single linked list
	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_append_list_copy (
	SLList * dest,
	SLList * src,
	SLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to copy another list to the middle of a single linked list, after
	a particular node

 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	node		the dest node you wish to copy the data after
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_insert_list_after_copy (
	SLList * dest,
	SLList * src,
	SLList_node * node,
	SLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to copy another list to the middle of a single linked list, before
	a particular node

 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	node		the dest node you wish to copy the data before
 @param	copy_func	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_insert_list_before_copy (
	SLList * dest,
	SLList * src,
	SLList_node * node,
	SLList_data_copy_func copy_func);

/*****************************************************************************//**
	function to sort a single linked list
	
 @param	list		the linked list to sort
 @param	cmp_func	function to compare the data in 2 nodes
	
 @return	nothing

	Notes:


*******************************************************************************/

void SLList_sort (
	SLList * list,
	SLList_data_cmp_func cmp_func);

#endif
