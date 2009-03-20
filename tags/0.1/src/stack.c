/*******************************************************************************
	stack.c
	
  filo
  
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
#include <stdint.h>
#include "../include/stack.h"
#include "../include/SLList.h"



/*******************************************************************************
	function to push a node onto a stack
	
  Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
	
*******************************************************************************/

stackLList_node *stackLList_push (
	stackLList * list,
	void *data)
{
	
	SLList_prepend(
				  (SLList *) list,
				  data);
	
	return data;
}

/*******************************************************************************
	function to pop a node from a stack and delete the node
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node held

*******************************************************************************/

void *stackLList_pop (
	stackLList * list)
{
	void *result = NULL;

	result = SLList_delete(
						   (SLList *) list,
						   (SLList_node *) list->head);
	
	return result;
}

/*******************************************************************************
	function to read the head of a stack without delteing it
  
  Arguments:
				list	the linked list
	
	returns:
				the data the node holds

*******************************************************************************/

void *stackLList_read_head (
	stackLList * list)
{
	void *result = list->head->data;

	return result;
}

/*******************************************************************************
	function to count the nodes in a stack

	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t stackLList_length (
	stackLList * list)
{
	size_t result = list->length;
	
	return result;
}


/*******************************************************************************
	function to iterate a stack
  
  Arguments:
				list 		the linked list
				function	the function to pass each node to for processing
				extra		extra data to pass to/from the proccessing function
	
	return:
			the non null returned from the proccessing function that stops the
			iteration
      NULL if the end of the linked list was reached
  
*******************************************************************************/

void *stackLList_iterate (
	stackLList * list,
	stackLList_iterate_func function,
	void *extra)
{
	void *result = NULL;

	result = SLList_iterate((SLList *) list,
							(SLList_iterate_func) function,
							extra);

	return result;
}

/*******************************************************************************
	function to delete all the nodes in a stack
  
  Arguments:
				list		the linked list
				function	the function to call to free the data

	returns:
				nothing

*******************************************************************************/

void stackLList_delete_all (
	stackLList * list,
	stackLList_data_free_func function)
{

	SLList_delete_all ((SLList *) list, (SLList_data_free_func) function);
	list->head = NULL;

	return;
}

/*******************************************************************************
	function to move another stack to the head of a stack
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void stackLList_push_list (
	stackLList * dest,
	stackLList * src)
{
	
	SLList_prepend_list((SLList *) dest, (SLList *) src);
  
	return;
}

/*******************************************************************************
	function to copy another list to the head of a stack
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				func  	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, SLList , QLList, or DQLList to stackLList
				for the src argument
				
*******************************************************************************/

stackLList_node *stackLList_push_list_copy (
	stackLList * dest,
	stackLList * src,
	stackLList_data_copy_func func)
{
	stackLList_node *result = NULL;
	
  result = (stackLList_node *) SLList_prepend_list_copy(
                                                   (SLList *) dest,
                                                   (SLList *) src,
                                                   (SLList_data_copy_func) func);
  
	return result;
}
