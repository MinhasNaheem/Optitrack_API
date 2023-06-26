// OptitrackCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <conio.h>
#include <chrono>
#include <typeinfo>
#include <cstdint>
#include <vector>
#include <array>
#include "NPTrackingTools.h"
#include <thread>
#include <mutex>
#include "eigen-3.4.0/Eigen/Dense"
#include "Marker.h"
#define 	NPRESULT_SUCCESS     		0
#define 	NPRESULT_FILENOTFOUND           1
#define 	NPRESULT_LOADFAILED		2
#define 	NPRESULT_FAILED			3
#define 	NPRESULT_INVALIDFILE		8
#define 	NPRESULT_INVALIDCALFILE		9
#define 	NPRESULT_UNABLETOINITIALIZE	10
#define 	NPRESULT_INVALIDLICENSE		11
#define 	NPRESULT_NOFRAMEAVAILABLE	14
#define 	NPRESULT_DEVICESINUSE		15
#define 	NPVIDEOTYPE_SEGMENT	0
#define 	NPVIDEOTYPE_GRAYSCALE	1
#define 	NPVIDEOTYPE_OBJECT	2
#define 	NPVIDEOTYPE_PRECISION	4
#define 	NPVIDEOTYPE_MJPEG	6
#define 	NPVIDEOTYPE_VIDEO	9
using namespace std;
using namespace chrono;
using namespace Eigen;




bool initialize = false;
mutex m;

class CameraSettings
{
    
public:
    int cameraIndex;
    int result1;
    int camCount = TT_CameraCount();
    double rot[3][3];
    int result2;
    int result3;
    int result4;
    int result5;
    int result6;


public:
    void display_settings()
    {
        for (int i = 0; i < TT_CameraCount(); i++) {
            cameraIndex = i;
            result1 = cameraIndex;
            result2 = TT_CameraVideoType(cameraIndex);    // Returns Video Type
            result3 = TT_CameraExposure(cameraIndex);    // Returns Camera Exposure
            result4 = TT_CameraThreshold(cameraIndex);  // Returns Pixel Threshold
            result5 = TT_CameraIntensity(cameraIndex);  // Returns IR Illumination Intensity
            result6 = TT_CameraFrameRate(cameraIndex);  // Returns Camera Frame Rate
            float TT_CameraOrientationMatrix(int cameraIndex, int matrixIndex);
            //cout << TT_GetResultString(result2) << " : video type";
            //std::cout << "cameraIndex" << TT_GetResultString(result1);
            



        }
       
    }
public:
    void set_camera(int cameraIndex, int videoType, int exposure, int threshold, int intensity)
    {
        TT_SetCameraSettings(cameraIndex, videoType, exposure, threshold, intensity);
    }
    vector<float> fetchCameraLocation() {
        vector<float> camPosition[2];
        

        for (int i = 0; i < camCount; i++)
        {
            float camX = TT_CameraXLocation(i);
            float camY = TT_CameraYLocation(i);
            float camZ = TT_CameraZLocation(i);

            //printf("Camera %d: (%f, %f, %f)\n", i, camX, camY, camZ);
            camPosition[i] = { camX,camY,camZ };


        }
        return camPosition[0];


    }
    
    vector<float> insertAtEnd(vector<float> A, float e) {
        A.push_back(e);
        return A;
    }


    
    
    vector<float> GetCameraOrientation(int camIndex) {
        vector<float>  rot_g2cam;
        int matrixIndex;
        for (int j = 0; j < 9; j++)
            matrixIndex = j;

        float val = TT_CameraOrientationMatrix(camIndex, matrixIndex);
        rot_g2cam = insertAtEnd(rot_g2cam, val);
        cout << val << "matrix\n";

    }
};







void connection() {
    
    while (!initialize)
    {
        
       
        if (TT_Update() == NPRESULT_SUCCESS) {
             m.lock();
            initialize = true;
            cout << "initialze: " << initialize;
            
            m.unlock();
        }
        else {
            cout << "check cable connection\n";
        }

       
        
    }
    
}



int main()
{
    const char* profile = "\\x64\\Debug\\Profile\\profile.motive";
    const char* cal = "\\x64\\Debug\\Profile\\calibration.cal";
    int Opti_status = TT_Initialize();
    const char* TT_GetResultString(NPRESULT result); // Returns text of detail information on the result.
    
    cout << Opti_status;
    TT_LoadProfile(profile);
    TT_LoadCalibration(cal);
    int camCount;
    int port = 8080;
    bool enablePublish = true;
    vector<float> camPos;
    TT_SaveRigidBodies("x64\\Debug\\Profile\\Current_tool.tra");

    
    CameraSettings settings ;
    //initialize = connection();
    thread connectionThread(connection);
    //connectionThread.join()
    cout << connectionThread.get_id() << ":thread id \n";
    TT_StreamVRPN(enablePublish, port);
    


    while (!_kbhit())
    {


        if (TT_UpdateSingleFrame() == NPRESULT_SUCCESS and initialize and Opti_status == 0)
        {
            // Each time the TT_Update function successfully updates the frame,
            // the frame counter is incremented, and the new frame is processed.
            //cout << "1\n";
            //settings.display_settings();
            camCount = TT_CameraCount();
            settings.camCount = camCount;
            camPos = settings.fetchCameraLocation();
            int totalMarker = TT_FrameMarkerCount();
            printf(" (Markers: %d)\n", totalMarker);
            //== Save Rigid Bodies ==/
            
            Marker trackTool;
            


            //== Use a loop to access every marker in the frame ==//
            for (int i = 0; i < totalMarker; i++) {
               
                trackTool.setPosition(TT_FrameMarkerX(i), TT_FrameMarkerY(i), TT_FrameMarkerZ(i));
                
                trackTool.getTime();
                
                printf(" (Time : %f)\n", trackTool.time);

                     

            }
            
            
            

            //cout << totalMarker << "marker count";
            //cout << camCount << "camera count ";
            //cout << camPos << "CameraPosition\n";
            continue;




        }
    }


    int status = 0;
    //cout << active_status;




    TT_Shutdown();

    return 0;

}

