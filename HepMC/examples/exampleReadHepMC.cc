//////////////////////////////////////////////////////////////////////////
// Lynn Garren
//
// read file generated by examplePythiaWriteHepMC
//////////////////////////////////////////////////////////////////////////
// To Compile: go to the HepMC directory and type:
// gmake examples/example_EventSelection.exe
//

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/config/iostream.h"
#include "CLHEP/config/fstream.h"

#include "CLHEP/HepMC/ReadHepMC.h"
#include "CLHEP/HepMC/WriteHepMC.h"
#include "CLHEP/HepMC/GenEvent.h"

bool isGoodEvent( const HepMC::GenEvent* evt )
{
    // event selection returns true if the event contains
    // a photon with pT > 50 GeV
    for ( HepMC::GenEvent::particle_const_iterator p 
	      = evt->particles_begin(); p != evt->particles_end(); ++p ){
	if ( (*p)->pdg_id() == 22 && (*p)->momentum().perp() > 25. ) {
	    return true;
	}
    }
    return false;
}

int main() { 
    // open an input stream
    std::ifstream is( "examplePythiaWriteHepMC.dat" );
    // open an output stream
    std::ofstream os( "exampleReadHepMC.dat" );
    HepMC::writeLegend( os );
    //........................................EVENT LOOP
    int icount=0;
    int numGood=0;
    HepMC::GenEvent* evt = HepMC::findNextGenEvent( is );
    while ( evt ) {
	++icount;
	if ( icount%50==1 ) std::cout << "Processing Event " << icount
				      << " with Event Number " 
				      << evt->event_number() 
				      << std::endl;
	if ( isGoodEvent(evt) ) {
	    os << evt;
	    ++numGood;
	}
	delete evt;
        evt = HepMC::findNextGenEvent( is );
    }
    // flush the output stream
    HepMC::flushOutput( os );
    //........................................PRINT RESULT
    std::cout << numGood << " out of " << icount 
	      << " processed events passed the cuts. Finished." << std::endl;
    return 0;
}
