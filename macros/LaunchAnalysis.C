// LaunchAnalysis.C
// Launch the analysis on a sample of 100000 events 
//
// Simulation parameters:
// kUnifMult -> uniform multiplicity
// kHistMult -> multiplicity from histogram
// kUnifEta -> uniform pseudorapidity
// kHistEta -> pseudorapidity from histogram
// kOnScat/kOffScat -> multiple scattering generation ON/OFF
//
// Reconstruction parameters:
// kDeltaPhi  -> 0.005
// kZbinWidth -> 0.04
// kDeltaZ    -> 0.10
// kZwidth    -> 0.24
// kMeanNoise -> mean number of noise/soft hits

#include <Riostream.h>
#include <TROOT.h>
#include "../cfg/Constants.h"

enum LaunchSettings{
  kUnifOn,
  kUnifOff,
  kUnifOnLowNoise, // set mean noise to zero
  kHistOn,
  kHistOff
};

void LaunchAnalysis(int nEvents,LaunchSettings setting){
  char launchSimul[80];
  char launchRec[80];
  char launchHist[80];
  char launchDraw[80];
  char SettingDistr[50];
  char DistrMult[15];
  char DistrEta[15];
  char OnOffScat[15];
  char DeltaMultBins[3];
  char MeanNoise[5];
  sprintf(MeanNoise,"20");
  switch(setting){
    case kUnifOff:{
      sprintf(SettingDistr,"UnifOff");
      sprintf(DistrMult,"kUnifMult");
      sprintf(DistrEta,"kUnifEta");
      sprintf(OnOffScat,"kOffScat");
      sprintf(DeltaMultBins,"0");
      std::cout<<"Uniform distributions, multiple scattering off"<<std::endl;
    } break;
    case kHistOn:{
      sprintf(SettingDistr,"HistOn");
      sprintf(DistrMult,"kHistMult");
      sprintf(DistrEta,"kHistEta");
      sprintf(OnOffScat,"kOnScat");
      sprintf(DeltaMultBins,"1"); // do not consider last multiplicity bin (empty bin)
      std::cout<<"Histogram distributions, multiple scattering on"<<std::endl;
    } break;
    case kHistOff:{
      sprintf(SettingDistr,"HistOn");
      sprintf(DistrMult,"kHistMult");
      sprintf(DistrEta,"kHistEta");
      sprintf(OnOffScat,"kOffScat");
      sprintf(DeltaMultBins,"1"); // do not consider last multiplicity bin (empty bin)
      std::cout<<"Histogram distributions, multiple scattering off"<<std::endl;
    } break;
    case kUnifOnLowNoise:{
      sprintf(MeanNoise,"0");
      std::cout<<"Low noise: mean noise = "<<MeanNoise<<std::endl;
    }
    default:{
      sprintf(SettingDistr,"UnifOn"); // set filename
      sprintf(DistrMult,"kUnifMult");
      sprintf(DistrEta,"kUnifEta");
      sprintf(OnOffScat,"kOnScat");
      sprintf(DeltaMultBins,"0");
      std::cout<<"Uniform distributions, multiple scattering on"<<std::endl;
    }
  }
  sprintf(launchSimul,"SteerSimul(\"%s%s\",%d,%s,%s,%s)",kFileNameSimul,SettingDistr,nEvents,DistrMult,DistrEta,OnOffScat);
  sprintf(launchRec,"SteerRec(\"%s%s\",\"%s%s\")",kFileNameSimul,SettingDistr,kFileNameRec,SettingDistr);
  sprintf(launchHist,"CreateHist(\"%s%s\",\"%s%s\",%s)",kFileNameRec,SettingDistr,kFileNameHist,SettingDistr,DeltaMultBins);
  sprintf(launchDraw,"DrawHist(\"%s%s\")",kFileNameHist,SettingDistr);
  gROOT->ProcessLine(launchSimul);
  gROOT->ProcessLine(launchRec);
  gROOT->ProcessLine(launchHist);
  gROOT->ProcessLine(launchDraw);
}