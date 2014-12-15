/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#ifndef _TIO_STACK_INT_H
#define _TIO_STACK_INT_H


/* -- Have written stack routines generically for good software engineering purposes */


struct iStack_t {
  void *base;
  void *top;
};


extern int iTIO_StackInit    ( struct iStack_t *stack              );
extern int iTIO_StackPush    ( struct iStack_t *stack, void  *data );
extern int iTIO_StackPop     ( struct iStack_t *stack, void **data );
extern int iTIO_IsStackAlloc ( struct iStack_t *stack              );

#endif

/*
 * EOF
 */
