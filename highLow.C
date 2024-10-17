void highLow(int pool_size = 10,
              double high_Pt_threshold = 3,
              double low_Pt_threshold = 3)
{
    gStyle->SetPalette(kRainbow);

    TFile *inFile = new TFile("vac.root", "read");

    TTree* fTree = (TTree*)inFile->Get("TT");
    TClonesArray* trackArray = new TClonesArray("TParticle", 100);
    fTree->SetBranchAddress("particles", &trackArray);

    // creating the 2D histogram for the two particle correlation in the same event
    TH2F *delta_hist = new TH2F("delta_hist", "High-Low Two Particle Correlation",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -2, 2);           // y axis
    delta_hist->SetStats(0); // to get rid of the legend

    // creating the 2D histogram for the two particle correlation in mixed events (from a pool of previous events)
    TH2F *mixed_delta_hist = new TH2F("mixed_delta_hist", "Two Particle Correlation",
                          100, 0, 2*TMath::Pi(), // x axis
                          100, -2, 2);           // y axis
    mixed_delta_hist->SetStats(0); // to get rid of the legend

    vector<vector<double>> low_phi_event_pool;
    vector<vector<double>> low_eta_event_pool;

    Int_t nEvents = fTree->GetEntries(); // number of events
    for(Int_t nEv = 0; nEv < nEvents; nEv++) // loop over all events
    {
        fTree->GetEntry(nEv); // "get" the current event

        vector<double> low_phi_values = {};
        vector<double> low_eta_values = {};

        vector<double> high_phi_values = {};
        vector<double> high_eta_values = {};

        Int_t nTracks = trackArray->GetEntriesFast(); // number of tracks in the current event
        for(Int_t nTr = 0; nTr < nTracks; nTr++) // loop over tracks
        {
            TParticle *track = (TParticle*)trackArray->At(nTr); // "get" the current track

            double current_particle_Pt = track->Pt();
            if (current_particle_Pt > high_Pt_threshold)
            {
                // fill vectors with properties of the current track
                high_phi_values.push_back(track->Phi());
                high_eta_values.push_back(track->Eta());
            }
            else if (current_particle_Pt < low_Pt_threshold)
            {
                low_phi_values.push_back(track->Phi());
                low_eta_values.push_back(track->Eta());
            }
            else
            {
                continue;
            }
        }

        // SAME EVENT TWO PARTICLE CORRELATION
        for(int i = 0; i < high_phi_values.size(); ++i)
        {
            for(int j = 0; j < low_phi_values.size(); ++j)
            {
                if (j == i) {continue;}

                double delta_phi = high_phi_values[i] - low_phi_values[j];
                double delta_eta = high_eta_values[i] - low_eta_values[j];

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

        // MIXED EVENT CORRELATION
        if(nEv > pool_size)
        {
            for(int i = 0; i < high_phi_values.size(); i++)
            {
                for(int j = 0; j < low_phi_event_pool.size(); j++)
                {
                    for(int k = 0; k < low_phi_event_pool[j].size(); k++)
                    {
                        double delta_phi = high_phi_values[i] - low_phi_event_pool[j][k];
                        double delta_eta = high_eta_values[i] - low_eta_event_pool[j][k];

                        // accounting for the periodic nature of delta_phi
                        while (delta_phi > 2*TMath::Pi())
                        {
                            delta_phi -= 2*TMath::Pi();
                        }

                        while (delta_phi < 0)
                        {
                            delta_phi += 2*TMath::Pi();
                        }

                        mixed_delta_hist->Fill(delta_phi, delta_eta);
                    }
                }
            }
        }

        // inserting the current phi and eta values (vectors) into the pool at the beginning
        low_phi_event_pool.insert(low_phi_event_pool.begin(), low_phi_values);
        low_eta_event_pool.insert(low_eta_event_pool.begin(), low_eta_values);

        // removing the oldest event from the pool
        if(nEv > pool_size)
        {
            low_phi_event_pool.pop_back();
            low_eta_event_pool.pop_back();
        }
    } // end loop over events

    TCanvas *c2 = new TCanvas("c2");

    delta_hist->Divide(mixed_delta_hist);

    delta_hist->GetXaxis()->SetTitle("#Delta#phi");
    delta_hist->GetYaxis()->SetTitle("#Delta#eta");
    delta_hist->DrawCopy("Surf2"); // Colz for 2D plot, Surf2 for 3D plot

    inFile->Close();
}