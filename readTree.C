#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TStyle.h"

void readTree()
{
  gStyle->SetOptStat(111111);

  TFile *inFile = new TFile("charged_particle_tree.root","read");
  
  TTree* fTree = (TTree*)inFile->Get("TT");
  TClonesArray* trackArray = new TClonesArray("TParticle", 100);
  fTree->SetBranchAddress("tracks", &trackArray);

  // read in histograms that were saved in the root file
  TH1F* hMult = (TH1F*)inFile->Get("hMult");
  TH1F* hdNdeta = (TH1F*)inFile->Get("hdNdeta");

  // make new histograms that you will fill with input from the tree
  TH1F* hPhi = new TH1F("hPhi","hPhi",100,0,2*TMath::Pi()); // azimuthal angle phi
  TH1F* hPt = new TH1F("hPt","hPt",100,0,15); // transverse momentum pt
  TH1F* hPtPion = new TH1F("hPtPion","hPtPion",100,0,15); // transverse momentum pt of pions
  
  Int_t nEvents = fTree->GetEntries(); // number of events
  for(Int_t nEv = 0; nEv < nEvents; nEv++) // loop over all events
    {
      if (nEv%1000==0) Printf("Event=%i   %.2lf%% done",nEv,Double_t(nEv)/Double_t(nEvents)*100.);
      
      fTree->GetEntry(nEv); // "get" the current event

      Int_t nTracks = trackArray->GetEntriesFast(); // number of tracks in the current event
      for(Int_t nTr = 0; nTr < nTracks; nTr++) // loop over tracks
	{
	  TParticle *track = (TParticle*)trackArray->At(nTr); // "get" the current track

	  // fill histogram with properties of the tracks
	  hPhi->Fill(track->Phi());
	  hPt->Fill(track->Pt());

	  // example: fill pt histogram only with pions (PDG code 211 is pi+, -211 is pi-)
	  // all pdg codes are listed here: https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
	  if(track->GetPdgCode() == 211 || track->GetPdgCode() == -211)
	    {
	      hPtPion->Fill(track->Pt());
	    }
	} // end loop over tracks
    } // end loop over events

  
  TCanvas *cEvent = new TCanvas("cEvent","event information",1600,500);
  cEvent->Divide(2,1);
  cEvent->cd(1);
  hMult->DrawCopy();
  cEvent->cd(2);
  hdNdeta->DrawCopy();
  
  TCanvas *cPhi = new TCanvas("cPhi");
  hPhi->DrawCopy();

  TCanvas *cPt = new TCanvas("cPt");
  hPt->DrawCopy();
  hPtPion->SetLineColor(2);
  hPtPion->DrawCopy("same"); // draw histogram on the same canvas
  gPad->SetLogy(); // set y-axis on the log scale
  
}
