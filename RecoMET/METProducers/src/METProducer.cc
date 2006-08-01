// File: METProducer.cc
// Description:  see METProducer.h
// Author: R. Cavanaugh, The University of Florida
// Creation Date:  20.04.2006.
//
//--------------------------------------------
#include <memory>
#include "RecoMET/METProducers/interface/METProducer.h"
#include "RecoMET/METAlgorithms/interface/CaloSpecificAlgo.h"
//#include "DataFormats/METObjects/interface/METCollection.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
//#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/CommonMETData.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace edm;
using namespace std;
using namespace reco;

namespace cms 
{
  //--------------------------------------------------------------------------
  // Constructor : used to fill the parameters from the configuration file
  // Currently there are only two defined parameters:
  // 1. src = the label of the input data product (which must derive from 
  //    Candidate)
  // 2. METType = the type of to produce into the event.  currently there are
  //    only two types of MET defined: (1) MET from calorimetery (and so 
  //    contains extra information specific to calorimetery) and (2) the 
  //    default MET which contains only generic information.  Additional
  //    MET types will appear (such as GenMET) in the future.  (All "types"
  //    of MET inherit from RecoCandidate and merely extend that class with
  //    extra information)
  //-----------------------------------
  METProducer::METProducer(const edm::ParameterSet& iConfig) : alg_() 
  {
    inputLabel = iConfig.getParameter<std::string>("src");
    METtype    = iConfig.getParameter<std::string>("METType");
    std::cout << "MET Type = " << METtype << std::endl;
    if(      METtype == "CaloMET" ) produces<CaloMETCollection>(); 
    else if( METtype == "GenMET" )  /*produces<GenMETCollection>()*/; 
    else                            produces<METCollection>();
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // Default Constructor
  //-----------------------------------
  METProducer::METProducer() : alg_() 
  {
    produces<METCollection>(); 
  }
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // Default Destructor
  //-----------------------------------
  METProducer::~METProducer() {}
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  //-----------------------------------
  void METProducer::produce(Event& event, const EventSetup& setup) 
  {
    //-----------------------------------
    // Step A: Get Inputs.  Create an empty collection of candidates
    edm::Handle<CandidateCollection> inputs;
    // Now, fill the collection of candidates with the event product defined
    // by its product label via the config parameter "src"
    event.getByLabel( inputLabel, inputs );
    // create a vector of pointers to candidates (for economy) and reserve
    // enough space for all candidates in the above collection
    vector <const Candidate*> input;
    input.reserve( inputs->size() );
    // Now, fill the vector of points by loops over all the candidates and 
    // assigning each element a pointer the corresponding candidate
    CandidateCollection::const_iterator input_object = inputs->begin();
    for( ; input_object != inputs->end(); input_object++ )
      input.push_back( &*input_object );
    //-----------------------------------
    // Step B: Create an empty MET struct output.
    CommonMETData output;
    //-----------------------------------
    // Step C: Invoke the MET algorithm, which runs on any candidate input. 
    alg_.run(input, &output);  
    //-----------------------------------
    // Step D: Invoke the specific "afterburner", which adds information
    //         depending on the input type, given via the config parameter.
    //         Also, after the specific algorithm has been called, store
    //         the output into the Event.
    if( METtype == "CaloMET" ) 
    {
      CaloSpecificAlgo calo;
      std::auto_ptr<CaloMETCollection> calometcoll; 
      calometcoll.reset(new CaloMETCollection);
      calometcoll->push_back( calo.addInfo(input, output) );
      event.put( calometcoll );
    }
    //-----------------------------------
    else if( METtype == "GenMET" ) 
    {
    /*
      std::auto_ptr<GenMETCollection> GenMET;
      GenMET.reset (new GenMETCollection);
      GenSpecificInfo gen;
      GenMET->push_back( gen.addInfo(output) );
      event.put(GenMET);
    */
    }
    //-----------------------------------
    else
    {
      LorentzVector p4( output.mex, output.mey, 0.0, output.met);
      Point vtx(0,0,0);
      MET met( output.sumet, p4, vtx );
      std::auto_ptr<METCollection> metcoll;
      metcoll.reset(new METCollection);
      metcoll->push_back( met );
      event.put( metcoll );
    }
    //-----------------------------------
  }
  //--------------------------------------------------------------------------
}
