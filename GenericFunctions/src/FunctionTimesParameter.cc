// -*- C++ -*-
// $Id: FunctionTimesParameter.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
#include "GenericFunctions/defs.h"

#include "CLHEP/GenericFunctions/FunctionTimesParameter.hh"
#include "CLHEP/GenericFunctions/Parameter.hh"
#include <iostream>

namespace Genfun {
FUNCTION_OBJECT_IMP(FunctionTimesParameter)

FunctionTimesParameter::FunctionTimesParameter(const AbsParameter *parameter, const AbsFunction *function):
  _function(function->clone()),
  _parameter(parameter->clone())
{
  if (parameter->parameter() && _parameter->parameter()) {
    _parameter->parameter()->connectFrom(parameter->parameter());
  }
}

FunctionTimesParameter::FunctionTimesParameter(const FunctionTimesParameter & right) :
  _function(right._function->clone()),
  _parameter(right._parameter->clone())
{}

unsigned int FunctionTimesParameter::dimensionality() const {
  return _function->dimensionality();
}

FunctionTimesParameter::~FunctionTimesParameter()
{
  delete _function;
  delete _parameter;
}


double FunctionTimesParameter::operator ()(double x) const
{
  return _parameter->getValue() * (*_function)(x);
}


double FunctionTimesParameter::operator ()(const Argument & x) const
{
  return _parameter->getValue() * (*_function)(x);
}




Derivative FunctionTimesParameter::partial(unsigned int index) const {
  const AbsFunction & fPrime  = (*_parameter)*(_function->partial(index));
  return Derivative(&fPrime);
}

} // namespace Genfun
