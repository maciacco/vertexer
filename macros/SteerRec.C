// SteerRec.C
// The reconstruction steering macro
//
// Authors: Mario Ciacco & Emanuele Data

#include <Riostream.h>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TClonesArray.h>
#include <TStopwatch.h>
#include "../src/SimulManager.h"
#include "../src/RecManager.h"
#include "../cfg/Constants.h"

void SteerRec(std::string inFilename="simul",std::string outFilename="recResult",double deltaPhi=kDeltaPhi,double zBinWidth=kZbinWidth,double deltaZ=kDeltaZ,double zWidth=kZwidth,int meanNoiseSoft=kMeanNnoise){
  if(deltaPhi>0.05 || meanNoiseSoft>100){ // check if deltaPhi or meanNoiseSoft is not too high to not have segmentation violation due to dimension of tracklets intersection array
    std::cout<<"--- WARNING ---"<<std::endl;
    std::cout<<"Parameter DeltaPhi or MeanNnoise is too high"<<std::endl;
    return;
  }
  std::string inFilename_ext=FILE_DIR+inFilename+".root";      // filename with *.root extension
  std::string outFilename_ext=FILE_DIR+outFilename+".root";    // filename with *.root extension
    
  TStopwatch swatch;

  // DECLARE MEMORY LOCATION
  static VTX vert={0,0,0,0};
  TClonesArray *hitsFirstLayer=new TClonesArray("Point2D",100);
  TClonesArray *hitsSecondLayer=new TClonesArray("Point2D",100);

  // OPEN FILES (IN/OUT)
  TFile inFile(inFilename_ext.c_str());
  TFile outFile(outFilename_ext.c_str(),"RECREATE");  // open a file (write mode)
  if(!inFile.IsOpen()){
    std::cout<<"No input file!"<<std::endl;
    return;
  }
    
  // INSTANTIATE NEW TREE TO SAVE VERTICES
  TTree *vtxTree=new TTree(RecTreeName,"vertices");
  static Vertex vtx(-999.f,-999.f,-999.f,-999.f,false);// memory location mapped to tree
  vtxTree->Branch(RecVertBaranchName,&vtx);            // connect branch to the first memory location; specify types

  // GET TREE FROM INPUT FILE
  TTree *inTree=(TTree*)inFile.Get(SimulTreeName);
    
  // GET TREE BRANCHES FROM EXISTING TREE
  TBranch *bVertMult=inTree->GetBranch(SimVertBranchName);
  TBranch *bFirstLayer=inTree->GetBranch(SimHitFirstBranchName);
  TBranch *bSecondLayer=inTree->GetBranch(SimHitSecondBranchName);

  // SET ADDRESSES OF BRANCHES
  bVertMult->SetAddress(&vert.X);
  bFirstLayer->SetAddress(&hitsFirstLayer);
  bSecondLayer->SetAddress(&hitsSecondLayer);

  // INSTANTIATE LAYERS
  Layer *layers[2];
  layers[0]=new Layer(kFirstLayerRadius,kFirstLayerThick,kFirstLayerLength,kRadLengthSi,kZresol,kRphiResol); // cm, cm
  layers[1]=new Layer(kSecondLayerRadius,kSecondLayerThick,kSecondLayerLength,kRadLengthSi,kZresol,kRphiResol); // cm, cm

  // INSTANTIATE RECONSTRUCTION MANAGER AND RUN SIMULATION
  RecManager *manager=RecManager::GetInstance(deltaPhi,zBinWidth,deltaZ,zWidth,meanNoiseSoft);
  swatch.Start(); // start stopwatch
  manager->RunReconstruction(inTree,vtxTree,vert,vtx,hitsFirstLayer,hitsSecondLayer,layers);
  swatch.Stop();
  swatch.Print("m");
  manager=RecManager::Destroy();
    
  // WRITE AND CLOSE FILE
  outFile.Write();
  outFile.Close();
  inFile.Close();
    
}