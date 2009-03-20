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

#define MAKING_LL


/***** our headers *****/

#include <stddef.h>
#include <stdlib.h>
#include "../include/CLList.h"
#include "../include/DLList.h"

/*******************************************************************************
	function to add a node to the tail of a circular linked list
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error

  note:
        the new node becomes the tail

*******************************************************************************/

CLList_node *CLList_append (
	CLList * list,
	void *data)
{
	CLList_node *new = NULL;
  
  /***** remove the circle *****/
  
  if (list->head) {
    list->head->prev = NULL;
    list->tail->next = NULL;
  }
  
	new = (CLList_node *) DLList_append((DLList *) list, data);
    
	/***** tie it in a circle *****/
	
  if (list->head) {
  	list->head->prev = list->tail;
		list->tail->next = list->head;
	}
  
  return new;
}

/*******************************************************************************
	function to add a node after any node in a circular linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to place the new node after
				data	the data you wish to store in the linked list
		
	returns:
				the new node
				NULL on error

  note:
        if node is null or the list is empty the new node will be appended to
        the list
        the new node becomes the tail

*******************************************************************************/

CLList_node *CLList_insert_after (
	CLList * list,
	CLList_node * node,
	void *data)
{
  CLList_node *new = NULL;


	if ((new = malloc (sizeof (CLList_node)))) {

		new->data = data;
		new->next = NULL;
		new->prev = NULL;

		/***** empty list? *****/

		if (!list->head && !list->tail) {
			list->head = new;
			list->tail = new;
		}
    
    /***** node null? *****/
    
    else if (!node) {
    	new->prev = list->tail;
			new->next = list->head;
      list->head->prev = new;
      list->tail->next = new;
      list->tail = new;
    }		  
 
		/***** everything else is a middle entry *****/

		else {
			new->prev = node;
			new->next = node->next;
			node->next->prev = new;
			node->next = new;
      list->tail = new;
      list->head = new->next;
    }
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node before any node in a circular linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to place the new node before
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error

note:
        if node is null or the list is empty the new node will be appended to
        the list
        the new node becomes the tail
				
*******************************************************************************/

CLList_node *CLList_insert_before (
	CLList *list,
	CLList_node * node,
	void *data)
{
	CLList_node *new = NULL;


	if ((new = malloc (sizeof (CLList_node)))) {

		new->data = data;
		new->next = NULL;
		new->prev = NULL;

    /***** empty list? *****/

		if (!list->head && !list->tail) {
			list->head = new;
			list->tail = new;
		}

    /***** node null? *****/
    
    else if (!node) {
    	new->prev = list->tail;
			new->next = list->head;
      list->head->prev = new;
      list->tail->next = new;
      list->tail = new;
    }		  
 
		/***** everything else is a middle entry *****/
    
		else {
			new->prev = node->prev;
			new->next = node;
			node->prev->next = new;
			node->prev = new;
		  list->tail = new;
      list->head = new->next;
    }
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to delete a node from a circular linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to delete
	
	returns:
				the data the node held
  
  note:
        the head becomes the next node

*******************************************************************************/

void *CLList_delete (
	CLList *list,
	CLList_node * node)
{
	void *result = NULL;
	CLList_node *next = NULL;
  
	/***** remove the circle *****/
  
  if (list->head) {
    list->head->prev = NULL;
    list->tail->next = NULL;
  }
  
  if (list->head != list->tail)
    next = node->next;
  
  result = DLList_delete(
						   (DLList *) list,
						   (DLList_node *) node);
	
	/***** tie it in a circle *****/
	
  if (list->head) {
  	list->head->prev = list->tail;
		list->tail->next = list->head;
	}
  
  /***** if theres a node left set head to the next node *****/
  
  if (next) {
    list->head = next;
    list->tail = next->prev;
  }
	
  return result;
}

/*******************************************************************************
	function to delete the node after a node in a circular linked list
	
	Arguments:
				list	the linked list
				node	the node before the node you wish to delete
	
	returns:
				the data the node held

  note:
        the node becomes the tail

*******************************************************************************/

void *CLList_delete_after (
	CLList *list,
	CLList_node * node)
{
  CLList_node *next = NULL;
	void *result = NULL;

	/***** node only entry? *****/

	if (list->head == node && list->tail == node) {
	}

	/***** node NULL? *****/

	else if (!node) {
	}

	else {
    next = node->next;
		result = next->data;

		next->next->prev = node;
    node->next = next->next;
		
    list->tail = node;
    list->head = node->next;
    
		free (next);
    
		list->length--;
	}

	return result;
}

/*******************************************************************************
	function to delete the node before a node in a circular linked list
	
	Arguments:
				list	the linked list
				node	the node after the node you wish to delete
	
	returns:
				the data the node held

  note:
        the node becomes the head

*******************************************************************************/

void *CLList_delete_before (
	CLList *list,
	CLList_node * node)
{
	CLList_node *prev = NULL;
	void *result = NULL;

	/***** node only entry? *****/

	if (list->head == node && list->tail == node) {
	}

	/***** node null? *****/

	else if (!node) {
	}

	else {
    prev = node->prev;
		result = prev->data;

		prev->prev->next = node;
		node->prev = prev->prev;
		
    list->head = node;
    list->tail = node->prev;
    
		free (prev);
		
		list->length--;
	}

	return result;
}

/*******************************************************************************
	function to count the nodes in a circular linked list
	
	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t CLList_length (
	CLList * list)
{
	size_t result = list->length;

	return result;
}


/*******************************************************************************
	function to iterate a circular linked list
	
	Arguments:
				list 		the linked list
				function	the function to pass each node to for processing
				extra		extra data to pass to/from the proccessing function
	
	return:
			the non null returned from the proccessing function that stops the
			iteration
			NULL if the end of the linked list was reached

  note:
      if the proccessing function does not return NULL the tail becomes the
      current node
      
*******************************************************************************/

void *CLList_iterate (
	CLList *list,
	CLList_iterate_func function,
	void *extra)
{	
	CLList_node *node = NULL;
	CLList_node *next = NULL;
	void *result = NULL;

  /***** remove the circle *****/
  
  if (list->head) {
    list->head->prev = NULL;
    list->tail->next = NULL;
  }
  
	for (node = list->head; node && !result; node = next) {
		next = node->next;
		result = function (list, node, node->data, extra);
	}

  /***** tie it in a circle *****/
	
  if (list->head) {
  	list->head->prev = list->tail;
		list->tail->next = list->head;
	}
  
  /***** set head to the node iteration was stopped on *****/
  
  if (result && node ) {
    list->head = node;
    list->tail = node->prev;
  }
	
	return result;
}

/*******************************************************************************
	function to delete all the nodes in a circular linked list
	
	Arguments:
				list		the linked list
				function	the function to call to free the data

	returns:
				nothing

*******************************************************************************/

void CLList_delete_all (
	CLList * list,
	CLList_data_free_func function)
{
	
	/***** remove the circle *****/
  
  if (list->head) {
    list->head->prev = NULL;
    list->tail->next = NULL;
  }
  
  DLList_delete_all((DLList *) list, (DLList_data_free_func) function);
	
	return;
}

/*******************************************************************************
  function to rotate the Circular linked list
  
  Arguments:
        list      the linked list
        nodes     the number of nodes to rotate the linked list
        reverse   non zero value to reverse the rotation
        
  returns:
        nothing

  note:
        if the number of nodes is greater than or equal to the number of nodes
        in the list head, and tail will be simply swapped
*******************************************************************************/

void CLList_rotate (
  CLList *list,
  size_t nodes,
  int reverse)
{
  size_t i;
  
  if (nodes >= CLList_length(list)) {
    list->head = list->tail;
  }
  
  for (i = 0 ; i < nodes ; i++) {
    if (!reverse)
      list->head = list->head->next;
    else
      list->head = list->head->prev;
  }
  
  return;
}

/*******************************************************************************
	function to move another double linked list to the tail of a circular linked
	list
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

  note:
 				you can cast a QLList, or DQLList or DLList to CLList for the src 
        argument
        the head becomes the head of dest if dest is not empty, otherwise head
        becomes the head of src
*******************************************************************************/

void CLList_append_list (
	CLList * dest,
	CLList * src)
{
CLList_node *head = dest->head;
    
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }
  
  DLList_append_list((DLList *) dest, (DLList *) src);
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return;
}

/*******************************************************************************
	function to move another double linked list to the middle of a circular linked
	list, after a particular node
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
				node	the node you wish to insert src after
	returns:
				nothing
	
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
	CLList_node * node)
{
  CLList_node *head = NULL;
  
  if (node)
    head = node->next;
  else if (dest->head)
    head = dest->head;
    
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }

  DLList_insert_list_after((DLList *) dest,
                           (DLList *) src,
                           (DLList_node *) node);
  
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return;
}

/*******************************************************************************
	function to move another double linked list to the middle of a circular linked
	list, before a particular node
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
				node	the node you wish to insert src before

	returns:
				nothing
	
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
	CLList_node * node)
{
  CLList_node *head = NULL;
  
  if (node)
    head = node;
  else if (dest->head)
    head = dest->head;
    
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }

  DLList_insert_list_before((DLList *) dest,
                           (DLList *) src,
                           (DLList_node *) node);
  
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return;
}

/*******************************************************************************
	function to copy another list to the tail of a circular linked list
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
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
	CLList_data_copy_func copy_func)
{
  CLList_node *head = dest->head;
  CLList_node *result = NULL;
  
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }
  
  DLList_append_list_copy((DLList *) dest,
                          (DLList *) src,
                          (DLList_data_copy_func) copy_func);
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return result;
}

/*******************************************************************************
	function to copy another list to the middle of a circular linked list, after
	a particular node

	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				node		the dest node you wish to copy the data after
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
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
	CLList_data_copy_func copy_func)
{
  CLList_node *head = NULL;
  CLList_node *result = NULL;
  
  if (node)
    head = node->next;
  else if (dest->head)
    head = dest->head;
    
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }
  
  result = (CLList_node *)
    DLList_insert_list_after_copy((DLList *) dest,
                                  (DLList *) src,
                                  (DLList_node *) node,
                                  (DLList_data_copy_func) copy_func);
  
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return result;
}

/*******************************************************************************
	function to copy another list to the middle of a circular linked list, before
	a particular node

	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				node		the dest node you wish to copy the data before
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
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
	CLList_data_copy_func copy_func)
{
  CLList_node *head = NULL;
  CLList_node *result = NULL;
  
  if (node)
    head = node;
  else if (dest->head)
    head = dest->head;
    
  /***** remove the circle *****/
  
  if (dest->head) {
    dest->head->prev = NULL;
    dest->tail->next = NULL;
  }
  if (src->head) {
    src->head->prev = NULL;
    src->tail->next = NULL;
  }

  result = (CLList_node *)
    DLList_insert_list_before_copy((DLList *) dest,
                                   (DLList *) src,
                                   (DLList_node *) node,
                                   (DLList_data_copy_func) copy_func);
  
  
  /***** tie it in a circle *****/
	
  if (dest->head) {
  	dest->head->prev = dest->tail;
		dest->tail->next = dest->head;
	}
  
  if (head) {
    dest->head = head;
    dest->tail = head->prev;
  }
  
  return result;
}

