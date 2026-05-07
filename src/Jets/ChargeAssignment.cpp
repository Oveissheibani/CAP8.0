/* class  implementation */

#include "ChargeAssignment.hpp"
#include <map>
#include <cmath>



ChargeAssignment :: ChargeAssignment ()
: particleCharges ({
		{1,-0.3334}, {2,0.6667}, {3,-0.3334}, {4,0.6667}, {5,-0.3334}, {6,0.6667}, {7,-0.3334}, {8, 0.6667}, //Quarks, the last two are super symmetric quarks
		{-1,0.3334}, {-2,-0.6667}, {-3,0.3334}, {-4,-0.6667}, {-5,0.3334}, {-6,-0.6667}, {-7,0.3334}, {-8, -0.6667}, //Anti-Quarks, the last two are super symmetric quarks

		{11, -1}, {12,0}, {13,-1}, {14,0}, {15,-1}, {16,0}, {17,-1}, {18,0}, // Leptons
		{-11, 1}, {-12,0}, {-13,1}, {-14,0}, {-15,1}, {-16,0}, {-17,1}, {-18,0}, // Anti-Leptons

		{4000001,-0.3334}, {4000002,0.6667}, {4000011,-1}, {4000012,0}, //Excited Particles
		{-4000001,0.3334}, {-4000002,-0.6667}, {-4000011,1}, {-4000012,0}, //Anti-Excited Particles

		{21,0}, {9,0}, {22,0}, {23,0}, {24,1}, {25,0}, {32,0}, {33,0}, {34,1}, {35,0}, {36,0}, {37,1}, //Gauge and Higgs Bosons
		{-21,0}, {-9,0}, {-22,0}, {-23,0}, {-24,-1}, {-25,0}, {-32,0}, {-33,0}, {-34,-1}, {-35,0}, {-36,0}, {-37,-1}, //Anti-Gauge and Higgs Bosons

		{39,0}, {41,0}, {42,0}, {110,0}, {990,0}, {9990,0},//Special Particles
		{-39,0}, {-41,0}, {-42,0}, {-110,0}, {-990,0}, {-9990,0}//Anti-Special Particles

	})
{ 
	n_J = n_q3 = n_q2 = n_q1 = n_L = n_r = n = 0; 

	// Set key-value pairs for keys 81 to 100 to zero
    for (int key = 81; key <= 100; ++key) {
        particleCharges[key] = 0.0;
    }
}

/* Defining Rounding function which can be used later */
double ChargeAssignment :: roundOff (double input){
	double temp = round(input);
	if (temp == -0.0){
		temp = 0; 
	}
	return temp;
}


ChargeAssignment ::~ChargeAssignment() {}

double ChargeAssignment :: charge(int pdgid){
	 // Check if particle is in the lookup map
    auto it = particleCharges.find(pdgid);
    if (it != particleCharges.end()) {
        return it->second;
	}

	//Exgtract charge of the particle based on it's quark content

	//+-n n_r n_L n_q1 n_q2 n_q3 n_J this is how particles are numbered according to pdg Normanclature
	//Thus n_q1, n_q2, n_q3 can give us the idea regarding quark composition.

	/* Now lets find the n_q1, n_q2, n_q3 values */
	int mod_pdgid = (pdgid >= 0) ? pdgid : -pdgid;

	 n_J = ( mod_pdgid % 10 );
	 n_q3 = ( (mod_pdgid % 100) - n_J ) /10 ; 
	 n_q2 =  ( (mod_pdgid %1000) - ((mod_pdgid % 1000) % 100 ) ) / 100 ; 
	 n_q1 = ( ( mod_pdgid %10000 ) - ( (mod_pdgid % 10000) % 1000 ) ) /1000; 
	 n_L = ( (mod_pdgid % 100000) - ( (mod_pdgid % 100000) % 10000 ) )/ 10000;
	 n_r = ( (mod_pdgid % 1000000) - ( (mod_pdgid % 1000000) % 100000 ) )/ 100000;
	 n = ( (mod_pdgid % 10000000) - ( (mod_pdgid % 10000000) % 1000000 ) )/ 1000000; 
	// cout<< "n_J = " << n_J<<" n_q3 = "<< n_q3 <<" n_q2 = "<< n_q2<<" n_q1 = "<<n_q1<<" n_L = "<<n_L<< " n_r = "<< n_r << " n = "<<n<< endl;

	double Q ;

	if (pdgid>0){
		if(n == 9  &&  n_r == 9){
			Q = 0;
			return roundOff(Q);
		}else if (n_q1 == 0){//mesons
			Q =( particleCharges.at(n_q2) - particleCharges.at(n_q3) );
			return roundOff(Q);
		}else if (n_q3 == 0){ // Diquarks
			 Q = ( particleCharges.at(n_q2) + particleCharges.at(n_q1) );
			 cout<< particleCharges.at(n_q1)<<endl;
			return roundOff(Q);
		}else if (n == 9  &&  n_q1 != 0){ // PentaQuark
			Q = particleCharges.at(n_r) + particleCharges.at(n_L) + particleCharges.at(n_q1) + particleCharges.at(n_q2) + particleCharges.at(n_q3);
			return roundOff(Q);
		}else { // Baryons
			Q = particleCharges.at(n_q1) + particleCharges.at(n_q2) + particleCharges.at(n_q3);
			return roundOff(Q);
		}	
	}else if (pdgid<0){
		if(n == 9  &&  n_r == 9){
			Q = 0;
			return roundOff(Q);
		}else if (n_q1 == 0){//mesons
			Q = (particleCharges.at(n_q2) -particleCharges.at(n_q3) );
			return roundOff(-Q);
		}else if (n_q3 == 0){ // Diquarks
			 Q = particleCharges.at(n_q2)+particleCharges.at(n_q1);
			return roundOff(-Q);
		}else if (n == 9  &&  n_q1 != 0){ // PentaQuark
			Q = particleCharges.at(n_r) + particleCharges.at(n_L) + particleCharges.at(n_q1) + particleCharges.at(n_q2) + particleCharges.at(n_q3);
			return roundOff(-Q);
		}else { // Baryons
			Q = particleCharges.at(n_q1) + particleCharges.at(n_q2) + particleCharges.at(n_q3);
			return roundOff(-Q);
		}	
	}else { cout<< "Error: wrong pdgid = 00000 \n";
	return 0;
	}

}

