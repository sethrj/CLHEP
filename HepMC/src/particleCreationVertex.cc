// $Id: particleCreationVertex.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
// ----------------------------------------------------------------------
//
// particleCreationVertex.cc
//
// ----------------------------------------------------------------------

#include "CLHEP/HepMC/GenParticle.h"
#include "CLHEP/HepMC/GenVertex.h"

namespace HepMC  {

CLHEP::HepLorentzVector   GenParticle::creationVertex() const	
{
   // empty if there is no vertex
   CLHEP::HepLorentzVector cv(0,0,0,0);
   if( m_production_vertex ) {
       cv = m_production_vertex->position();
   }
   return cv;
}

}  // HepMC
