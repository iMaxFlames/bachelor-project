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
    TH2F *hist = new TH2F("hist", "Isotropically Generated Event(s)",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -1, 1);           // y axis
    hist->SetStats(0); // to get rid of the legend

    // initialize a random number generator
    gRandom = new TRandom3(0);

    // isotropic multi-event generator
    int number_of_events = 1000;

    for(int i = 0; i < number_of_events; i++)
    {
        // number of particles per event (sets the length of arrays to be generated)
        int number_of_particles = 100;

        // phi is in an interval [0, 2pi], these are the endpoints for the interval
        double phi_min = 0;
        double phi_max = 2*TMath::Pi();

        // doing the same for eta
        double eta_min = -1;
        double eta_max = 1;

        // generating random values for phi from the above interval
        for(int i = 0; i < number_of_particles; i++)
        {
            double phi = phi_min + gRandom->Rndm()*(phi_max - phi_min);
            double eta = eta_min + gRandom->Rndm()*(eta_max - eta_min);

            // entering these into our histogram
            hist->Fill(phi, eta);
        }
    }

    hist->GetXaxis()->SetTitle("#phi");
    hist->GetYaxis()->SetTitle("#eta");
    hist->Draw("Colz"); // use Lego2 for fancy 3D plot and Colz for regular 2D plot
}