/*******************************************************************************
	stack.h
	
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

typedef struct stackLList_tab {
	struct stackLList_tab *next;
	void *data;
} stackLList_node;

typedef struct {
	size_t length;
	stackLList_node *head;
	stackLList_node *tail;
} stackLList;

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

typedef void *(*stackLList_iterate_func) (
	stackLList *list,
	stackLList_node *node,
  void *data,
  void *extra);

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/


typedef void (*stackLList_data_free_func) (void *data);

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
  
typedef void *(*stackLList_data_copy_func) (void **dest, void *src);

/*****************************************************************************//**
	function to push a node onto a stack
	
 @param	list	the linked list
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error
	
*******************************************************************************/

stackLList_node *stackLList_push (
	stackLList * list,
	void *data);

/*****************************************************************************//**
	function to pop a node from a stack and delete the node
	
 @param	list	the linked list
	
 @return	the data the node held

*******************************************************************************/

void *stackLList_pop (
	stackLList * list);

/*****************************************************************************//**
	function to read the head of a stack without delteing it
  
 @param	list	the linked list
	
 @return	the data the node holds

*******************************************************************************/

void *stackLList_read_head (
	stackLList * list);

/*****************************************************************************//**
	function to count the nodes in a stack

 @param	list	the linked list
	
 @return	the number of nodes in the linked list

*******************************************************************************/

size_t stackLList_length (
	stackLList * list);

/*****************************************************************************//**
	function to iterate a stack
  
 @param	list 		the linked list
 @param	function	the function to pass each node to for processing
 @param	extra		extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			iteration
      NULL if the end of the linked list was reached
  
*******************************************************************************/

void *stackLList_iterate (
	stackLList * list,
	stackLList_iterate_func function,
	void *extra);

/*****************************************************************************//**
	function to delete all the nodes in a stack
  
 @param	list		the linked list
 @param	function	the function to call to free the data

 @return	nothing

*******************************************************************************/

void stackLList_delete_all (
	stackLList * list,
	stackLList_data_free_func function);

/*****************************************************************************//**
	function to move another stack to the head of a stack
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
	
 @return	nothing

*******************************************************************************/

void stackLList_push_list (
	stackLList * dest,
	stackLList * src);

/*****************************************************************************//**
	function to copy another list to the head of a stack
	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	func  	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, SLList , QLList, or DQLList to stackLList
				for the src argument
				
*******************************************************************************/

stackLList_node *stackLList_push_list_copy (
	stackLList * dest,
	stackLList * src,
	stackLList_data_copy_func func);
