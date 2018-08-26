#include "stdafx.h"
#include "global_var.h"


int flagCamera = 0;
cv:: Mat originalBuffer;
std::vector<cv::Rect> detectedFaces;
std::vector<FACE> stored_users;
bool flagInitCreate = false;
FACE* currentFace = NULL;
bool flagStream = false;
cv::VideoCapture* webcam = NULL;
bool flagShot = false;
bool flagRecShot = false;
cv::Mat stranger;
float thresholdSetting = 0.0f;

cv::Mat background = cv::imread("./BACK.png");

void globalEnableCamera(void) 
{
	flagStream = true;
	cv::Mat grayBuffer;
	cv::Mat copy;
	cv::Mat optBuffer;
	cv::CascadeClassifier face;
	face.load("./haarcascade_frontalface_default.xml");
	cv::namedWindow("LIVE STREAM");


	std::vector<cv::Rect> detected_Faces;
	webcam = new cv::VideoCapture(flagCamera);
	webcam->set(CV_CAP_PROP_FRAME_WIDTH, 320);
	webcam->set(CV_CAP_PROP_FRAME_HEIGHT,240);

	while (true) 
	{
		cv::waitKey(5);
		while (flagStream) 
		{
			//*webcam >> originalBuffer;
			webcam->read(originalBuffer);
			copy = originalBuffer;
			double rows = (double)originalBuffer.rows;
			double cols = (double)originalBuffer.cols;
			//face detection
			if (originalBuffer.channels() == 3)
			{
				cv::cvtColor(originalBuffer, grayBuffer, CV_RGB2GRAY);
			}
			else if (originalBuffer.channels() == 4)
			{
				cv::cvtColor(originalBuffer, grayBuffer, CV_RGBA2BGR);
			}
			else
			{
				grayBuffer = originalBuffer;
			}
			cv::equalizeHist(grayBuffer, optBuffer);
			detectedFaces.clear();
			face.detectMultiScale(optBuffer, detectedFaces, 1.1f, 3);
			int area = -1;
			int index = 0;
			for (int i = 0; i < detectedFaces.size(); i++)
			{
				if (detectedFaces[i].height > 0 && detectedFaces[i].width > 0)
				{
					if (detectedFaces[i].area() > area)
					{
						index = i;
						area = detectedFaces[i].area();
					}
				}
			}
			if (area > 0)
			{
				cv::rectangle(copy, detectedFaces[index], cv::Scalar(255, 255, 0), 2, 8, 0);
			}		
			cv::resize(copy,copy,cv::Size(copy.cols * 2,copy.rows * 2),cv::INTER_LINEAR);		
			cv::imshow("LIVE STREAM", copy);
			cv::waitKey(5);
			if (flagShot) 
			{
				cv::Mat face;
				cv::Mat cand;
				optBuffer(detectedFaces[index]).copyTo(face);			
				_cprintf("\n\nwriting extracted face image...\n\n");
				cv::imwrite("./tmp/face.png",face);
				cv::waitKey(20);
				flagShot = false;
				//resize and store
				double rows = (double)face.rows;
				double cols = (double)face.cols;
				cv::resize(face,cand,cv::Size(floor(rows / cols * 100.0),100), cv::INTER_LINEAR);
				currentFace->shotASample(cand);
			}
			if (flagRecShot) 
			{
				cv::Mat face;
				optBuffer(detectedFaces[index]).copyTo(face);
				cv::imwrite("./tmp/stranger.png", face);
				//cv::waitKey(20);
				flagRecShot = false;
				//resize and store
				double rows = (double)face.rows;
				double cols = (double)face.cols;
				cv::resize(face, stranger, cv::Size(floor(rows / cols * 100.0), 100), cv::INTER_LINEAR);				
				_cprintf("\n\nstranger is in memory now.\n\n");
			}
		}
	}
}