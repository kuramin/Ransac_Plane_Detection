#include "Scan_Point.h"
#include "TXTReader.h"
#include "TXTWriter.h"
#include "my_Color.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

// function deletes zeros from vector of pointers to Scan_Points
void Delete_zeros_1D(vector<Scan_Point*> &points) {
	int i = 0;
	// while on the position i there is element with x = y = z = 0, delete element i
	while (i < points.size())
	{ 
		if (points[i]->x == 0 && points[i]->y == 0 && points[i]->z == 0)
			points.erase(points.begin() + i);
		else
			i++;
	}
}

// function transforms vector of vector of scanpoints into vector of scanpoints
vector<Scan_Point*> Matrix_2D_to_1D(vector<vector<Scan_Point*>> &input) {
	vector<Scan_Point*> result;
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input[i].size(); j++) {
			result.push_back(input[i][j]);
		}
	}
	return result;
}

// function calculates euclidean distance between points p_scp1 and p_scp2
float distancePointToPoint(Scan_Point* &p_scp1, Scan_Point* &p_scp2) {
	return sqrt(pow(p_scp1->x - p_scp2->x, 2) + pow(p_scp1->y - p_scp2->y, 2) + pow(p_scp1->z - p_scp2->z, 2));
}

// function saves coefficients A, B, C, D of plane through points p1, p2, p3
void planeCoefficients(Scan_Point* p1, Scan_Point* p2, Scan_Point* p3, float &A, float &B, float &C, float &D) {
	A = p1->y * p2->z + p3->y * p1->z + p2->y * p3->z - p3->y * p2->z - p2->y * p1->z - p1->y * p3->z;
	B = -(p1->x * p2->z + p3->x * p1->z + p2->x * p3->z - p3->x * p2->z - p2->x * p1->z - p1->x * p3->z);
	C = p1->x * p2->y + p3->x * p1->y + p2->x * p3->y - p3->x * p2->y - p2->x * p1->y - p1->x * p3->y;
	D = -(p1->x * p2->y * p3->z + p3->x * p1->y * p2->z + p2->x * p3->y * p1->z - p3->x * p2->y * p1->z - p2->x * p1->y * p3->z - p1->x * p3->y * p2->z);
}

float areaOfTriangle(Scan_Point* p1, Scan_Point* p2, Scan_Point* p3) {
	float i = (p2->y - p1->y) * (p3->z - p1->z) - (p3->y - p1->y) * (p2->z - p1->z);
	float j = (p2->z - p1->z) * (p3->x - p1->x) - (p3->z - p1->z) * (p2->x - p1->x);
	float k = (p2->x - p1->x) * (p3->y - p1->y) - (p3->x - p1->x) * (p2->y - p1->y);
	return sqrtf(pow(i, 2) + pow(j, 2) + pow(k, 2)) / 2;
}

// calculates distance from point to the plane given by A, B, C, D
float perpendicularDistance(Scan_Point* point, float A, float B, float C, float D) {
	return abs((A * point->x + B * point->y + C * point->z + D) / sqrt(pow(A, 2) + pow(B, 2) + pow(C, 2)));
}

// function returns vector of vec_length randoms in range (0, max_rand)
vector<int> randomVector(int vec_length, int max_rand) {
	int rand_koef, rand_term;
	vector<int> result = {};
	
	for (int i = 0; i < vec_length; i++) { // create vector
		rand_koef = rand();
		rand_term = rand();
		result.push_back((rand_koef * (RAND_MAX + 1) + rand_term) % max_rand);
	}
	return result;
}

vector<vector<Scan_Point*>> setOfPlanarPointClouds(vector<Scan_Point*>& vec_p_scp, float thresh_area, float thresh_resid, float iter_ratio, float rest_ratio, float consen_ratio) {
	float A, B, C, D;
	int c1, c2, c3;

	int counter, N_iter;
	float distPointPlane;
	int thresh_rest = rest_ratio * vec_p_scp.size();
	int point_counter, max_point_counter = 0;
	vector<Scan_Point*> consen_set, best_consen_set = {}; // points which are close enough to the plane will be called plane's consensus set
	vector<vector<Scan_Point*>> result;

	// generate 3 random numbers in range (0; vec_p_scp.size() - 1) in order to use them instead (c1, c2, c3)
	vector<int> ranvec;
	srand(time(NULL)); // used to generate different randoms

	// 1) thresh_area - minimal area of triangle between triplet 
	// 2) thresh_resid - error tolerance, defines if point is compatible with plane
	// 3) thresh_iter - number of subsets to try to find a plane for current cloud
	// 4) thresh_rest - how many points can stay without of plane
	// 5) consen_ratio - which part of current cloud should be compatible with current plane to consider plane correct and quit searching

	while (vec_p_scp.size() > thresh_rest) {
		// repeat : choose 3 random indexes and calculate consensus set
		// if consensus set has enough points (consen_ratio), take this plane
		// if after thresh_iter iterations there is no good consensus set, take the best

		counter = 0;
		cout << "         " << vec_p_scp.size() << " points left,";

		// among these point cloud lets find a plane with the best consensus set 
		N_iter = 0;
		while (N_iter < iter_ratio * vec_p_scp.size()) {
			// set c1, c2, c3 as different random indexes, taken from generated random vector
			do ranvec = randomVector(3, vec_p_scp.size() - 1);
			while (ranvec[0] == ranvec[1] || ranvec[1] == ranvec[2] || ranvec[0] == ranvec[2]); 
			c1 = ranvec[0];
			c2 = ranvec[1];
			c3 = ranvec[2];
			counter++;

			// if lengths of sides of triangle between points are all more than threshold
			if (areaOfTriangle(vec_p_scp[c1], vec_p_scp[c2], vec_p_scp[c3]) > thresh_area) { 

				// then lets check if its the most popular plane for current pointset and if that's the case, update corresp values
				consen_set = { vec_p_scp[c1] , vec_p_scp[c2] , vec_p_scp[c3] }; // consen_set will already contain vertices of triangle
				planeCoefficients(vec_p_scp[c1], vec_p_scp[c2], vec_p_scp[c3], A, B, C, D); // calculate plane coefficients

				// if "function of update of CONSEN_SET for cloud VEC_P_SCP with plane ABCD of THRESH_RESID degree of freedom" is said to exist, begin it here
				for (int c4 = 0; c4 < vec_p_scp.size(); c4++) { // for every resting point of the pointcloud
					if (c4 != c1 && c4 != c2 && c4 != c3) { // if this point does not coincide with taken 3 points
						distPointPlane = perpendicularDistance(vec_p_scp[c4], A, B, C, D); // calculate distance from point to plane
						if (distPointPlane < thresh_resid) { // if point is close enough to the plane
							consen_set.push_back(vec_p_scp[c4]); // then add pointer to this point to vector of consens
						}
					}
				} // now we have number of points which are close enough to a current plane      // and end it here

				if (consen_set.size() > consen_ratio * vec_p_scp.size()) { // if this consensus set found for current plane is big enough
					best_consen_set = consen_set; // assign best_consen_set
					break; // break out from for-loop and dont look for best_consen_set for current cloud, its found
				} // else we should at least find the biggest best_consen_set
				else if (consen_set.size() > best_consen_set.size()) { // if current plane got more consens than the most popular of previous
						best_consen_set = consen_set; // update value of vector of pointers to the consens of current plane
			    }

				N_iter++; // increment number of iteration
			} // if some vertices of triangle are too close to each other, do nothing
		} // now we know the best consen_set for current state of pointcloud

		cout << " among them " << counter << " iterations found consensus set of " << best_consen_set.size() << endl;

		//lets push back detected vector of pointers to vec_of_vec result
		result.push_back(best_consen_set);

		// lets delete pointers to winners of current round from vector of pointers
		Scan_Point* p_scp;
		while (!best_consen_set.empty()) {
			p_scp = best_consen_set.back(); // take pointer from best_consen_set vector
			best_consen_set.pop_back();
			for (int i = 0; i < vec_p_scp.size(); i++) { // find which point of the cloud does it correspond to
				if (p_scp == vec_p_scp[i]) {
					vec_p_scp.erase(vec_p_scp.begin() + i); // and erase this point from the cloud
				}
			}
		}
	}
	return result;
}

// fucntion establishes rule of color comparison, which is used in function assignColorsToPlanars
bool colorComparison(my_Color* color1, my_Color* color2) { 
	return (color1->weight > color2->weight); 
}

// function assigns colors to planes
void assignColorsToPlanars(vector<vector<Scan_Point*>> &input) {
	// number of shades in each of component (R, G, B) must have value, which gives number of colors not less than number of planar segments
	int shades_num = ceil(pow(input.size(), 1.0/3));
	int shade_maxval = 255;

	// create a vector of different shades which can be taken by every channel R, G, or B
	vector<int> vec_shades;
	for (int i = 0; i < shades_num; i++) {
		vec_shades.push_back(i * shade_maxval / (shades_num-1));
	}

	vector<my_Color*> vec_col;
	// assign values of colors and calculate weights
	for (int m = 0; m < pow(shades_num, 3); m++) {
		vec_col.push_back(new my_Color(vec_shades[m / (shades_num*shades_num)],
								       vec_shades[(m % (shades_num*shades_num)) / shades_num],
			                           vec_shades[(m % shades_num) % shades_num]));
	}

	// sort colors based on weight
	sort(vec_col.begin(), vec_col.end(), colorComparison);

	// assign colors to planes
	for (int m = 0; m < input.size(); m++) { // for every cluster 
		for (int n = 0; n < input[m].size(); n++) { // colorise all points of this cluster to the same color
			input[m][n]->R = vec_col[m]->R;
			input[m][n]->G = vec_col[m]->G;
			input[m][n]->B = vec_col[m]->B;
		}
	}

	// assign black color to the last vector which represents not-compatible points
	for (int n = 0; n < input[input.size() - 1].size(); n++) { // colorise all points of this cluster to the same color
		input[input.size() - 1][n]->R = 0;
		input[input.size() - 1][n]->G = 0;
		input[input.size() - 1][n]->B = 0;
	}
}

int main()
{
	// Set path
	string folder_name = "C:\\Users\\user\\GoogleDisk\\Study\\Laser\\Laser_Exers\\Laser_Exer4\\Data\\";
	string file_name = "Charite-large";

	// read initial pointcloud
	cout << "Lets read point file:" << endl;
	vector<vector<Scan_Point*>> mat_p_scp = TXTReader::Read_point_PTX_to_matrix_of_pointers(folder_name + file_name + ".ptx");
	cout << mat_p_scp.size() << " x " << mat_p_scp[0].size() << " = " << mat_p_scp.size() * mat_p_scp[0].size() << " points read." << endl;

	// transform vector of vectors into a vector
	vector<Scan_Point*> vec_p_scp = Matrix_2D_to_1D(mat_p_scp);
	
	// delete zero-points from the vector
	Delete_zeros_1D(vec_p_scp);
	cout << vec_p_scp.size() << " points left after removal of zero-points. Lets ransac!" << endl;

	vector<vector<Scan_Point*>> set_of_planars;

	// use threshold values to detect vector of planes from 1D point cloud
	float thresh_area = 0.5;
	float thresh_resid = 0.1;
	float iter_ratio = 0.1; 
	float rest_ratio = 0.01; 
	float consen_ratio = 0.4;

	set_of_planars = setOfPlanarPointClouds(vec_p_scp, thresh_area, thresh_resid, iter_ratio, rest_ratio, consen_ratio);
	cout << "Number of planes found is " << set_of_planars.size() << ". Number of unassigned points is " << vec_p_scp.size() << endl;
	set_of_planars.push_back(vec_p_scp); // push back non-compatible points
	assignColorsToPlanars(set_of_planars); // colorize pixels of vector of planes
	vector<Scan_Point*> planars_vec = Matrix_2D_to_1D(set_of_planars); // transform it to 1D
	TXTWriter::WritePointPointersPTX(folder_name + file_name + "_result.ptx", planars_vec); // and save it to a file

	system("PAUSE");
	return 0;
}