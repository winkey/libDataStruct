/*******************************************************************************
	SLList.c
	
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
#include "../include/SLList.h"


/*******************************************************************************
	function to add a node to the head of a single linked list
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
				
*******************************************************************************/

SLList_node *SLList_prepend (
	SLList * list,
	void *data)
{
	SLList_node *new = NULL;

	if ((new = malloc (sizeof (SLList_node)))) {

		new->data = data;
		new->next = list->head;
    
    /***** if there is no tail set it *****/
      
    if (!list->tail)
      list->tail = new;
    
		list->head = new;
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node to the tail of a single linked list
	
	Arguments:
				list	the linked list
				data	the data you wish to store in the linked list
	
	returns:
				the new node
				NULL on error
				
*******************************************************************************/

SLList_node *SLList_append (
	SLList * list,
	void *data)
{
	SLList_node *new = NULL;

	if ((new = malloc (sizeof (SLList_node)))) {

		new->data = data;
		new->next = NULL;

		/***** empty list? *****/

		if (!list->head)
			list->head = list->tail = new;

    else
			list->tail->next = new;
		
		list->tail = new;
		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node after any node in a single linked list
	
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

SLList_node *SLList_insert_after (
	SLList * list,
	SLList_node * node,
	void *data)
{
	SLList_node *new = NULL;
	
	if (list->head && !node) {
	}
	
	else if ((new = malloc (sizeof (SLList_node)))) {

		new->data = data;
		new->next = NULL;
    
		/***** empty list? *****/

		if (!list->head)
			list->head = list->tail = new;
		
		/***** last entry or node is null *****/

		else if (node == list->tail || !node) {
			list->tail->next = new;
			list->tail = new;
		}

		/***** middle entry *****/

		else {
			new->next = node->next;
			node->next = new;
		}

		list->length++;
	}

	return new;
}

/*******************************************************************************
	function to add a node before any node in a single linked list
	
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

SLList_node *SLList_insert_before (
	SLList * list,
	SLList_node * node,
	void *data)
{
	SLList_node *new = NULL;
	SLList_node *prev = NULL;

	if ((new = malloc (sizeof (SLList_node)))) {

		new->data = data;
		new->next = NULL;

		/***** first or only entry or node is null *****/

		if (list->head == node || !node) {
			new->next = list->head;
      list->head = new;
		}

		else {

			/***** find the prev entry *****/

			for (prev = list->head; prev->next != node; prev = prev->next) {
			}

			prev->next = new;
			new->next = node;

		}
		list->length++;
		
	}

	return new;
}

/*******************************************************************************
	function to delete a node from a single linked list
	
	Arguments:
				list	the linked list
				node	the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *SLList_delete (
	SLList * list,
	SLList_node * node)
{

	SLList_node *prev = NULL;
	void *result = node->data;

	/***** only entry *****/

	if (!list->head->next) {
		list->head = list->tail = NULL;
	}

	/***** first entry *****/

	else if (node == list->head)
		list->head = node->next;

	else {

		/***** find the prev entry *****/

		for (prev = list->head; prev->next != node; prev = prev->next) {
		}

		/***** last entry *****/

		if (!node->next) {
			prev->next = NULL;
      list->tail = prev;
		}

		/***** middle entry? *****/

		else
			prev->next = node->next;
	}

	free (node);
	list->length--;

	return result;
}

/*******************************************************************************
	function to delete the node after a node in a single linked list
	
	Arguments:
				list	the linked list
				node	the node before the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *SLList_delete_after (
	SLList * list,
	SLList_node * node)
{

	SLList_node *next = NULL;
	void *result = NULL;

	/***** only entry *****/

	if (list->tail == node && list->head == node) {
	}

  /***** is there a node->next? *****/
  
	else if (!(next = node->next)) {
	}

	else {
    
    /***** last entry? *****/
    
    if (next == list->tail) {
      next = list->tail;
      list->tail = node;
      node->next = NULL;
    }
    
    /***** middle entry *****/
    
    else
      node->next = next->next;
    
    result = next->data;
    free (next);
    list->length--;
	}

	return result;
}

/*******************************************************************************
	function to delete the node before a node in a single linked list
	
	Arguments:
				list	the linked list
				node	the node after the node you wish to delete
	
	returns:
				the data the node held
				
*******************************************************************************/

void *SLList_delete_before (
	SLList * list,
	SLList_node * node)
{

	SLList_node *prev = NULL;
	SLList_node *before = NULL;
	void *result = NULL;

	/***** only entry *****/

	if (list->head == node && !node->next) {
	}

	/***** first entry *****/

	else if ((list->head)->next == node) {
    result = list->head->data;
		free (list->head);
		list->head = node;
 		list->length--;

	}

	else {

		/***** find the prev entry *****/

		before = list->head;
		for (prev = before->next; prev->next != node;
			 before = prev, prev = prev->next) {
		}
    
    result = prev->data;
    
		before->next = node;
		free (prev);
		list->length--;
	}

	return result;
}

/*******************************************************************************
	function to count the nodes in a single linked list
	
	Arguments:
				list	the linked list
	
	returns:
				the number of nodes in the linked list

*******************************************************************************/

size_t SLList_length (
	SLList * list)
{
	size_t result = list->length;

	return result;
}



/*******************************************************************************
	function to iterate a single linked list
	
	Arguments:
				list 		the linked list
				function	the function to pass each node to for processing
				extra		extra data to pass to/from the proccessing function
	
	return:
			the non null returned from the proccessing function that stops the
			iteration
			NULL if the end of the linked list was reached

*******************************************************************************/

void *SLList_iterate (
	SLList * list,
	SLList_iterate_func function,
	void *extra)
{
	SLList_node *node = NULL;
	SLList_node *next = NULL;
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

static void *SLList_delete_all_iterate (
	SLList * list,
	SLList_node * node,
	void *data,
	void *extra)
{
	void (
	*data_free) (
	void *data) = extra;

	data_free (node->data);

	SLList_delete (list, node);

	return NULL;
}

/*******************************************************************************
	function to delete all the nodes in a single linked list
	
	Arguments:
				list		the linked list
				function	the function to call to free the data

	returns:
				nothing

*******************************************************************************/

void SLList_delete_all (
	SLList * list,
	SLList_data_free_func function)
{

	SLList_iterate (list, &SLList_delete_all_iterate, function);
  
	return;
}

/*******************************************************************************
	function to move another single linked list to the head of a single linked
	list
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void SLList_prepend_list (
	SLList * dest,
	SLList * src)
{
	
	/***** is src an empty list *****/

	if (!src->head) {
	}

	else {
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
	function to move another list to the tail of a single linked list
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
	
	returns:
				nothing

*******************************************************************************/

void SLList_append_list (
	SLList * dest,
	SLList * src)
{

	/***** is src an empty list *****/

  if (!src->head) {
  }
  
  /***** is dest empty list? *****/

	if (!dest->head) {
		dest->head = src->head;
    dest->tail = src->tail;
  }
  
  /*****  neither is empty? *****/

	else {
		dest->tail->next = src->head;
    dest->tail = src->tail;
  }
  
	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to move another single linked list to the middle of a single linked
	list, after a particular node
	
	Arguments:
				dest	the linked list you want to move the src to
				src		the linked list you want to move to the dest
				node	the node you wish to insert src after
	returns:
				nothing
	
	if node is null src is appended to dest

*******************************************************************************/

void SLList_insert_list_after (
	SLList * dest,
	SLList * src,
	SLList_node * node)
{

	/***** is node null or tail? then append *****/

	if (!node || node == dest->tail)
		SLList_append_list (dest, src);

	/***** is src an empty list? *****/

	else if (!src->head) {
	}

	else {
    src->tail->next = node->next;
		node->next = src->head;
	}

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to move another single linked list to the middle of a single linked
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

void SLList_insert_list_before (
	SLList * dest,
	SLList * src,
	SLList_node * node)
{
	SLList_node *prev;

	/***** is node null or head? then prepend *****/

	if (!node || node == dest->head)
		SLList_prepend_list (dest, src);

	/***** is src an empty list *****/

	else if (!src->head) {
	}

	else {
		/***** find the node before "node" *****/

		for (prev = dest->head; prev->next != node; prev = prev->next) {
		}

		prev->next = src->head;
    src->tail->next = node;
  }

	dest->length += src->length;
	src->head = NULL;
	src->tail = NULL;
	src->length = 0;

	return;
}

/*******************************************************************************
	function to copy another list to the head of a single linked list
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_prepend_list_copy (
	SLList * dest,
	SLList * src,
	SLList_data_copy_func copy_func)
{
	SLList_node *node;
	void *newdata;
	SLList_node *result = NULL;
	SLList_node *prev = NULL;

	for (node = src->head; node && !result; node = node->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, node->data)) {
			result = node;
		}

		else {
      
		/***** first node we copy? *****/

			if (!prev) {
				if (!(prev = SLList_prepend (dest, newdata)))
					result = node;
			}

			/***** nope insert the node after the last one we copyed *****/

			else if (!(prev = SLList_insert_after (dest, prev, newdata)))
				result = node;
		}
	}

	return result;
}

/*******************************************************************************
	function to copy another list to the tail of a single linked list
	
	
	Arguments:
				dest		the linked list you want to copy the src to
				src			the linked list you want to copy to the dest
				copy_func	function to copy the data held in each node
	
	returns:
				null on success
				the src node we were trying to copy when malloc failed
	
	notes:
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_append_list_copy (
	SLList * dest,
	SLList * src,
	SLList_data_copy_func copy_func)
{
	SLList_node *node;
	void *newdata;
	SLList_node *result = NULL;
	SLList_node *prev = NULL;

	for (node = src->head; node && !result; node = node->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, node->data))
			result = node;

		/**** append the node *****/

		else if (!(prev = SLList_append (dest, newdata)))
      result = node;
    }
    
	return result;
}

/*******************************************************************************
	function to copy another list to the middle of a single linked list, after
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
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_insert_list_after_copy (
	SLList * dest,
	SLList * src,
	SLList_node * node,
	SLList_data_copy_func copy_func)
{
	SLList_node *srcnode;
	void *newdata;
	SLList_node *result = NULL;
	SLList_node *prev = node;

	for (srcnode = src->head; srcnode && !result; srcnode = srcnode->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, srcnode->data))
			result = srcnode;

		/**** insert the node *****/

		else if (!(prev = SLList_insert_after (dest, prev, newdata)))
      result = srcnode;
  }

	return result;
}

/*******************************************************************************
	function to copy another list to the middle of a single linked list, before
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
				you can cast a DLList, stackLList , QLList, or DQLList to SLList
				for the src argument
				
*******************************************************************************/

SLList_node *SLList_insert_list_before_copy (
	SLList * dest,
	SLList * src,
	SLList_node * node,
	SLList_data_copy_func copy_func)
{
	SLList_node *srcnode;
	void *newdata;
	SLList_node *result = NULL;
	SLList_node *prev = NULL;

	for (srcnode = src->head; srcnode && !result; srcnode = srcnode->next) {

		/***** copy the data *****/

		if (!copy_func (&newdata, srcnode->data))
			result = srcnode;

		/**** insert the node *****/

		else {

			/***** first node were copying? *****/

			if (srcnode == src->head) {
				if (!(prev = SLList_insert_before (dest, node, newdata)))
					result = srcnode;
			}

			/***** not the first, we can do this faster with insert after *****/

			else {
				if (!(prev = SLList_insert_after (dest, prev, newdata)))
					result = srcnode;
			}
		}
	}

	return result;
}




/*******************************************************************************
	function to sort a single linked list
	
	Arguments:
				list		the linked list to sort
				cmp_func	function to compare the data in 2 nodes
	
	Returns:
				nothing

	Notes:


*******************************************************************************/

void SLList_sort (
	SLList * list,
	SLList_data_cmp_func cmp_func)
{
	SLList a = { 0 };
	SLList b = { 0 };
	SLList new = { 0 };
  SLList *next = NULL;
  
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
        if (!new.head)
          new.head = new.tail = next->head;
        else
          new.tail = new.tail->next = next->head;

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
