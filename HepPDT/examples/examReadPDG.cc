// $Id: examReadPDG.cc,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
// ----------------------------------------------------------------------
// examReadPDG.cc
//
// read PDG table and write it out
//
// ----------------------------------------------------------------------

#include <fstream>

#include "CLHEP/HepPDT/DefaultConfig.hh"
#include "CLHEP/HepPDT/TableBuilder.hh"
#include "CLHEP/HepPDT/ParticleDataTableT.hh"

#ifdef __osf__
// kludge so linker can find HepPDT::DMFactory<DefaultConfig>::_inst
template class HepPDT::DMFactory<DefaultConfig>;
#endif

int main()
{
    const char pdgfile[] = "../data/pdg_mass.tbl";
    const char outfile[] = "examReadPDG.out";
    // open input file
    std::ifstream pdfile( pdgfile );
    if( !pdfile ) { 
      std::cerr << "cannot open " << pdgfile << std::endl;
      exit(-1);
    }
    // construct empty PDT
    DefaultConfig::ParticleDataTable datacol( "PDG Table" );
    {
        // Construct table builder
        HepPDT::TableBuilder  tb(datacol);
	// read the input - put as many here as you want
	if( !addPDGParticles( pdfile, tb ) ) { std::cout << "error reading PDG file " << std::endl; }
    }	// the tb destructor fills datacol
    std::ofstream wpdfile( outfile );
    if( !wpdfile ) { 
      std::cerr << "cannot open " << outfile << std::endl;
      exit(-1);
    }
    datacol.writeParticleData(wpdfile);
}
