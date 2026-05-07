#include "JetSingleHistos.hpp"
#include "Paired_class.hpp"
#include <iostream>
#include "Pythia8/Pythia.h"
#include "fastjet/ClusterSequence.hh"
#include <string>
#include <TFile.h>
#include <TH1D.h>
#include "TProfile.h"


using namespace fastjet;
using namespace std;
using namespace Pythia8;

void Store(Event & event_array, vector<PseudoJet>& clustering_input){
	/* we should clear the vector before storing new data */
	clustering_input.clear();

	int no_of_particles_in_an_event = event_array.size();
	cout<< "no_of_particles_in_an_event "<<no_of_particles_in_an_event<<endl;

        for (int i = 0; i < no_of_particles_in_an_event; ++i)
        {   
            if(event_array[i].isFinal()){
                double px, py, pz, E;
                 int id;

                 id = event_array[i].id();

                 px = event_array[i].px();
                 py = event_array[i].py();
                 pz = event_array[i].pz();
                 E = event_array[i].e();

                 PseudoJet particles (px, py, pz, E);
                particles.set_user_index(id);

                clustering_input.push_back(particles);
            }else{ continue; }
        	
        }

}

void Cluster_ing(vector<PseudoJet> & input,ClusterSequence* & ptr ){

	// jet definition
       double R = 0.6;
       JetDefinition jet_def(antikt_algorithm, R);

      
       // Create a new ClusterSequence object
    	ptr = new ClusterSequence(input, jet_def);
}

int main()
{   
    // Retriving seed from the exported environment variable called "SEED"
    const char* seed_env = std::getenv("SEED");
    if (seed_env == nullptr) {
        std::cerr << "Environment variable SEED not set." << std::endl;
       // return 1;
    }

    int seed = 0;//std::atoi(seed_env);  // Convert the environment variable to an integer
   

    Pythia pythia;

    int no_events = 5000e3;
    string Single_title = "5000K_Test_without_Psorted_R=0.4";

	JetSingleHistos Particles(Single_title);

    string Paired_tile = Single_title +"_Paired";

	Paired_class Paired_Particles(Paired_tile);

	vector<PseudoJet> Stored_particles;
	// Generate events

	/* Configurtion setting of the Collision events*/
    pythia.readString("Random:seed = "+ std::to_string(seed));  // Set the seed
    pythia.readString("Random:setSeed = on");  // Ensure Pythia uses the set seed

	pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 13000");
    pythia.readString("SoftQCD:all = off");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 10.");

    pythia.init();// Initializing

    if (!pythia.init()) {// Checking the Initialization succedd or not
        cerr << "Pythia initialization failed!" << endl; 
        return 1;
        }

    for (int i = 1; i < (no_events +1) ; i++) {

        ClusterSequence* cs = nullptr;

    	// Clear the event
        pythia.event.clear();

        // Generate next event
        if (!pythia.next()) continue;

        double entries = pythia.event.size();
        cout<<"sucessful "<< i <<endl;

        //call the Stor fucntion which stores all the generated particles in a vector of PseudoJets.
        Store(pythia.event, Stored_particles);

        //We pass the the stored vector of PseudoJets to be cluster by call Cluster_ing function.
        Cluster_ing(Stored_particles,cs);

        //Since the class Clustering Sequence in the Fastejet contains useful function such at constituenys of jets and px() etc we need to acces this so we need the instace of the clustering sequnce class. thus we asre passing this information to our defined class called Single and Pairs.
        Particles.Set_ClusterSequence(cs);
        Paired_Particles.Set_Paired_ClusterSequence(cs);


        /* Now store the clustered jets in the vector jets*/
       vector<PseudoJet> Clustered_Jets = sorted_by_pt( Particles.Get_ClusterSequence()->inclusive_jets() );

      /*
        // Sorting the clustered jets by ascending total momentum (p)
        std::sort(Clustered_Jets.begin(), Clustered_Jets.end(), [](const fastjet::PseudoJet& a, const fastjet::PseudoJet& b) {
        double p_a = std::sqrt(a.px() * a.px() + a.py() * a.py() + a.pz() * a.pz());
        double p_b = std::sqrt(b.px() * b.px() + b.py() * b.py() + b.pz() * b.pz());
        return p_a < p_b; // Custom comparison for ascending order by p
        });
    */
       int count = 0;
       for (int k = 0; k < Clustered_Jets.size(); ++k)
       {
           if(count<=1  &&  Clustered_Jets[k].constituents().size() >=5){

                auto& J = Clustered_Jets[k];

                if( J.eta()>=-0.5  &&  J.eta()<=0.5 ){
                    Particles.Fill_histogram( J );
                    Paired_Particles.Fill_Paired_histogram( J );
                    count++;

                }else{ continue; }

            }else if(count>1){
                count = 0;
                break;
            }
       }
        
       delete cs;
       cs = nullptr;
       Paired_Particles.Set_Paired_ClusterSequence(nullptr);
       Particles.Set_ClusterSequence(nullptr);
     
       
    }

    // to make sure all the histograms are ploted before accessing it.
    Particles.writeData();

    int no_of_jets_btw_0to10, no_of_jets_btw_10to20, no_of_jets_btw_20to30, no_of_jets_btw_30to40, no_of_jets_btw_40to50, no_of_jets_btw_50to200, total_jets;

    for (int j = 1; j <= Particles.Get_hist_jetp()->GetNbinsX(); ++j) {
    int binContent = Particles.Get_hist_jetp()->GetBinContent(j);
    cout << "Bin " << j << ": " << binContent << endl;

        total_jets+=binContent;

        if(j == 1){
            no_of_jets_btw_0to10  = binContent;          
         }else if( j == 2){
            no_of_jets_btw_10to20 = binContent;
         }else if(j == 3){
            no_of_jets_btw_20to30 = binContent;
          }else if (j == 4){
            no_of_jets_btw_30to40 = binContent;
         }else if(j == 5){
            no_of_jets_btw_40to50 = binContent;
        }else if(j >=6  &&  j<=20){
            no_of_jets_btw_50to200+= binContent;
         }

    }

    Particles.Normalize(total_jets,no_of_jets_btw_10to20);
    
    Paired_Particles.Normalize(no_of_jets_btw_0to10,no_of_jets_btw_10to20,no_of_jets_btw_20to30,no_of_jets_btw_30to40,no_of_jets_btw_40to50,no_of_jets_btw_50to200);

	return 0;
}
