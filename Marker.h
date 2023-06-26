#pragma once
#include <vector>
#include "eigen-3.4.0/Eigen/Dense"
#include <conio.h>
#include <iostream>
#include "NPTrackingTools.h"

using namespace std;
using namespace Eigen;
class Marker
{
public:
	double quaternian[4];
	Vector3f position;
	Matrix3f rotation;
	int count;
	double time = -1;
	void setPosition(float X,float Y,float Z) {
		position(0) = X *1000;
		position(1) = Y*1000;
		position(2) = Z*1000;



	}
	void getRotation() {

	}
	void getTime() {
		time = TT_FrameTimeStamp();
	}

	//void set_rotation(Vector4d quat) {
	//	cout << quat;
	//}
	//Matrix3f   get_quat(vector<float> rot) {
	//	int index = 0;

	//	for (int i = 0; i < 3, i++;) {
	//		for (int j = 0; j < 3; j++) {
	//			rotation[i][j] = rot[index];
	//			index++;
	//		}

	//	}
	// float data[] = {1,2,3,4};
	//Map<Vector3f> v1(data);       // uses v1 as a Vector3f object
	//Map<ArrayXf>  v2(data, 3);
	//	return rotation;

	//}




};

