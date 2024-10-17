#include <iostream>
#include <fstream>
#include <vector>

#include "HepMC3/ReaderAscii.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/GenParticle.h"

#include "TMath.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TFile.h"
#include "TStyle.h"

void readHepMC(const std::string& hepmcFile = "test_out.hepmc") {

  gStyle->SetOptStat(111111);
  
  // Open the HEPMC file for reading
    HepMC3::ReaderAscii reader(hepmcFile);

    TFile *outFile = new TFile("jetscape_tree.root","recreate");

    TTree* tree = new TTree("TT", "JETSCAPE Tree");
    TClonesArray* particleArray = new TClonesArray("TParticle", 100);
    tree->Branch("particles", &particleArray);
    Int_t nparticles;
    tree->Branch("mult", &nparticles, "nparticles/I");

    TH1F* hPt = new TH1F("hPt","Transverse momentum;pt (GeV/c);dN/dpt",100,0,100);
    TH2F* hPhiEta = new TH2F("hPhiEta","phi vs eta;#varphi;#eta;N",60,0,TMath::TwoPi(),100,-5,5);
    
    Int_t nevents = 0;
    
    // Loop over the events in the HEPMC file
    while (!reader.failed()) {
        HepMC3::GenEvent event;
        reader.read_event(event);

	nparticles = 0;
        // Loop over the particles in the event
        for (const auto& particle : event.particles()) {
            int pdgId = particle->pid();

	    if(particle->status() == 1 && particle->momentum().eta() <= 1 && particle->momentum().eta() >= -1) // final-state particles
	      {
		TParticle* track = new((*particleArray)[nparticles]) TParticle();
		track->SetPdgCode(particle->pid());
		track->SetMomentum(particle->momentum().px(), particle->momentum().py(),
				   particle->momentum().pz(), particle->momentum().e());
		hPt->Fill(track->Pt());
		hPhiEta->Fill(track->Phi(),track->Eta());
		nparticles++;
	      }
        }
	nevents++;
	tree->Fill();
	particleArray->Delete();
    }

    TCanvas *c1 = new TCanvas();
    hPt->DrawCopy();

    TCanvas *c2 = new TCanvas();
    hPhiEta->DrawCopy("colz");

    nevents -= 1;
    
    cout << "Processed " << nevents << " events!" << endl;
    outFile->Write();
    outFile->Close();
}
