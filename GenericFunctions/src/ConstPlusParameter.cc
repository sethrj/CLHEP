// -*- C++ -*-
// $Id: ConstPlusParameter.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
#include "GenericFunctions/defs.h"

#include "CLHEP/GenericFunctions/ConstPlusParameter.hh"
#include "CLHEP/GenericFunctions/Parameter.hh"

namespace Genfun {
PARAMETER_OBJECT_IMP(ConstPlusParameter)

ConstPlusParameter::ConstPlusParameter(double constant, const AbsParameter *parameter):
  _constant(constant),
  _parameter(parameter->clone())
{
  if (parameter->parameter() && _parameter->parameter()) _parameter->parameter()->connectFrom(parameter->parameter());
}

ConstPlusParameter::ConstPlusParameter(const ConstPlusParameter & right) :
_constant(right._constant),
_parameter(right._parameter->clone())
{}


ConstPlusParameter::~ConstPlusParameter()
{
  delete _parameter;
}


double ConstPlusParameter::getValue() const {
  return _constant + _parameter->getValue();
}

} // namespace Genfun
