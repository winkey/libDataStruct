/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/DLList.h"

#include "../include/MWTree.h"


void *levelorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra);


/*******************************************************************************
  function to find a node in a multi way tree
*******************************************************************************/

MWTree_node *find(
  MWTree *t,
	MWTree_node *n,
  void *data)
{
	
	if (n) {
		if (!t->cmp(data, n->data))
			return n;
		
		else {
			DLList_node *child;
			for (child = n->children.head ; child; child = child->next) {
				MWTree_node *c = child->data;
				
				if (!t->cmp(data, c->data))
					return c;
			}
		}
	}
	
	return NULL;
}

/*******************************************************************************
  function to find a node in a multi way tree
  
  args:
        tree  the tree to find the node in
        data  the data to look for
  
  returns:
        the node that holds the data
        null if the data is not found
  
  notes:
        the data passed to this function to look for is only bound by the data
        compare function

*******************************************************************************/

MWTree_node *MWTree_find(
  MWTree *tree,
  void *data)
{
	return find(tree, tree->root, data);
	
}

/*******************************************************************************
	function to allocate and clear a new node
*******************************************************************************/

MWTree_node *newnode(void) {
  
  MWTree_node *new = NULL;

  if ((new = malloc(sizeof(MWTree_node)))) {
    new->parent = NULL;
    new->children.length = 0;
    new->children.head = NULL;
    new->children.tail = NULL;
    new->data = NULL;
  }

  return new;
}


/*******************************************************************************
	function to add a new node to the tree
*******************************************************************************/

MWTree_node *add(
	MWTree *t,
	MWTree_node *n,
	MWTree_node *p,
  int add_on_zero,
	void *data)
{
	MWTree_node *new = NULL;
	
	/***** if the node is null then our new node is the parents child *****/
	
	if (!n) {
	
		/***** alocate memory for the node *****/
		
		if (!(new = newnode()))
			return NULL;
		
		new->data = data;
		t->length++;
		new->parent = p;

    /***** if there is a parent append, otherwise the nde is the root *****/
    
		if (p)
			DLList_append(&p->children, new);
		
		return new;
	}
	
	/***** compare *****/
	
	int cmp = t->cmp(data, n->data);

	/***** if the new node is greater than the current node *****/

	if (cmp > 0) {
		
		/***** alocate memory for the node *****/
		
		if (!(new = newnode()))
			return NULL;
		
		new->data = data;
		t->length++;
		new->parent = p;

    /***** if theres no parent then the new node is the root *****/
    
		if (!p) {
			t->root = new;
      printf("gotcha\n");
    }
    
    /***** take the place of the current node *****/

    else {

      /***** go though the children of the parent node *****/
			
			DLList_node *c = NULL, *next = NULL; 
			for (c = p->children.head ; c ; c = next) {
        next = c->next;

        MWTree_node *ctn = c->data;

        /***** is the new node greater than the parents child node? *****/
        
        if (n == ctn || t->cmp(data, ctn->data) > 0) {

          /***** move that child node to the new nodes children *****/

          DLList_delete(&p->children, c);
          DLList_append(&(new->children), ctn);
          ctn->parent = new;
        }
			}
    
      /***** append the new node to the parent *****/
    
      DLList_append(&(p->children), new);
		}

		return new;
	}

  /***** if the new node is less than the current node *****/

  else if (cmp < 0) {
    
    /***** parse the nodes children *****/
    
    DLList_node *c = NULL;
    for (c = n->children.head ; c ; c = c->next) {
      
			if ((new = add(t, c->data, n, 0, data)))
				return new;
    }

		if ((new = add(t, NULL, n, cmp, data)))
			return new;

  }

  else if (add_on_zero) {
		if ((new = add(t, NULL, n, cmp, data)))
			return new;
  }

  return NULL;
}
					
/*******************************************************************************
  function to add a node to a multi way tree
  
  args:
        tree  the tree to add the node to
        data  the data the node is to hold
        func  compare function for the data
  
  returns:
        NULL if malloc fails
        the new node
*******************************************************************************/

MWTree_node *MWTree_insert (
  MWTree *tree,
  void *data)
{

	printf("MWTree_insert\n");
	MWTree_node *new = add(tree, tree->root, NULL, 0, data);
	
	/***** if the tree is empty we have our first node *void *levelorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra)****/
	
	if (!tree->root)
		tree->root = new;
	
	return new;
}

/*******************************************************************************
  funtion to delete a node from a multi way tree
  
  args:
        tree  the tree to delete the node from
        node  the node to delete
  
  returns:
        the data the node held
*******************************************************************************/

void *MWTree_delete (
  MWTree *tree,
  MWTree_node *node)
{
	DLList_node *child;
	MWTree_node *p = node->parent;
	
	/***** tell all the children they live with there grandparents now *****/
	
	for (child = node->children.head ; child ; child = child->next) {
		MWTree_node *c = child->data;
		c->parent = p;
	}
	
	/***** move the children in with there grandparents *****/
	
	if (p && node->children.head) {
		DLList_append_list(&(p->children), &(node->children));
	}
	/***** tell dad to disown me *****/
	
	DLList_node *oc = NULL;
	if (p) {
		for (oc = p->children.head ; oc ; oc = oc->next) {
			if (oc->data == node) {
				DLList_delete(&(p->children), oc);
				break;
			}
		}
	}
	
	/***** last node? *****/
	
	if (node == tree->root && !node->children.head)
		tree->root = NULL;
	
	/***** give the house away *****/
	
	void *data = node->data;
	free(node);
	tree->length--;
	
	return data;
}

/*******************************************************************************
	function to count the nodes in a multi way tree
	
	Arguments:
				tree	the tree
	
	returns:
				the number of nodes in the tree

*******************************************************************************/

size_t MWTree_length (
	MWTree * tree)
{
	
	return tree->length;
}

/*******************************************************************************
	inorder recursive function
*******************************************************************************/

void *inorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	DLList_node *child, *next, *prev;
	
	if (!converse) {
		for (child = n->children.head ; child ; child = next) {
			next = child->next;
			MWTree_node *c = child->data;
			
			if ((result = inorder(t, c, converse, func, extra)))
				return result;
			
			else if ((result = func(t, c, c->data, extra)))
				return result;
		}
	}
	
	else {
		
		for (child = n->children.tail ; child ; child = prev) {
			prev = child->prev;
			MWTree_node *c = child->data;
			
			if ((result = inorder(t, c, converse, func, extra)))
				return result;
			
			else if ((result = func(t, c, c->data, extra)))
				return result;
		}
	}
	
	return result;
}

/*******************************************************************************
  function to traverse a multi way tree inorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_inorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	if (!tree->root)
		return NULL;
	
	if ((result = inorder(tree, tree->root, converse, func, extra)))
			return result;
	
	return func(tree, tree->root, tree->root->data, extra);
}

/*******************************************************************************
	preorder recursive function
*******************************************************************************/

void *preorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	DLList_node *child;
	
	if (!converse) {
		
		for (child = n->children.head ; child ; child = child->next) {
			MWTree_node *c = child->data;
			
			if ((result = func(t, c, c->data, extra)))
				return result;
			
			else if ((result = preorder(t, c, converse, func, extra)))
				return result;
			
		}
	}
	
	else {
		
		for (child = n->children.tail ; child ; child = child->prev) {
			MWTree_node *c = child->data;
			
			if ((result = func(t, c, c->data, extra)))
				return result;
			
			if ((result = preorder(t, c, converse, func, extra)))
				return result;
		}
	}
	
	return result;
}

/*******************************************************************************
  function to traverse a multi way tree preorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_preorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	if (!tree->root)
		return NULL;
	
	/****** the root is first in a multi way tree *****/
	
	if ((result = func(tree, tree->root, tree->root->data, extra)))
			return result;
	
	return preorder(tree, tree->root, converse, func, extra);
}
	
/*******************************************************************************
	postorder recursive function
*******************************************************************************/

void *postorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	if (!converse) {
		
		DLList_node *child;
		for (child = n->children.head ; child ; child = child->next) {
			MWTree_node *c = child->data;
			
			if ((result = postorder(t, c, converse, func, extra)))
				return result;
		}
		for (child = n->children.head ; child ; child = child->next) {
			MWTree_node *c = child->data;
			
			if ((result = func(t, c, c->data, extra)))
				return result;
		}
	}
	
	else {
		
		DLList_node *child;
		for (child = n->children.tail ; child ; child = child->prev) {
			MWTree_node *c = child->data;
			
			if ((result = postorder(t, c, converse, func, extra)))
				return result;
		}
		for (child = n->children.tail ; child ; child = child->prev) {
			MWTree_node *c = child->data;
			
			if ((result = func(t, c, c->data, extra)))
				return result;
		}
	}
	
	return result;
}

/*******************************************************************************
  function to traverse a multi way tree postorder
  
  Arguments:
				tree 		  the tree to traverse
        converse  if true swap left and right
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_postorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	if (!tree->root)
		return NULL;
	
	if ((result = postorder(tree, tree->root, converse, func, extra)))
		return result;

	else if ((result = func(tree, tree->root, tree->root->data, extra)))
		return result;
	
	return result;
}

/*******************************************************************************
	levelorder recursive function
*******************************************************************************/

void *levelorder (
  MWTree *t,
  MWTree_node *n,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	void *result = NULL;
	
	if (n == t->root) {
		if ((result = func(t, n, n->data, extra)))
			return result;
	}
	

		
		if (!converse) {
			
			DLList_node *child;
			for (child = n->children.head ; child ; child = child->next) {
				MWTree_node *c = child->data;
				
				if ((result = func(t, c, c->data, extra)))
					return result;
			}
			
			for (child = n->children.head ; child ; child = child->next) {
				MWTree_node *c = child->data;
				
				if ((result = levelorder(t, c, converse, func, extra)))
					return result;
			}
		}
		
		else {
			
			DLList_node *child;
			for (child = n->children.tail ; child ; child = child->prev) {
				MWTree_node *c = child->data;
				
				if ((result = func(t, c, n->data, extra)))
					return result;
			}
			
			for (child = n->children.tail ; child ; child = child->prev) {
				MWTree_node *c = child->data;
				
				if ((result = levelorder(t, c, converse, func, extra)))
					return result;
			}
		}
	
	return result;
}

/*******************************************************************************
  function to traverse a multi way tree levelorder
  
  Arguments:
				tree 		  the tree to traverse
        func  	  the function to pass each node to for processing
				extra		  extra data to pass to/from the proccessing function
	
	returns:
			the non null returned from the proccessing function that stops the
			traversal
			NULL if the end of the tree was reached
*******************************************************************************/

void *MWTree_levelorder (
  MWTree *tree,
  int converse,
  MWTree_traverse_function func,
  void *extra)
{
	
	if (!tree->root)
		return NULL;
	
	return levelorder(tree, tree->root, converse, func, extra);
}

/*******************************************************************************
	traverse function for delete all
*******************************************************************************/

void *delete_all_traverse(
	MWTree *tree,
	MWTree_node *node,
	void *data,
	void *extra)
{
	
	MWTree_delete(tree, node);
	
	tree->free(data);
	
	return NULL;
}

/*******************************************************************************
  function to delete all the nodes in a multi way tree

  args:
        tree        the tree to delete all the nodes in
          
  returns
        nothing
*******************************************************************************/

void MWTree_delete_all(
  MWTree *tree)
{
	
	MWTree_inorder(tree, 0, delete_all_traverse, NULL);
	
	return;
}

/*******************************************************************************
	traverse function for move
*******************************************************************************/

void *move_traverse(
	MWTree *tree,
	MWTree_node *node,
	void *data,
	void *extra)
{
	MWTree *dest = extra;
	
	if (!MWTree_insert(dest, data))
		return node;
	
	MWTree_delete(tree, node);
	
	return NULL;
}

	
/*******************************************************************************
  function to move a multi way tree to another multi way tree

  args:
        dest    the tree to move the nodes to
        src     the tree to move the nodes from
        branch  the node in the src tree to move

  returns:
        nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void MWTree_move (
  MWTree *dest,
  MWTree *src,
  MWTree_node *branch)
{
	MWTree temp;
	
	memcpy(&temp, src, sizeof(MWTree));
	
	if (branch)
		temp.root = branch;
				 
	MWTree_inorder(&temp, 0, move_traverse, dest);
	
}

/*******************************************************************************
	traverse function for copy
*******************************************************************************/

void *copy_traverse(
	MWTree *tree,
	MWTree_node *node,
	void *data,
	void *extra)
{
	MWTree *dest = extra;
	void *newdata;
	
	if (!tree->copy(&newdata, data))
		return node;
	
	if (!MWTree_insert(dest, newdata))
		return node;
	
	return NULL;
}

/*******************************************************************************
  function to copy a multi way tree to another multi way tree

  args:
        dest    the tree to copy the nodes to
        src     the tree to move the nodes from
        branch  the node in the src tree to copy

  returns:
        nothing

  notes:
        if branch is null the whole src tree is moved
*******************************************************************************/

void MWTree_copy (
  MWTree *dest,
  MWTree *src,
  MWTree_node *branch)
{

	MWTree temp;
	
	memcpy(&temp, src, sizeof(MWTree));
	
	if (branch)
		temp.root = branch;
				 
	MWTree_levelorder(&temp, 0, copy_traverse, dest);

}

