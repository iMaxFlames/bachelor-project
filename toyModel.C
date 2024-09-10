#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TMath.h"

#include <iostream>
#include <vector>
using namespace std;

void toyModel(int number_of_events = 100,
              int number_of_particles = 100,     // number of particles per event
              int number_of_jet_particles = 100, // same for recoil jets
              bool show_recoil_jet = false)       // this is the jet emitted opposite in azimuth to the single jet
{
    // gStyle->SetOptStat(111111);
    gStyle->SetPalette(kDeepSea);
    // gStyle->SetOptStat(0);
    // gStyle->SetOptTitle(0);

    // creating a canvas to draw stuff on
    TCanvas *c1 = new TCanvas();
    c1->cd(); // I think this sets c1 as the canvas to draw on

    // creating the 2D histogram
    TH2F *hist = new TH2F("hist", "Isotropically Generated Event(s)",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -1, 1);           // y axis
    hist->SetStats(0); // to get rid of the legend

    // vectors to store values of phi and eta
    vector<double> phi_values = {};
    vector<double> eta_values = {};

    // isotropic multi-event generator
    for(int i = 0; i < number_of_events; i++)
    {
        // initialize a random number generator
        gRandom = new TRandom3(0);

        // phi is in an interval [0, 2pi], these are the endpoints for the interval
        double phi_min = 0;
        double phi_max = 2*TMath::Pi();

        // eta in [-1, 1]
        double eta_min = -1;
        double eta_max = 1;

        // generating random values for phi from the above interval (background)
        for(int i = 0; i < number_of_particles; i++)
        {
            double phi = phi_min + gRandom->Rndm()*(phi_max - phi_min);
            double eta = eta_min + gRandom->Rndm()*(eta_max - eta_min);

            // appending the random values to our vectors
            phi_values.push_back(phi);
            eta_values.push_back(eta);

            // entering these into our histogram
            hist->Fill(phi, eta);
        }

        // GENERATING ONE JET PER EVENT
        double phi_jet = phi_min + gRandom->Rndm()*(phi_max - phi_min); // jet axis in azimuth
        double eta_jet = eta_min + gRandom->Rndm()*(eta_max - eta_min);

        for(int i = 0; i < number_of_jet_particles; i++)
        {
            double phi_jet_particle = gRandom->Gaus(phi_jet, 0.1);
            double eta_jet_particle = gRandom->Gaus(eta_jet, 0.1);

            phi_values.push_back(phi_jet_particle);
            eta_values.push_back(eta_jet_particle);

            hist->Fill(phi_jet_particle, eta_jet_particle);
        }

        // RECOIL JET
        if (show_recoil_jet)
        {
            double phi_recoil = phi_jet + TMath::Pi(); // jet axis plus pi
            
            // making sure its in the interval [0, 2pi]
            while (phi_recoil > 2*TMath::Pi())
            {
                phi_recoil -= 2*TMath::Pi();
            }

            while (phi_recoil < 0)
            {
                phi_recoil += 2*TMath::Pi();
            }

            double eta_recoil = eta_min + gRandom->Rndm()*(eta_max - eta_min); // this is still random because of lab frame boost

            for(int i = 0; i < number_of_jet_particles; i++)
            {
                double phi_recoil_particle = gRandom->Gaus(phi_recoil, 0.1);
                double eta_recoil_particle = gRandom->Gaus(eta_recoil, 0.1);

                phi_values.push_back(phi_recoil_particle);
                eta_values.push_back(eta_recoil_particle);

                hist->Fill(phi_recoil_particle, eta_recoil_particle);
            }
        }

    }

    hist->GetXaxis()->SetTitle("#phi");
    hist->GetYaxis()->SetTitle("#eta");
    hist->Draw("Colz"); // use Lego2 for fancy 3D plot and Colz for regular 2D plot

    // TWO PARTICLE CORRELATION

    // creating a canvas to draw stuff on
    TCanvas *c2 = new TCanvas();
    c2->cd(); // selecting c2 to draw on

    // creating the 2D histogram
    TH2F *delta_hist = new TH2F("delta_hist", "Two Particle Correlation",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -2, 2);           // y axis
    delta_hist->SetStats(0); // to get rid of the legend

    for(int i = 0; i < phi_values.size(); ++i)
    {
        for(int j = 0; j < phi_values.size(); ++j)
        {
            if (j == i) {continue;}

            double delta_phi = phi_values[i] - phi_values[j];
            double delta_eta = eta_values[i] - eta_values[j];

            // accounting for the periodic nature of delta_phi
            while (delta_phi > 2*TMath::Pi())
            {
                delta_phi -= 2*TMath::Pi();
            }

            while (delta_phi < 0)
            {
                delta_phi += 2*TMath::Pi();
            }
            
            delta_hist->Fill(delta_phi, delta_eta);
        }
    }

    delta_hist->GetXaxis()->SetTitle("#Delta#phi");
    delta_hist->GetYaxis()->SetTitle("#Delta#eta");
    delta_hist->Draw("Colz"); // use Lego2 for fancy 3D plot and Colz for regular 2D plot

    
}