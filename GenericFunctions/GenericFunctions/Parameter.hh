// -*- C++ -*-
// $Id: Parameter.hh,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
//-----------------------Class Parameter------------------------------------//
//                                                                          //
//  Joe Boudreau                                                            //
//  Petar Maksimovic                                                        //
//  November 1999                                                           //
//                                                                          //
//  This class is a simple low-level double precision number, together with //
//  some limiting values. It is designed essentially as an ingredient for   //
//  building function objects.                                              //
//                                                                          //
//  Parameters can be connnected to one another. If a parameter is          //
//  connected, it takes is value (and limits) from the parameter to which   //
//  it is connected.                                                        //
//  When disconnected, it captures the values of the connected field before //
//  dropping the connection.  An attempt to alter the values of the field   //
//  while the Parameter is connected to another parameter will result in    //
//  an obnoxious warning mesage.                                            //
//                                                                          //
//--------------------------------------------------------------------------//
#ifndef Parameter_h
#define Parameter_h 1

#include <iostream>
#include <string>
#include "CLHEP/GenericFunctions/AbsParameter.hh"

namespace Genfun {

  /**
   * @author
   * @ingroup genfun
   */
  class Parameter:public AbsParameter {

    PARAMETER_OBJECT_DEF(Parameter)
  
      public:
  
    // Constructor.
    Parameter(std::string name,
	      double value, 
	      double lowerLimit=-1e100,
	      double upperLimit= 1e100);
  
    // Copy constructor
    Parameter(const Parameter & right);
  
    // Destructor
    virtual ~Parameter();

    // Assignment
    const Parameter & operator=(const Parameter &right);
  
    // Accessor for the Parameter name
    const std::string & getName() const;

    // Accessor for value
    virtual double getValue() const;
  
    // Accessor for Lower Limit
    double getLowerLimit() const;
  
    // Accessor for Upper Limit
    double getUpperLimit() const;
  
    // Set Value
    void setValue(double value);
  
    // Set Lower Limit
    void setLowerLimit(double lowerLimit);
  
    // Set Upper Limit
    void setUpperLimit(double upperLimit);
  
    // Take values + limits from some other parameter.
    void connectFrom(const AbsParameter *  source);
  
    // Extra lingual type information:
    virtual Parameter *parameter() {return this;}
    virtual const Parameter *parameter() const {return this;}
  
  private:

    std::string        _name ;                // name
    double                _value;                // value
    double                _lowerLimit;           // lower limit
    double                _upperLimit;           // upper limit
    const AbsParameter   *_sourceParameter;      // connection
  
  };
std::ostream & operator << ( std::ostream & o, const Parameter &p);
} // namespace Genfun

#endif
