// $Id: RandomVector.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                        --- HepRandomVector ---
//                      class implementation file
// -----------------------------------------------------------------------
// =======================================================================
// Mark Fischler  - Created: 19 OCtober, 1998
// =======================================================================

#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/RandomObjects/RandomVector.h"
#include "RandomObjects/defs.h"


//------------------------- HepRandomVector ---------------------------------

HepRandomVector::HepRandomVector()
: theEngine(new HepJamesRandom(11327503L)), deleteEngine(true)
{
}

HepRandomVector::HepRandomVector(long seed)
: theEngine(new HepJamesRandom(seed)), deleteEngine(true) {
}

HepRandomVector::HepRandomVector(HepRandomEngine & engine)
: theEngine(&engine), deleteEngine(false) {
}

HepRandomVector::HepRandomVector(HepRandomEngine * engine)
: theEngine(engine), deleteEngine(true) {
}

HepRandomVector::~HepRandomVector() {
  if ( deleteEngine ) delete theEngine;
}

HepVector HepRandomVector::operator()() {
  return flat();
}

