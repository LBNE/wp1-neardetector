// -------------------------------------------------------------------
/// \file   TestIOGenieRec.cxx
/// \brief  
///
/// \author  <justo.martin-albo@physics.ox.ac.uk>
/// \date    Creation: 2 Aug 2016
// -------------------------------------------------------------------

#include "RootFileReader.h"
#include "EventRecord.h"
#include "MCGenInfo.h"
#include "MCParticle.h"

#include <Ntuple/NtpMCEventRecord.h>
#include <EVGCore/EventRecord.h>

#include <TFile.h>
#include <TTree.h>

#include <iostream>



void PrintUsage()
{
  std::cerr << "Usage: TestIOGenieRec <input_file>" << std::endl;
  std::exit(EXIT_FAILURE);
}


bool ReadGenieRecord(gastpc::MCGenInfo* mcgi)
{
  genie::NtpMCEventRecord* gmcrec = mcgi->GetGeneratorRecord();
  std::cout << *gmcrec << std::endl;

  //genie::Interaction* interaction = (gmcrec->event)->Summary();
  //const genie::Target& tgt = interaction->InitState().Tgt();
  //std::cout << "Interaction in Z = " << 

  //if (tgt.Z() != 18) continue;

  return true;
}


int main(int argc, char* argv[])
{
  if (argc < 2) PrintUsage();

  std::string input_filename(argv[1]);

  gastpc::RootFileReader r;
  r.OpenFile(input_filename);

  for (int i=0; i<r.GetNumberOfEntries(); ++i) {

    gastpc::EventRecord& evtrec = r.Read(i);

    for (gastpc::MCGenInfo* mcgi: evtrec.GetMCGenInfo()) {
      ReadGenieRecord(mcgi);
   } 
  }

  return EXIT_SUCCESS;
}
