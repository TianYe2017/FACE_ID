#pragma once
#ifndef FACE_H
#define FACE_H

#include "common_header.h"

using namespace std;

class FACE 
{
public:
	FACE(void);
	void createHeader(char* _userId, char* _userDescription, int _sizeOfCodebook, int _dimOfVector);
	void writeDat(void);
	void loadDatFromFile(char* path);
	void printDat(void);
	void createDemo(void);
	void shotASample(cv::Mat image);
	void configureSetting(char* _userId, char* _userDescription, int _dimOfVector, int _sizeOfCodebook);
	void train(void);
	float computeScore(cv::Mat img);
	void reset(void);
	void goBack(void);
	int checkNumberOfSamples(void);
	float getAverageScore(void);
	char* getId(void);

private:

	char userId[100] = "";
	char userDescription[500] = "";
	int sizeOfCodebook = 179;
	int numberOfSample = 0;
	int dimOfVector = 4;
	vector<vector<float>> codeBook;
	vector<float> refScore;
	vector<vector<float>> histograms;
	vector<cv::Mat> samples;

	bool overwriteAccess = true;
	bool readyToComputeCodebook = false;
	cv::Mat image2Blocks(vector<cv::Mat> imgs_in);
	void computeCenters(cv::Mat data);
	float computeL1Distance(vector<float>v1, vector<float>v2);
	float computeL2Distance(vector<float>v1, vector<float>v2);
	vector<float> computeHistogram(cv::Mat block, vector<vector<float>> codeBook);
	void computeRefScore(vector<cv::Mat> imgs, vector<vector<float>> codeBook);
	
	
};

#endif