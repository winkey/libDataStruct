/*******************************************************************************
	DLList.c
	
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

#include <stddef.h>
#include <stdlib.h>
#include "../include/DLList.h"


/*******************************************************************************
	function to add a node to the head of a double linked list
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
				
*******************************************************************************/

DLList_node *DLList_prepend (
	DLList *list,
	void *data)
{
	DLList_node *new = NULL;

	if ((new = malloc (sizeof (DLList_node)))) {

		new->data = data;
		new->next = list->head;
		new->prev = NULL;
		list->head = new;

		/***** empty list? *****/

		if (!list->tail)
			list->tail = new;
		else
			new->next->prev = new;
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node to the tail of a double linked list
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
				
*******************************************************************************/

DLList_node *DLList_append (
	DLList * list,
	void *data)
{
	DLList_node *new = NULL;

	if ((new = malloc (sizeof (DLList_node)))) {

		new->data = data;
		new->next = NULL;
		new->prev = list->tail;
		list->tail = new;

		/***** empty list? *****/

		if (!list->head)
			list->head = new;
		else
			new->prev->next = new;
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node after any node in a double linked list
	
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
 
*******************************************************************************/

DLList_node *DLList_insert_after (
	DLList * list,
	DLList_node * node,
	void *data)
{
	DLList_node *new = NULL;


	if ((new = malloc (sizeof (DLList_node)))) {

		new->data = data;
		new->next = NULL;
		new->prev = NULL;

		/***** empty list? *****/

		if (!list->head && !list->tail) {
			list->head = new;
			list->tail = new;
		}
    
    /***** last entry or node is null *****/

		else if (list->tail == node || !node) {
			new->prev = list->tail;
			list->tail = new;
			node->next = new;
		}

		/***** middle entry *****/

		else {
			new->prev = node;
			new->next = node->next;
			node->next->prev = new;
			node->next = new;
		}
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node before any node in a double linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to place the new node before
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
  
  note:
        if node is null or the list is empty the new node will be prepended to
        the list
 
*******************************************************************************/

DLList_node *DLList_insert_before (
	DLList * list,
	DLList_node * node,
	void *data)
{
	DLList_node *new = NULL;


	if ((new = malloc (sizeof (DLList_node)))) {

		new->data = data;
		new->next = NULL;
		new->prev = NULL;

	/***** empty list? *****/

		if (!list->head && !list->tail) {
			list->head = new;
			list->tail = new;
		}

	/***** first entry or node is null *****/

		else if (list->head == node || !node) {
			new->next = list->head;
			list->head = new;
			node->prev = new;
		}

	/***** middle entry *****/

		else {
			new->prev = node->prev;
			new->next = node;
			node->prev->next = new;
			node->prev = new;
		}
		
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to delete a node from a double linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *DLList_delete (
	DLList * list,
	DLList_node * node)
{
	void *result = node->data;

	/***** only entry *****/

	if (list->head == node && list->tail == node) {
		list->head = NULL;
		list->tail = NULL;
	}

	/***** last entry *****/

	else if (node == list->tail) {
		node->prev->next = NULL;
		list->tail = node->prev;
	}

	/***** first entry *****/

	else if (node == list->head) {
		node->next->prev = NULL;
		list->head = node->next;
	}

	/***** middle entry? *****/

	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	free (node);
	list->length--;

	return result;
}

/*******************************************************************************
	function to delete the node after a node in a double linked list
	
	Arguments:
				list	the linked list
				node	the node before the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *DLList_delete_after (
	DLList * list,
	DLList_node * node)
{
  DLList_node *next = NULL;
	void *result = NULL;

	/***** node only entry? *****/

	if (list->head == node && list->tail == node) {
	}

	/***** node last entry? *****/

	else if (node == list->tail) {
	}

	else {
    next = node->next;
		result = next->data;

		/***** last entry *****/

		if (next == list->tail) {
			node->next = NULL;
			list->tail = node;
		}

		/***** middle entry? *****/

		else {
			next->next->prev = node;
			node->next = next->next;
		}

		free (next);
    
		list->length--;
	}

	return result;
}

/*******************************************************************************
	function to delete the node before a node in a double linked list
	
	Arguments:
				list	the linked list
				node	the node after the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *DLList_delete_before (
	DLList * list,
	DLList_node * node)
{
  DLList_node *prev = NULL;
	void *result = NULL;

	/***** node only entry? *****/

	if (list->head == node && list->tail == node) {
	}

	/***** node first entry? *****/

	else if (node == list->head) {
	}

	else {
    prev = node->prev;
		result = prev->data;

		/***** first entry *****/

		if (prev == list->head) {
			node->prev = NULL;
			list->head = node;
		}

		/***** middle entry? *****/

		else {
			prev->prev->next = node;
			node->prev = prev->prev;
		}

		free (prev);
		
		list->length--;
	}

	return result;
}


/*******************************************************************************
	function to count the nodes in a double linked list
	
	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t DLList_length (
	DLList * list)
{
	size_t result = list->length;

	return result;
}

/*******************************************************************************
	function to iterate a double linked list
	
	Arguments:
				list 		the linked list
				function	the function to pass each node to for processing
				extra		extra data to pass to/from the proccessing function
	
	return:
			the non null returned from the proccessing function that stops the
			iteration
			NULL if the end of the linked list was reached

*******************************************************************************/

void *DLList_iterate (
	DLList * list,
	DLList_iterate_func function,
	void *extra)
{
	DLList_node *node = NULL;
	DLList_node *next = NULL;
	void *result = NULL;

	for (node = list->head; node && !result; node = next) {
		next = node->next;
		result = function (list, node, node->data, extra);
	}

	return result;
}

/*******************************************************************************
	slave function to delete all the nodes in a linked list
*******************************************************************************/

static void *DLList_delete_all_iterate (
	DLList * list,
	DLList_node * node,
	void *data,
	void *extra)
{
	DLList_data_free_func data_free = extra;

	data_free (node->data);

	DLList_delete (list, node);

	return NULL;
}

/*******************************************************************************
	function to delete all the nodes in a double linked list
	
	Arguments:
				list		the linked list
				function	the function to call to free the data

	returns:
				nothing

*******************************************************************************/

void DLList_delete_all (
	DLList * list,
	DLList_data_free_func data_free)
{

	DLList_iterate (list, &DLList_delete_all_iterate, data_free);
	
	return;
}

/*******************************************************************************
	function to move another double linked list to the head of a double linked
	list
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void DLList_prepend_list (
	DLList * dest,
	DLList * src)
{

	/***** is src an empty list *****/

	if (!src->head) {
	}

	else {
		
		if (dest->head)
			dest->head->prev = src->tail;
		
		src->tail->next = dest->head;
		dest->head = src->head;
	}

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to move another double linked list to the tail of a double linked
	list
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void DLList_append_list (
	DLList * dest,
	DLList * src)
{

	/***** is dest empty list? *****/

	if (!dest->head) {
		dest->head = src->head;
		dest->tail = src->tail;
	}

	/***** is src an empty list *****/

	else if (!src->head) {
	}

	/*****  neither is empty? *****/

	else {
		dest->tail->next = src->head;
		src->head->prev = dest->tail;
		dest->tail = src->tail;
	}

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to move another double linked list to the middle of a double linked
	list, after a particular node
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
				node	the node you wish to insert src after
	returns:
				nothing
	
	if node is null src is appended to dest

*******************************************************************************/

void DLList_insert_list_after (
	DLList * dest,
	DLList * src,
	DLList_node * node)
{
	
	/***** is node null? then append *****/

	if (!node)
		DLList_append_list (dest, src);

	/***** if node is tail were appending *****/
	
	else if (node == dest->tail)
		DLList_append_list (dest, src);
	
	/***** is dest empty list? *****/

	else if (!dest->head) {
		dest->head = src->head;
		dest->tail = src->tail;
	}

	/***** is src an empty list *****/

	else if (!src->head) {
	}

	else {

		src->tail->next = node->next;
		node->next->prev = src->tail;
		node->next = src->head;
		src->head->prev = node;
		
	}

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to move another double linked list to the middle of a double linked
	list, before a particular node
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
				node	the node you wish to insert src before

	returns:
				nothing
	
	notes:
				if node is null src is appended to dest

*******************************************************************************/

void DLList_insert_list_before (
	DLList * dest,
	DLList * src,
	DLList_node * node)
{
	
	/***** is node null? then prepend *****/

	if (!node)
		DLList_prepend_list (dest, src);

	/***** if node is head were prepending *****/
	
	else if (node == dest->head)
		DLList_prepend_list (dest, src);
	
	/***** is dest empty list? *****/

	else if (!dest->head) {
		dest->head = src->head;
		dest->tail = src->tail;
	}

	/***** is src an empty list *****/

	else if (!src->head) {
	}

	else {
		node->prev->next = src->head;
		src->head->prev = node->prev;
		node->prev = src->tail;
		src->tail->next = node;
	}

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to copy another list to the head of a double linked list
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a SLList, stackLList , QLList, or DQLList to DLList
				for the src argument
				
*******************************************************************************/

DLList_node *DLList_prepend_list_copy (
	DLList * dest,
	DLList * src,
	DLList_data_copy_func copy_func)
{
	DLList_node *node;
	void *newdata;
	DLList_node *result = NULL;
	DLList_node *prev = NULL;

	for (node = src->head; node && !result; node = node->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, node->data)) {
			result = node;
		}

		else {

			/***** first node we copy? *****/

			if (!prev) {
				if (!(prev = DLList_prepend (dest, newdata)))
					result = node;
			}

			/***** nope insert the node after the last one we copyed *****/

			else if (!(prev = DLList_insert_after (dest, prev, newdata)))
				result = node;
		}
	}

	return result;
}

/*******************************************************************************
	function to copy another list to the tail of a double linked list
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a SLList, stackLList , QLList, or DQLList to DLList
				for the src argument
				
*******************************************************************************/

DLList_node *DLList_append_list_copy (
	DLList * dest,
	DLList * src,
	DLList_data_copy_func copy_func)
{
	DLList_node *node;
	void *newdata;
	DLList_node *result = NULL;
  
	for (node = src->head; node && !result; node = node->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, node->data))
			result = node;

		/**** append the node *****/

		else if (!DLList_append (dest, newdata))
			result = node;
	}

	return result;
}

/*******************************************************************************
	function to copy another list to the middle of a double linked list, after
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
				you can cast a SLList, stackLList , QLList, or DQLList to DLList
				for the src argument
				
*******************************************************************************/

DLList_node *DLList_insert_list_after_copy (
	DLList * dest,
	DLList * src,
	DLList_node * node,
	DLList_data_copy_func copy_func)
{
	DLList_node *srcnode;
	void *newdata;
	DLList_node *result = NULL;
	DLList_node *prev = node;

	for (srcnode = src->head; srcnode && !result; srcnode = srcnode->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, srcnode->data))
			result = srcnode;

		/**** insert the node *****/

		else if (!(prev = DLList_insert_after (dest, prev, newdata)))
			result = srcnode;
	}

	return result;
}

/*******************************************************************************
	function to copy another list to the middle of a double linked list, before
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
				you can cast a SLList, stackLList , QLList, or DQLList to DLList
				for the src argument
				
*******************************************************************************/

DLList_node *DLList_insert_list_before_copy (
	DLList * dest,
	DLList * src,
	DLList_node * node,
	DLList_data_copy_func copy_func)
{
	DLList_node *srcnode;
	void *newdata;
	DLList_node *result = NULL;
	DLList_node *prev = NULL;
  
  for (srcnode = src->head; srcnode && !result; srcnode = srcnode->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, srcnode->data))
			result = srcnode;

		/**** insert the node *****/

		else if (!(prev = DLList_insert_before (dest, node, newdata)))
				result = srcnode;
	}

	return result;
}

/*******************************************************************************
	function to sort a double linked list
	
	Arguments:
				list		the linked list to sort
				cmp_func	function to compare the data in 2 nodes
	
	Returns:
				nothing

	Notes:


*******************************************************************************/

void DLList_sort (
	DLList * list,
	DLList_data_cmp_func cmp_func)
{
	DLList a = { 0 };
	DLList b = { 0 };
	DLList new = { 0 };
  DLList *next = NULL;
  
	size_t mergesize = 1;
	size_t merges;
	size_t i;
  
	/***** if the list has one or less nodes its already sorted *****/

	if (list->length < 2)
		return;

	/***** assign the list to new *****/

	new = *list;
  
	do {
		merges = 0;

		/***** assign new to a *****/
    
		a = new;
    
    /***** clear new *****/
    
    new.head = new.tail = NULL;

		for (b.head = a.head; a.head; a.head = b.head) {
			merges++;
      
      a.length = 0;
      
      /***** make b point to mergesize nodes after a *****/
      
			for (i = 0 ; b.head && i < mergesize; i++) {
				a.length++;
				b.head = b.head->next;
			}

			b.length = mergesize;
      
      /***** merge while a or b has something to merge *****/
      
			while (a.length > 0 || (b.length > 0 && b.head)) {
        
        /***** is a empty? *****/

        if (a.length == 0)
          next = &b;

        /***** is b empty or a lower than b? *****/

        else if (b.length == 0 || !b.head ||
             cmp_func (a.head->data, b.head->data) <= 0)
          next = &a;

        /***** b is lower *****/

        else
          next = &b;

        /***** ad next to the new list *****/
        
        /***** is new empty? *****/
        if (!new.head) {
          new.head = new.tail = next->head;
          new.head->prev = NULL;
        }
        else {
          next->head->prev = new.tail;
          new.tail = new.tail->next = next->head;
        }

        /***** next is pointing at a or b make its head   *****/
        /***** point at its next node and decr its length *****/

        next->length--;
        next->head = next->head->next;
        
        new.tail->next = NULL;

      }
    }

	/***** double the mergesize *****/

		mergesize *= 2;

	/***** loop till theres one or less merges *****/

	} while (merges > 1);

	/***** reassign the sorted list to the list *****/

	list->head = new.head;
	list->tail = new.tail;
  
	return;
}
