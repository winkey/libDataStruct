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


/***** our headers *****/

#include <stddef.h>
#include <stdlib.h>
#include "../include/QLList.h"
#include "../include/DLList.h"



/*******************************************************************************
	function to push a node onto a que
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error

*******************************************************************************/

void *QLList_push (
	QLList *list,
	void *data)
{
	
	DLList_prepend(
				  (DLList *) list,
				  data);
	
	return data;
}

/*******************************************************************************
	function to pull a node from a que and delete the node
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node held

*******************************************************************************/

void *QLList_pull (
	QLList * list)
{
	void *result = NULL;

	result = DLList_delete(
						   (DLList *) list,
						   (DLList_node *) list->tail);
	
	return result;
}

/*******************************************************************************
	function to read the data in the tail of the que without delteing it
	
  Arguments:
				list	the linked list
	
	returns:
				the data the node holds

*******************************************************************************/

void *QLList_read (
	QLList * list)
{
	void *result = list->tail->data;

	return result;
}

/*******************************************************************************
	function to count the nodes in a que

	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t QLList_length (
	QLList * list)
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

void *QLList_iterate (
	QLList * list,
	QLList_iterate_func function,
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

void QLList_delete_all (
	QLList * list,
	QLList_data_free_func function)
{
	
	DLList_delete_all((DLList *) list, function);
	
	return;
}

/*******************************************************************************
	function to move another que to the head of a que
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void QLList_push_list (
	QLList * dest,
	QLList * src)
{
	
	DLList_prepend_list((DLList *) dest, (DLList *) src);
  
	return;
}

/*******************************************************************************
	function to copy another list to the head of a que
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				func  	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, SLList , stackLList, or DQLList to QLList
				for the src argument
				
*******************************************************************************/

QLList_node *QLList_push_list_copy (
	QLList * dest,
	QLList * src,
	QLList_data_copy_func func)
{
	QLList_node *result = NULL;
	
  result = (QLList_node *) DLList_prepend_list_copy(
                                                   (DLList *) dest,
                                                   (DLList *) src,
                                                   (DLList_data_copy_func) func);
  
	return result;
}
