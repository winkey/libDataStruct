/*******************************************************************************
	QLList.c
	
  fifo
  
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

#ifndef QLList_h
#define QLList_h

typedef struct QLList_node_tab {
	struct QLList_node_tab *prev;
	struct QLList_node_tab *next;
	void *data;
} QLList_node;

typedef struct {
	size_t length;
	QLList_node *head;
	QLList_node *tail;
} QLList;

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

typedef void *(*QLList_iterate_func) (
	QLList *list,
	QLList_node *node,
	void *data,
	void *extra);

/*****************************************************************************//**
  type of a function to pass to the delete functions to free the data
  
 @param	data  pointer to the data to be free'ed
  
 @return	nothing

  note:
        free() can be used if the data is a simple type
*******************************************************************************/


typedef void (*QLList_data_free_func) (
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

typedef void *(*QLList_data_copy_func) (
  void **dest,
  void *src);



/*****************************************************************************//**
	function to push a node onto a que
	
 @param	list	the linked list
 @param	data	the data you wish to store in the linked list
	
 @return	the new node
				NULL on error

*******************************************************************************/

void *QLList_push (
	QLList *list,
	void *data);

/*****************************************************************************//**
	function to pull a node from a que and delete the node
	
 @param	list	the linked list
	
 @return	the data the node held

*******************************************************************************/

void *QLList_pull (
	QLList * list);

/*****************************************************************************//**
	function to read the data in the tail of the que without delteing it
	
 @param	list	the linked list
	
 @return	the data the node holds

*******************************************************************************/

void *QLList_read (
	QLList * list);

/*****************************************************************************//**
	function to count the nodes in a que

 @param	list	the linked list
	
 @return	the number of nodes in the linked list

*******************************************************************************/

size_t QLList_length (
	QLList * list);

/*****************************************************************************//**
	function to iterate a linked list
  
 @param	list 		the linked list
 @param	function	the function to pass each node to for processing
 @param	extra		extra data to pass to/from the proccessing function
	
 @return	the non null returned from the proccessing function that stops the
			iteration
      NULL if the end of the linked list was reached

*******************************************************************************/

void *QLList_iterate (
	QLList * list,
	QLList_iterate_func function,
	void *extra);

/*****************************************************************************//**
	function to delete all the nodes in a linked list
  
 @param	list		the linked list
 @param	function	the function to call to free the data

 @return	nothing

*******************************************************************************/

void QLList_delete_all (
	QLList * list,
	QLList_data_free_func function);

/*****************************************************************************//**
	function to move another que to the head of a que
	
 @param	dest	the linked list you want to move the src to
 @param	src		the linked list you want to move to the dest
	
 @return	nothing

*******************************************************************************/

void QLList_push_list (
	QLList * dest,
	QLList * src);

/*****************************************************************************//**
	function to copy another list to the head of a que
	
 @param	dest		the linked list you want to copy the src to
 @param	src			the linked list you want to copy to the dest
 @param	func  	function to copy the data held in each node
	
 @return	null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, SLList , stackLList, or DQLList to QLList
				for the src argument
				
*******************************************************************************/

QLList_node *QLList_push_list_copy (
	QLList * dest,
	QLList * src,
	QLList_data_copy_func func);

#endif
