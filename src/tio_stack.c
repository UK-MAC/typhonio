/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include "tio_global_int.h"

#include "tio_stack_int.h"



/* Can't use TIOreturn to return-on-error as Stack is used by error handler */


struct xNode_t {
  void           *data;
  struct xNode_t *next, *prev;
};




/***************************************************************************************************
 *
 * iTIO_StackInit
 *
 **************************************************************************************************/
int
iTIO_StackInit( struct iStack_t *stack )
{
  stack->base = NULL;
  stack->top  = NULL;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_StackPush
 *
 **************************************************************************************************/
int
iTIO_StackPush( struct iStack_t *stack,
                void            *data )
{
  /* Returns -ve if fails to malloc stack node */

  const char *subname = "iTIO_StackPush";   /* Function name for debugging and error */

  struct xNode_t *node;



  node = NULL;

  if (NULL == stack->base) {

    stack->base = TIOmalloc(sizeof(struct xNode_t));
    if (NULL == stack->base) {
      return (-1);
    }
    node       = stack->base;
    node->prev = NULL;
  }
  else {

    node = (struct xNode_t *)TIOmalloc(sizeof(struct xNode_t));
    if (NULL == node) {
      return (-2);
    }
    node->prev = stack->top;
  }

  stack->top = node;

  /* -- Point the data pointer at the top of the stack at the given data */
  node->data = data;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_StackPop
 *
 **************************************************************************************************/
int
iTIO_StackPop( struct iStack_t *stack,
               void            **data )
{
  /* Retuns -ve if stack is popped when empty */

  const char *subname = "iTIO_StackPop";   /* Function name for debugging and error */

  struct xNode_t *node, *prev;


  /* -- Check that the stack or pointer to the top of it aren't null */
  if ( (NULL == stack) || (NULL == stack->base) ) {
    data = NULL;
    return (-1);
  }

  /* -- Return a pointer to the data at the top of the stack */
  node  = stack->top;
  *data = node->data;

  /* -- Free the top stack node */
  prev = node->prev;
  TIOfree (node); node = NULL;

  if (NULL == prev) {
    stack->base = NULL;
  }
  stack->top = prev;


  return (0);
}




/***************************************************************************************************
 *
 * iTIO_IsStackAlloc
 *
 **************************************************************************************************/
int iTIO_IsStackAlloc( struct iStack_t *stack )
{
  /* Returns TRUE if given stack is allocated */

  if (stack != NULL) {
    if (stack->base != NULL) {
      return (TRUE);
    }
  }

  return (FALSE);
}



/*
 * EOF
 */
