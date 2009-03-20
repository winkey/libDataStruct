/*******************************************************************************
	DQLList.c
	
  fifo or filo
  
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


/***** our headers *****/

#include <stddef.h>
#include <stdlib.h>
#include "../include/DQLList.h"
#include "../include/DLList.h"



/*******************************************************************************
	function to push a node onto a deque
	
		
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error

*******************************************************************************/

void *DQLList_push (
	DQLList * list,
	void *data)
{
	
	DLList_prepend(
				  (DLList *) list,
				  data);
	
	return data;
}

/*******************************************************************************
	function to pull a node from a deque and delete the node
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node held
        
*******************************************************************************/

void *DQLList_pull (
	DQLList * list)
{
	void *result = NULL;

	result = DLList_delete(
						   (DLList *) list,
						   (DLList_node *) list->tail);
	
	return result;
}

/*******************************************************************************
	function to pop a node from a que and delete the node
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node held
*******************************************************************************/

void *DQLList_pop (
	DQLList * list)
{
	void *result = NULL;

	result = DLList_delete(
						   (DLList *) list,
						   (DLList_node *) list->head);
	
	return result;
}

/*******************************************************************************
	function to read the tail from a deque without deleting it
	
	Arguments:
				list	the linked list
	
	returns:
				the data the node holds
*******************************************************************************/

void *DQLList_read_tail (
	DQLList * list)
{
	void *result = list->tail->data;

	return result;
	
}

/*******************************************************************************
	function to read the head of a deque without deleteing it
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node holds
*******************************************************************************/

void *DQLList_read_head (
	DQLList * list)
{
	void *result = list->head->data;

	return result;
}

/*******************************************************************************
	function to count the nodes in a deque

	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t DQLList_length (
	DQLList * list)
{
	size_t result = list->length;
	
	return result;
}

/*******************************************************************************
	function to iterate a linked list
  
     Arguments:
				list 		the linked list
				function	the function to pass each node to for processing
				extra		extra data to pass to/from the proccessing function
	
	return:
			the non null returned from the proccessing function that stops the
			iteration
      NULL if the end of the linked list was reached

*******************************************************************************/

void *DQLList_iterate (
	DQLList * list,
	DQLList_iterate_func function,
	void *extra)
{
	void *result = NULL;

	result = DLList_iterate((DLList *) list,
							(DLList_iterate_func) function,
							extra);

	return result;
}

/*******************************************************************************
	function to delete all the nodes in a linked list

  Arguments:
				list		the linked list
				function	the function to call to free the data

	returns:
				nothing

*******************************************************************************/

void DQLList_delete_all (
	DQLList * list,
	DQLList_data_free_func function)
{
	
	DLList_delete_all((DLList *) list,
					  function);
	
	return;
}

/*******************************************************************************
	function to move another deque to the head of a deque
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void DQLList_push_list (
	DQLList * dest,
	DQLList * src)
{
	
	DLList_prepend_list((DLList *) dest, (DLList *) src);
  
	return;
}

/*******************************************************************************
	function to copy another list to the head of a deque
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				func  	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, SLList , stackLList, or QLList to DQLList
				for the src argument
				
*******************************************************************************/

DQLList_node *DQLList_push_list_copy (
	DQLList * dest,
	DQLList * src,
	DQLList_data_copy_func func)
{
	DQLList_node *result = NULL;
	
  result = (DQLList_node *) DLList_prepend_list_copy(
                                                   (DLList *) dest,
                                                   (DLList *) src,
                                                   (DLList_data_copy_func) func);
  
	return result;
}
