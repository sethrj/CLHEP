//--------------------------------------------------------------------------
#ifndef HEPMC_GEN_PARTICLE_H
#define HEPMC_GEN_PARTICLE_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// particle within an event coming in/out of a vertex
// particle is the basic building block or unit of the event record
//////////////////////////////////////////////////////////////////////////
//
// example:
//      GenParticle* p = new GenParticle( CLHEP::HepLorentzVector(1,1,1,3), 11, 1 );
// creates a particle with 4-vector (p,E)=1,1,1,3 - with pdg id 11 (electron)
// and give this particle status =1.
//
// The definition of a CLHEP::HepLorentzVector scope resolution operator allows for
//  the use of 4 vector algebra with GenParticles.
//  i.e. if two particles are defined:
//   HepMC::GenParticle p_electron( CLHEP::HepLorentzVector(0,0,5,5), 11, 1 );
//   HepMC::GenParticle p_positron( CLHEP::HepLorentzVector(0,5,0,5), -11, 1 );
//  then you can find their cms 4 vector:
//   CLHEP::HepLorentzVector v_cms = (CLHEP::HepLorentzVector)p_electron 
//                            + (CLHEP::HepLorentzVector)p_positron;
//
// the pointers to end/production vertices can only be set by the
//  vertices themselves - thus to set the production vertex for a particle,
//  you add the particle to that vertex with GenVertex::add_particle_out()
//
// We decide not to have a separate 4 vector for the momentum 
//  at decay time (which MC++ includes to allow dE/dX losses etc). 
//  If you want that, just add a decay vertex with the
//  same particle (modified momentum) going out
//

#include "CLHEP/HepMC/Flow.h"
#include "CLHEP/HepMC/Polarization.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/HepMC/HepMCConfig.h"
#include "CLHEP/HepPDT/ParticleID.hh"

#include <vector>
#include <iostream>

namespace HepMC {

    class GenVertex;
    class GenEvent; 

    /**
     * @author
     * @ingroup hepmc
     */
    class GenParticle {

	friend class GenVertex; // so vertex can set decay/production vertexes
	friend class GenEvent;  // so event can set the barCodes
	friend std::ostream& operator<<( std::ostream&, const GenParticle& );

    public:

        typedef  HepMCConfig::ParticleData   ParticleData;
        typedef  HepMCConfig::DecayData      DecayData;

        GenParticle(void);
	GenParticle( const CLHEP::HepLorentzVector& momentum, int pdg_id,
		     int status = 0, const Flow& itsflow = Flow(),
		     const Polarization& polar = Polarization(0,0) );
	GenParticle( const CLHEP::HepLorentzVector& momentum, 
		     ParticleData * pd, DecayData * dd = 0,
		     int status = 0, const Flow& itsflow = Flow(),
		     const Polarization& polar = Polarization(0,0) );
	GenParticle( const GenParticle& inparticle ); // shallow copy.
	virtual ~GenParticle();
	
	GenParticle& operator=( const GenParticle& inparticle ); // shallow.
	bool         operator==( const GenParticle& ) const;
	bool         operator!=( const GenParticle& ) const;

	// dump this particle's full info to ostr
	void       print( std::ostream& ostr = std::cout ) const; 

	operator CLHEP::HepLorentzVector() const; // conversion operator

	////////////////////
	// access methods //
	////////////////////

	CLHEP::HepLorentzVector     momentum() const;
	int                  pdg_id() const;
	int                  status() const;
	Flow                 flow() const;
	int                  flow( int code_index ) const;
        double               generatedMass() const;
        // You can ask for 3 masses:  ParticleData->mass()        [nominal mass]
        //                            GenParticle.generatedMass() [generated mass]
        //                            GenParticle.momentum().m()  [on shell mass]
	//
	Polarization         polarization() const;
	GenVertex*           production_vertex() const;
	GenVertex*           end_vertex() const;
	GenEvent*            parent_event() const;

        HepPDT::ParticleID   particleID()   const;
	ParticleData const & particledata() const { return *itsParticleData; }
	ParticleData       & particledata()       { return *itsParticleData; }
	DecayData    const & decaydata()    const { return *itsDecayData; }
	DecayData          & decaydata()          { return *itsDecayData; }
	
	// get pointers to mothers and daughters
	GenParticle *      mother()         const;
	GenParticle *      secondMother()   const;
	GenParticle *      beginDaughters() const;
	GenParticle *      endDaughters()   const;
	// get vectors of pointers to particles
	std::vector<GenParticle*> listChildren() const;
	std::vector<GenParticle*> listDescendants() const;
	std::vector<GenParticle*> listParents() const;
	std::vector<GenParticle*> listAncestors() const;
	// more utility methods
	bool hasParents() const;
	bool hasChildren() const;
	int  numParents() const;
	int  numChildren() const;
	
	// StdHep information
	int                collisionNumber() const;
	CLHEP::HepLorentzVector   creationVertex() const;	
	CLHEP::HepLorentzVector   decayVertex()    const;	

	//
	// The barcode is the particle's reference number, every vertex in the
	//  event has a unique barcode. Particle barcodes are positive numbers,
	//  vertex barcodes are negative numbers.
	// In general there is no reason to "suggest_barcode", if a particle is
	//  added to the event without a suggested barcode, the event will
	//  assign one for it.
	int                  barcode() const;
	bool                 suggest_barcode( int the_bar_code );

	////////////////////
	// mutators       //
	////////////////////
	
	void   set_momentum( const CLHEP::HepLorentzVector& vec4 );
	void   set_pdg_id( int id );
	void   set_status( int status = 0 );
	void   set_flow( const Flow& f );
	void   set_flow( int code_index, int code );
	void   set_polarization( const Polarization& polarization =
				 Polarization(0,0) );
        void   setGeneratedMass( const double & m );
        //  If you do not call setGeneratedMass(), then 
        //  generatedMass() will simply return the mass calculated from
        //  momentum()

	// StdHep mutators
	void setParticleID      ( HepPDT::ParticleID pid );
	void setCollisionNumber ( int coll );
	void setMomentum        ( double px, double py, double pz, double E );
	void changeParticleType (ParticleData & pd) { itsParticleData = &pd; }
	void changeDecayData    (DecayData & dd)    { itsDecayData = &dd; }

	// old StdHepC++ methods
	// ---  accessors:
	//
	int               ParticleID()      const { return pdg_id(); }
	int               StatusCode()      const { return status(); }
	int               Mother()          const;
	int               SecondMother()    const;
	int               BeginDaughters()  const;
	int               EndDaughters()    const;
	int               CollisionNumber() const { return collisionNumber(); }
	CLHEP::HepLorentzVector  Momentum()        const { return momentum(); }
	double            Mass()            const { return generatedMass(); }
	CLHEP::HepLorentzVector  CreationVertex()  const { return creationVertex(); }
	CLHEP::HepLorentzVector  DecayVertex()     const { return decayVertex(); }
	// ---  mutators:
	//
	void  setStatusCode     ( int status )  { set_status(status); }

    protected: // for internal use only by friend GenVertex class

	static unsigned int counter(); // temporary for debugging

	void   set_production_vertex_( GenVertex* productionvertex = 0);
	void   set_end_vertex_( GenVertex* decayvertex = 0 );
	void   set_barcode_( int the_bar_code ); // for use by GenEvent only

    private:
	CLHEP::HepLorentzVector m_momentum;          // 4 vector
	int              m_pdg_id;            // id according to PDG convention
	int              m_status;            // As defined for HEPEVT
	Flow             m_flow;
	Polarization     m_polarization;
	GenVertex*       m_production_vertex; // null if vacuum or beam
	GenVertex*       m_end_vertex;        // null if not-decayed
	int              m_barcode;           // unique identifier in the event
        double           itsGeneratedMass;    // 
        int              itsCollisionNumber;  // for StdHep
	ParticleData *   itsParticleData;	// should be null only if no 
	                                        //    particle data is desired
	DecayData *      itsDecayData; 		// null if not customized

	static unsigned int s_counter;
    };  

    //////////////
    // INLINES  //
    //////////////
    inline GenParticle::operator CLHEP::HepLorentzVector() const 
    { return m_momentum; }

    inline CLHEP::HepLorentzVector GenParticle::momentum() const 
    { return m_momentum; }

    inline int GenParticle::pdg_id() const { return m_pdg_id; }

    inline int GenParticle::status() const { return m_status; }

    inline GenVertex* GenParticle::production_vertex() const 
    { return m_production_vertex; }

    inline GenVertex* GenParticle::end_vertex() const { return m_end_vertex; }

    inline Flow GenParticle::flow() const { return m_flow; }

    inline int GenParticle::flow( int code_index ) const
    { return m_flow.icode( code_index ); }

    inline Polarization GenParticle::polarization() const 
    { return m_polarization; }

    inline void GenParticle::set_momentum( const CLHEP::HepLorentzVector& vec4 )
    { m_momentum = vec4; }

    inline void GenParticle::set_pdg_id( int id ) { m_pdg_id = id; }

    inline void GenParticle::set_status( int status ) { m_status = status; }

    inline void GenParticle::set_flow( const Flow& f ) { m_flow = f; }

    inline void GenParticle::set_flow( int code_index, int code ) 
    {  m_flow.set_icode( code_index, code ); }

    inline void GenParticle::set_polarization( const Polarization& polar )
    { m_polarization = polar; }

    inline int  GenParticle::barcode() const { return m_barcode; }

    inline void GenParticle::set_barcode_( int bc ) { m_barcode = bc; }

} // HepMC

#endif  // HEPMC_GEN_PARTICLE_H
//--------------------------------------------------------------------------

