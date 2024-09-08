#include "TRandom3.h"
#include "TStyle.h"

void toyModel()
{
    // gStyle->SetOptStat(111111);
    gStyle->SetPalette(kBird);
    // gStyle->SetOptStat(0);
    // gStyle->SetOptTitle(0);

    // creating a canvas to draw stuff on
    TCanvas *c1 = new TCanvas();

    // creating the 2D histogram
    TH2F *hist = new TH2F("hist", "Isotropically Generated Event",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -1, 1);           // y axis
    hist->SetStats(0); // to get rid of the legend

    // initialize a random number generator
    gRandom = new TRandom3(0);

    // number of particles (sets the length of arrays to be generated)
    int N = 100;

    // phi is in an interval [0, 2pi], these are the endpoints for the interval
    double phi_min = 0;
    double phi_max = 2*TMath::Pi();

    // doing the same for eta
    double eta_min = -1;
    double eta_max = 1;

    // generating random values for phi from the above interval
    for(int i = 0; i < N; i++)
    {
        double phi = phi_min + gRandom->Rndm()*(phi_max - phi_min);
        double eta = eta_min + gRandom->Rndm()*(eta_max - eta_min);

        // entering these into our histogram
        hist->Fill(phi, eta);
    }

    hist->GetXaxis()->SetTitle("#phi");
    hist->GetYaxis()->SetTitle("#eta");
    hist->Draw("Lego2");
}