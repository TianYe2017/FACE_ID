#pragma once
#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H


#include "FACE.h"

extern cv::Mat background;
extern int flagCamera; ///0 = internal 1 = external
extern cv::Mat originalBuffer; ///global buffer to receive raw image from camera
extern std::vector<cv::Rect> detectedFaces; ///global buffer to store detected face from raw image
extern std::vector<FACE> stored_users; /// system buffer to store users
extern FACE* currentFace;
extern bool flagInitCreate;
extern bool flagStream;
extern bool flagValidConfig;
extern bool flagShot;
extern bool lockRun;
extern cv::VideoCapture* webcam;
extern bool flagRecShot;
extern cv::Mat stranger;
extern float thresholdSetting;


void globalEnableCamera(void);






#endif