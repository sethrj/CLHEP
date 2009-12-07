// ----------------------------------------------------------------------
//
// testBug58950 -- test problem with RanecuEngine on 64bit machines
//
// R. Weller	    11/11/09	initial test from Vanderbilt
// L. Garren	    12/1/09	rewritten for test suite
// 
// ----------------------------------------------------------------------
#include <iostream> 
#include <stdexcept>
#include <cmath>
#include <stdint.h>
#include "CLHEP/Random/RanecuEngine.h"
#include "CLHEP/Random/Random.h"

bool printCheck( int & i, double & r, std::ofstream & os )
{
    os << i << " " << r << std::endl; 
    if (r < 0 || r > 1.0 ) {
	std::cout << "Error: bad random number " << r << std::endl; 
	return false;
    }
    return true;
}

int main() { 

    std::ofstream output("testBug58950.cout");  

    output << std::endl << "short " << sizeof(short) << std::endl; 
    output << "int " << sizeof(int) << std::endl; 
    output << "unsigned int " << sizeof(unsigned int) << std::endl; 
    output << "long " << sizeof(long) << std::endl; 
    output << "float " << sizeof(float) << std::endl; 
    output << "double " << sizeof(double) << std::endl; 
    output << "long double " << sizeof(long double) << std::endl << std::endl; 

    CLHEP::RanecuEngine *eng = new CLHEP::RanecuEngine;
    CLHEP::HepRandom::setTheEngine(eng);
    CLHEP::HepRandom *g;
    g=CLHEP::HepRandom::getTheGenerator();

    long rvals[2];
    try {
	std::ifstream in("/dev/urandom", std::ios::in | std::ios::binary);
	if(in.is_open()) {
		in.read((char *)(&rvals), 2*sizeof(long));
		in.close();
		if(in.fail()) {
			throw std::runtime_error("File read error");
		}
	} else throw std::runtime_error("File open error");
    } catch(std::runtime_error e) {
	std::ostringstream dStr;
	dStr << "Error: " << e.what() 
	<< " processing seed from file \"" << "/dev/urandom" << "\"."; 
	throw std::runtime_error(dStr.str().c_str());
    }

    int nNumbers = 20; 
    int badcount = 0;

    long seeds[3];
    const long *pseeds;
    //***********************************************************************
    // Seeds are supposed to be positive.  Therefore, if either seed
    // is negative then the generator sets initial conditions and generates
    // the same sequence of numbers no matter what the seeds are.  
    seeds[0]=rvals[0];
    seeds[1]=rvals[1];
    seeds[2]=0;
    if( rvals[0] > 0 ) seeds[0] = -rvals[0];
    
    double negseq[20] = { 0.154707, 0.587114, 0.702059, 0.566, 0.988325,
                          0.525921, 0.191554, 0.269338, 0.234277, 0.358997,
			  0.549936, 0.296877, 0.162243, 0.227732, 0.528862,
			  0.631571, 0.176462, 0.247858, 0.170025, 0.284483 };
    double eps =  1.0E-6;

    output << std::endl << "********************" << std::endl;
    output << "This is the case that may or may not fail." << std::endl;
    output << "However, if it has values in (0,1), they are a " << std::endl
			    << "deterministic sequence beginning with 0.154707." << std::endl;
    output << "seeds[0] = " << seeds[0] << "\n" 
			    << "seeds[1] = " << seeds[1] << std::endl << std::endl;

    g->setTheSeeds(seeds);
    int rseq = 0;
    for (int i=0; i < nNumbers; ++i) { 
	double r = g->flat(); 
	if( ! printCheck(i,r,output) ) ++badcount;
	// before the change, the random number sequence was reliably the same
	if( std::fabs(r-negseq[i]) < eps ) {
	    std::cout << " reproducing sequence " << i << " "
	              << r << " " << negseq[i] << std::endl;
	    ++rseq;
	}
    }
    if( rseq == 20 ) ++badcount;
    pseeds=g->getTheSeeds();
    output << "Final seeds[0] = " << pseeds[0] << "\n" 
			    << "Final seeds[1] = " << pseeds[1] << std::endl << std::endl;

    //***********************************************************************
    // Forcing these seeds to positive values reliably crashes the generator.
    seeds[0]=std::abs(rvals[0]);
    seeds[1]=std::abs(rvals[1]);
    seeds[2]=0;

    output << std::endl << "********************" << std::endl;
    output << "This is the case that always fails." << std::endl;
    output << "seeds[0] = " << seeds[0] << "\n" 
			    << "seeds[1] = " << seeds[1] << std::endl << std::endl;

    g->setTheSeeds(seeds);
    for (int i=0; i < nNumbers; ++i) { 
	double r = g->flat(); 
	if( ! printCheck(i,r,output) ) ++badcount;
    }
    pseeds=g->getTheSeeds();
    output << "Final seeds[0] = " << pseeds[0] << "\n" 
			    << "Final seeds[1] = " << pseeds[1] << std::endl << std::endl;

    //***********************************************************************
    // 4-byte positive integers generate valid sequences, which appear to remain
    // within bounds.
    seeds[0]= std::abs(static_cast<int>(rvals[0]));
    seeds[1]= std::abs(static_cast<int>(rvals[1]));
    seeds[2]=0;

    output << std::endl << "********************" << std::endl;
    output << "This is the case that works." << std::endl;
    output << std::endl << "seeds[0] = " << seeds[0] << "\n" 
			    << "seeds[1] = " << seeds[1] << "\n"
			    << "seeds[2] = " << seeds[2] << std::endl << std::endl;

    g->setTheSeeds(seeds);
    for (int i=0; i < nNumbers; ++i) { 
	double r = g->flat(); 
	if( ! printCheck(i,r,output) ) ++badcount;
    } 
    pseeds=g->getTheSeeds();
    output << "Final seeds[0] = " << pseeds[0] << "\n" 
			    << "Final seeds[1] = " << pseeds[1] << std::endl << std::endl;

    //***********************************************************************
    // This tests the conjecture that eventually a bad sequence will rectify
    // itself. This starts with the seeds that always fail and loops until both
    // seeds are in the range of positive 32-bit integers. It appears from
    // my early tests that my conjecture of a stable correct region that
    // the generator falls into may be correct.
    seeds[0]=std::abs(rvals[0]);
    seeds[1]=std::abs(rvals[1]);
    seeds[2]=0;

    output << std::endl << "********************" << std::endl;
    output << "This case loops until it valid short seeds occur." << std::endl;
    output << "seeds[0] = " << seeds[0] << "\n" 
			    << "seeds[1] = " << seeds[1] << std::endl << std::endl;

    g->setTheSeeds(seeds);
    // Loop as long as the values are bad.
    double r;
    unsigned int low = ~0;
    unsigned long mask = (~0) << 31;
    unsigned long skipcount = 0;
    output << "low = " << low << "  mask = " << mask << std::endl;
    do {r = g->flat(); pseeds = g->getTheSeeds(); ++skipcount;} 
	    while((pseeds[0]&mask) || (pseeds[1]&mask));

    output << std::endl << "Loop terminates on two short seeds." << std::endl;
    output << "Skipcount = " << skipcount << std::endl;
    output << "pseeds[0]&mask = " << (pseeds[0]&mask) << std::endl;
    output << "pseeds[1]&mask = " << (pseeds[1]&mask) << std::endl;
    output << "Final seeds[0] = " << pseeds[0] << "\n" 
			    << "Final seeds[1] = " << pseeds[1] << std::endl << std::endl;

    output << "This should be a valid sequence." << std::endl;	
    for (int i=0; i < nNumbers; ++i) { 
	double r = g->flat(); 
	if( ! printCheck(i,r,output) ) ++badcount;
    }
    pseeds=g->getTheSeeds();
    output << "seeds[0] = " << pseeds[0] << "\n" 
			    << "seeds[1] = " << pseeds[1] << std::endl << std::endl;

    if( badcount > 0 ) std::cout << "Found " << badcount << " bad seeds" << std::endl;
    return badcount; 
} 
