// -*- C++ -*-
// $Id: ConstPlusFunction.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
#include "GenericFunctions/defs.h"

#include "CLHEP/GenericFunctions/ConstPlusFunction.hh"
#include <iostream>

namespace Genfun {
FUNCTION_OBJECT_IMP(ConstPlusFunction)

ConstPlusFunction::ConstPlusFunction(double constant, const AbsFunction *arg):
  _constant(constant),
  _arg(arg->clone())
{
}

ConstPlusFunction::ConstPlusFunction(const ConstPlusFunction & right) :
_constant(right._constant),
_arg(right._arg->clone())
{}

unsigned int ConstPlusFunction::dimensionality() const {
  return _arg->dimensionality();
}

ConstPlusFunction::~ConstPlusFunction()
{
  delete _arg;
}



double ConstPlusFunction::operator ()(double x) const
{
  return _constant + (*_arg)(x);
}


double ConstPlusFunction::operator ()(const Argument & x) const
{
  return _constant + (*_arg)(x);
}


Derivative ConstPlusFunction::partial(unsigned int index) const {
  return _arg->partial(index);
}


} // namespace Genfun
