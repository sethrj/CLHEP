// $Id: MTwistEngine.h,v 1.1.1.1 2003/07/15 20:15:05 garren Exp $
// -*- C++ -*-
//
// -----------------------------------------------------------------------
//                             HEP Random
//                        --- MTwistEngine ---
//                          class header file
// -----------------------------------------------------------------------
// A "fast, compact, huge-period generator" based on M. Matsumoto and 
// T. Nishimura, "Mersenne Twister: A 623-dimensionally equidistributed 
// uniform pseudorandom number generator", to appear in ACM Trans. on
// Modeling and Computer Simulation.  It is a twisted GFSR generator
// with a Mersenne-prime period of 2^19937-1, uniform on open interval (0,1)
// For further information, see http://www.math.keio.ac.jp/~matumoto/emt.html
// =======================================================================
// Ken Smith      - Started initial draft: 14th Jul 1998
//                - Optimized to get pow() out of flat() method: 21st Jul
//                - Added conversion operators:  6th Aug 1998
// M Fischler	  - Changes in way powers of two are kept: 16-Sep-1998
// =======================================================================

#ifndef MTwistEngine_h
#define MTwistEngine_h

#include "CLHEP/Random/RandomEngine.h"

/**
 * @author
 * @ingroup random
 */
class MTwistEngine : public HepRandomEngine {

public:

  MTwistEngine();
  MTwistEngine( long seed );
  MTwistEngine( int rowIndex, int colIndex );
  MTwistEngine( std::istream & is );
  virtual ~MTwistEngine();
  // Constructors and destructor.

  MTwistEngine( const MTwistEngine & p );
  MTwistEngine & operator=( const MTwistEngine & p );
  // Copy constructor and operator=.

  double flat();
  // Returns a pseudo random number between 0 and 1 (excluding the end points).

  void flatArray(const int size, double* vect);
  // Fills an array "vect" of specified size with flat random values.

  void setSeed(long seed, int);
  // Sets the state of the algorithm according to seed.

  void setSeeds(const long * seeds, int);
  // Sets the state of the algorithm according to the zero terminated
  // array of seeds. It is allowed to ignore one or many seeds in this array.

  void saveStatus( const char filename[] = "MTwist.conf") const;
  // Saves the current engine status in the named file

  void restoreStatus( const char filename[] = "MTwist.conf" );
  // Reads from named file the the last saved engine status and restores it.

  void showStatus() const;
  // Dumps the current engine status on the screen.

  operator float();     // returns flat, without worrying about filling bits
  operator unsigned int(); // 32-bit flat, quickest of all

  friend std::ostream & operator<< (std::ostream & os, const MTwistEngine & e);
  friend std::istream & operator>> (std::istream & is,       MTwistEngine & e);

private:

  unsigned int mt[624];
  int count624;

  static double twoToMinus_32;
  static double twoToMinus_53;
  static double nearlyTwoToMinus_54;
  void powersOfTwo();

  enum{ NminusM = 227, M = 397, N = 624};
  static int numEngines;
  static int maxIndex;

}; // MTwistEngine

#endif // MTwistEngine_h
