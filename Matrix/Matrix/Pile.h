// -*- C++ -*-
// CLASSDOC OFF
// $Id: Pile.h,v 1.2 2003/07/18 05:31:48 garren Exp $
// ---------------------------------------------------------------------------
// CLASSDOC ON
//
// This file is a part of the CLHEP - a Class Library for High Energy Physics.
//
// 
// Copyright Cornell University 1993, 1996, All Rights Reserved.
// 
// This software written by Nobu Katayama and Mike Smyth, Cornell University.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright
//    notice and author attribution, this list of conditions and the
//    following disclaimer in the documentation and/or other materials
//    provided with the distribution.
// 3. Neither the name of the University nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// Creation of derivative forms of this software for commercial
// utilization may be subject to restriction; written permission may be
// obtained from Cornell University.
// 
// CORNELL MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.  By way
// of example, but not limitation, CORNELL MAKES NO REPRESENTATIONS OR
// WARRANTIES OF MERCANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT
// THE USE OF THIS SOFTWARE OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS,
// COPYRIGHTS, TRADEMARKS, OR OTHER RIGHTS.  Cornell University shall not be
// held liable for any liability with respect to any claim by the user or any
// other party arising from use of the program.
//

//    This file contains an attempt to make the template "pile".  A pile is 
//    a finite size LIFO stack.  When a element is pushed on that increases
//    the stack beyond its maximum size, the oldest element is deleted from 
//    the stack.  A subroutine can be used on that oldest element first.

//    The orginal use of this stack was to store old double arrays.  When
//    a new array is needed, we can simply pull one off the pile.  However,
//    we don't want to keep too many old array's around, after a while we just
//    want to start getting rid of them.  When the pile gets too large, or
//    when the pile is destroyed, we want to call subroutines to get rid of
//    some of these arrays.

//    Unfortunately, in version 2.2 of g++ templates don't seem to work unless
//    they are declared inline.  So this class has ridiculously long inline
//    functions.  Also, g++ doesn't seem to allow multiple arguements to 
//    templates, so the size of the pile is hardwired in.  To change the size,
//    change the value of the const int sz.

//    A pile is easy to use.  Just declare pile<X> X_pile.  To add a X to the
//    pile, say X_pile.push(X item).  To get an item from the pile, first 
//    check that the pile is not empty, and then say item=X_pile.pop().  It
//    is an error to try and pop from an empty pile.  To check if a pile is
//    empty, say X_pile.is_empty().  If this is TRUE, then the pile is empty.
//    Otherwise it is FALSE.  The subroutine called when the stack begins to
//    overflow is set by X_pile.destroy(void (*function)(X)), or it can be
//    set in the construction pile<X> X_pile(void (*function)(X)).  It is
//    okay to not supply a function, in that case nothing is done when an
//    item falls off the bottom of the pile.  It is simply lost.

#ifndef _PILE_H
#define _PILE_H

#include <iostream>

/**
 * @author
 * @ingroup matrix
 */

namespace CLHEP {

template<class T> 
class HepPile 
{
public:
   // Destructor
   // (defined first in templated class due to a bug in VxWorks)
   ~HepPile()
   {
      while(bottom != top)
      {
#if 1
	 destroy(stack[bottom]);
#else
	 delete [] stack[bottom];
#endif
	 next(&bottom);
      }
   }

   HepPile(void (*f)(T)=0): top(0), bottom(0) { destroy_fun = f;}

   void set_destroy(void (*f)(T)) { destroy_fun = f;}
   void push(T item)
   {
      stack[top]=item;
      next(&top);
      if (top==bottom)
      {
#if 1
	 destroy(stack[bottom]);
#else
	 delete [] stack[bottom];
#endif
	 next(&bottom);
      }
   }
   bool is_empty() const { return top == bottom ?true :false;}
   T pop()
   {
      if (is_empty())
      {
	 std::cerr << "Attempt to pop empty pile.\n--- Exiting to system."
		   << std::endl;
	 exit(1);
      }
      previous(&top);
      return stack[top];
   }
   
private:
   enum {sz = 50};
   T stack[sz+1];
   int top,bottom;
   void (*destroy_fun)(T);
   void next(int *n) const {if (++(*n) >= sz+1) *n = 0;}
   void previous(int *n) const {if (--(*n) < 0) *n = sz;}
   void destroy(T t) { if (destroy_fun) (*destroy_fun)(t); }
};

}  // namespace CLHEP

#endif /*_PILE_H */
