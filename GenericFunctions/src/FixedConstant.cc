// -*- C++ -*-
// $Id: FixedConstant.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
#include "GenericFunctions/defs.h"

#include "CLHEP/GenericFunctions/FixedConstant.hh"
#include <assert.h>

namespace Genfun {
FUNCTION_OBJECT_IMP(FixedConstant)

FixedConstant::FixedConstant(double value):
_value(value)
{
}

FixedConstant::FixedConstant(const FixedConstant & right):
_value(right._value) {
}

FixedConstant::~FixedConstant()
{
}

double FixedConstant::operator ()(double argument) const
{
  return _value;
}

Derivative FixedConstant::partial(unsigned int index) const {
  assert(index==0);
  FixedConstant fPrime(0.0);
  return Derivative(&fPrime);
}

} // namespace Genfun
