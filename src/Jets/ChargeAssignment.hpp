#ifndef ChargeAssignment_HPP
#define ChargeAssignment_HPP
#include <iostream>
#include <map>
#include <cmath>

using namespace std;

class ChargeAssignment
{

public:
double charge(int pdgid);
ChargeAssignment(); //Constructor
~ChargeAssignment(); // Destructor
double roundOff(double input);

private:
	int n_J, n_q3, n_q2, n_q1, n_L, n_r, n;
	/* Map for the fundemental particles */
	std::map<int, double> particleCharges;
};
#endif // ChargeAssignment_HPP


/* = {
		{1,-0.3334}, {2,0.6667}, {3,-0.3334}, {4,0.6667}, {5,-0.3334}, {6,0.6667}, {7,-0.3334}, {8, 0.6667}, //Quarks, the last two are super symmetric quarks
		{11, -1}, {12,0}, {13,-1}, {14,0}, {15,-1}, {16,0}, {17,-1}, {18,0}, // Leptosn
		{4000001,-0.3334}, {4000002,0.6667}, {4000011,-1}, {4000012,0}, //Excited Particles
		{21,0}, {9,0}, {22,0}, {23,0}, {24,1}, {25,0}, {32,0}, {33,0}, {34,1}, {35,0}, {36,0}, {37,1}, //Gauge and Higgs Bosons
		{39,0}, {41,0}, {42,0}, {110,0}, {990,0}, {9990,0},//Special Particles
	}; */
