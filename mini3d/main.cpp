#include<iostream>
#include <Eigen/Dense> //$(SolutionDir)..\TPL\eigen-3.4.0
#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

using namespace std;
using namespace Eigen;

// c++ style
int CMID(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }
float interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }

// matrix transform of eigen


struct transform_t
{

};


int main0()
{
	cout << "main" << endl;
	
	return 0;
}