#include "stdafx.h"
#include "FACE.h"



FACE::FACE(void) 
{
	codeBook.clear();
	refScore.clear();
}

void FACE::writeDat(void) 
{
	_cprintf("\n\nWriting dat...\n\n");
	//compute how much space in byte does .dat needs
	int N = 100 + 500 + 3 + 4 * dimOfVector * sizeOfCodebook + 4 * numberOfSample + 4 * numberOfSample * sizeOfCodebook;
	unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * N);
	//load userId to buffer
	unsigned char buffer_userId[100];
	memcpy(buffer_userId, userId, 100);
	for (int i = 0; i < 100; i++) 
	{
		buffer[i] = buffer_userId[i];
	}
	//load userDescription to buffer
	unsigned char buffer_userDescription[500];
	memcpy(buffer_userDescription,userDescription,500);
	for (int i = 0; i < 500; i++) 
	{
		buffer[i + 100] = buffer_userDescription[i];
	}
	//load dimOfVetcor,sizeOfCodebook,numOfSample to buffer
	unsigned char buffer_dimOfVector = (unsigned char)dimOfVector;
	unsigned char buffer_sizeOfCodebook = (unsigned char)sizeOfCodebook;
	unsigned char buffer_numberOfSample = (unsigned char)numberOfSample;
	//load codeBook to buffer
	unsigned char* buffer_codeBook = (unsigned char*)malloc(sizeof(unsigned char) * 4 * dimOfVector * sizeOfCodebook);
	unsigned char buffer_float[4];
	float value;
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		for (int j = 0; j < dimOfVector; j++) 
		{
			value = codeBook[i][j];
			memcpy(buffer_float,&value,4);
			for (int k = 0; k < 4; k++) 
			{
				buffer_codeBook[ (dimOfVector * i + j) * 4 + k] = buffer_float[k];
			}
		}
	}
	//load refScore to buffer
	unsigned char* buffer_refScore = (unsigned char*)malloc(sizeof(unsigned char) * 4 * numberOfSample);
	for (int i = 0; i < numberOfSample; i++) 
	{
		value = refScore[i];
		memcpy(buffer_float, &value, 4);
		for (int k = 0; k < 4; k++) 
		{
			buffer_refScore[4 * i + k] = buffer_float[k];
		}
	}
	//load histograms to buffer
	unsigned char* buffer_histograms = (unsigned char*)malloc(sizeof(unsigned char) * 4 * numberOfSample * sizeOfCodebook);
	for (int i = 0; i < numberOfSample; i++) 
	{
		for (int j = 0; j < sizeOfCodebook; j++) 
		{
			value = histograms[i][j];
			memcpy(buffer_float, &value, 4);
			for (int k = 0; k < 4; k++) 
			{
				buffer_histograms[(sizeOfCodebook * i + j) * 4 + k] = buffer_float[k];
			}
		}
	}
	//load everything to main buffer;
	for (int i = 0; i < 100; i++) 
	{
		buffer[i] = buffer_userId[i];
	}
	for (int i = 0; i < 500; i++) 
	{
		buffer[i + 100] = buffer_userDescription[i];
	}
	buffer[600] = buffer_dimOfVector;
	buffer[601] = buffer_sizeOfCodebook;
	buffer[602] = buffer_numberOfSample;
	for (int i = 0; i < 4 * dimOfVector * sizeOfCodebook; i++) 
	{
		buffer[i + 603] = buffer_codeBook[i];
	}
	for (int i = 0; i < 4 * numberOfSample; i++) 
	{
		buffer[i + 603 + 4 * dimOfVector * sizeOfCodebook] = buffer_refScore[i];
	}
	for (int i = 0; i < 4 * sizeOfCodebook*numberOfSample; i++) 
	{
		buffer[i + 603 + 4 * dimOfVector * sizeOfCodebook + 4 * numberOfSample] = buffer_histograms[i];
	}
	//write .dat file
	char path[300] = "./data/";
	strcat(path,userId);
	strcat(path, ".dat");

	FILE* p = fopen(path,"wb");
	if (p == NULL) 
	{
		_cprintf("\n\nError: Can not create file... Data is not stored.\n\n");
	}
	else 
	{
		fwrite(buffer,sizeof(unsigned char),N,p);
	}
	fclose(p);

	free(buffer);
	free(buffer_codeBook);
	free(buffer_refScore);
	free(buffer_histograms);
	_cprintf("\n\nDone writing.\n\n");
}

void FACE::loadDatFromFile(char* path)
{
	unsigned char header[603];
	FILE *p = fopen(path, "rb");
	if (p == NULL)
	{
		_cprintf("\n\nFail to read file.");
	}
	else
	{
		fread(header, sizeof(unsigned char), 603, p);
	}
	fclose(p);
	unsigned char buffer_userId[100];
	unsigned char buffer_userDescription[500];
	for (int i = 0; i < 100; i++) 
	{
		buffer_userId[i] = header[i];
	}
	for (int i = 0; i < 500; i++) 
	{
		buffer_userDescription[i] = header[i + 100];
	}
	memcpy(userId, buffer_userId, 100);
	memcpy(userDescription, buffer_userDescription, 500);
	dimOfVector = (int)header[600];
	sizeOfCodebook = (int)header[601];
	numberOfSample = (int)header[602];
	

	int N = 100 + 500 + 3 + 4 * dimOfVector * sizeOfCodebook + 4 * numberOfSample + 4 * numberOfSample * sizeOfCodebook;
	unsigned char* buffer = (unsigned char*)malloc(sizeof(unsigned char) * N);
	FILE *q = fopen(path, "rb");
	if (q == NULL)
	{
		_cprintf("\n\nFail to read file_2.\n\n");
	}
	else
	{
		fread(buffer, sizeof(unsigned char), N, q);
	}
	fclose(q);
	float** buffer_codebook = (float**)malloc(sizeof(float*)*sizeOfCodebook);
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		buffer_codebook[i] = (float*)malloc(sizeof(float)*dimOfVector);
	}
	unsigned char buffer_float[4];
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		for (int j = 0; j < dimOfVector; j++) 
		{
			int index = (dimOfVector * i + j) * 4;
			buffer_float[0] = buffer[603 + index];
			buffer_float[1] = buffer[604 + index];
			buffer_float[2] = buffer[605 + index];
			buffer_float[3] = buffer[606 + index];
			memcpy(&buffer_codebook[i][j], buffer_float, 4);
		}
	}
	vector<float> tmp;
	for (int i = 0; i < sizeOfCodebook; i++)
	{			
		for (int j = 0; j < dimOfVector; j++)
		{
			tmp.push_back(buffer_codebook[i][j]);
		}
		codeBook.push_back(tmp);
		tmp.clear();
	}
	float* buffer_refScore = (float*)malloc(sizeof(float)*numberOfSample);
	for (int i = 0; i < numberOfSample; i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			buffer_float[j] = buffer[603 + 4 * dimOfVector * sizeOfCodebook + 4 * i + j];
		}
		memcpy(&buffer_refScore[i], buffer_float, 4);
	}
	for (int i = 0; i < numberOfSample; i++) 
	{
		refScore.push_back(buffer_refScore[i]);
	}
	float** buffer_histogram = (float**)malloc(sizeof(float*)*numberOfSample);
	for (int i = 0; i < numberOfSample; i++) 
	{
		buffer_histogram[i] = (float*)malloc(sizeof(float)*sizeOfCodebook);
	}
	int offset = 100 + 500 + 3 + 4 * sizeOfCodebook * dimOfVector + 4 * numberOfSample;
	for (int i = 0; i < numberOfSample; i++) 
	{
		for (int j = 0; j < sizeOfCodebook; j++) 
		{
			int index = (sizeOfCodebook * i + j) * 4;
			buffer_float[0] = buffer[0 + offset + index];
			buffer_float[1] = buffer[1 + offset + index];
			buffer_float[2] = buffer[2 + offset + index];
			buffer_float[3] = buffer[3 + offset + index];
			memcpy(&buffer_histogram[i][j], buffer_float, 4);
		}
	}
	for (int i = 0; i < numberOfSample; i++)
	{
		for (int j = 0; j < sizeOfCodebook; j++)
		{
			tmp.push_back(buffer_histogram[i][j]);
		}
		histograms.push_back(tmp);
		tmp.clear();
	}
	free(buffer);
	free(buffer_codebook);
	free(buffer_refScore);
	free(buffer_histogram);
}

void FACE::printDat(void) 
{
	_cprintf("\nuserId: %s", userId);
	_cprintf("\nuserDescription: %s", userDescription);
	_cprintf("\ndimOfVector: %d", dimOfVector);
	_cprintf("\nsizeOfCodebook: %d", sizeOfCodebook);
	_cprintf("\numberOfSample: %d", numberOfSample);
	_cprintf("\ncodeBook: \n");
	for (int i = 0; i < sizeOfCodebook; i++)
	{
		for (int j = 0; j < dimOfVector; j++)
		{		
			_cprintf("%f ", codeBook[i][j]);
		}
		_cprintf("\n");		
	}
	_cprintf("refScore: \n");
	for (int i = 0; i < numberOfSample; i++)
	{
		_cprintf("%f ",refScore[i]);	
	}
	_cprintf("\n");
	_cprintf("histograms: \n");
	for (int i = 0; i < numberOfSample; i++)
	{
		for (int j = 0; j < sizeOfCodebook; j++)
		{
			_cprintf("%f ", histograms[i][j]);
		}
		_cprintf("\n");
	}
}

void FACE::createDemo(void) 
{
	strcat(userId, "TIAN YE");
	strcat(userDescription, "DEVELOPER");
	dimOfVector = 2;
	sizeOfCodebook = 3;
	numberOfSample = 4;
	vector<float> tmp;
	tmp.push_back(1.0f);
	tmp.push_back(2.0f);
	codeBook.push_back(tmp);
	tmp[0] = 3.0f;
	tmp[1] = 4.0f;
	codeBook.push_back(tmp);
	tmp[0] = 5.0f;
	tmp[1] = 6.0f;
	codeBook.push_back(tmp);
	refScore.push_back(1.0f);
	refScore.push_back(2.0f);
	refScore.push_back(3.0f);
	refScore.push_back(4.0f);
	tmp.clear();
	tmp.push_back(0.1f);
	tmp.push_back(0.1f);
	tmp.push_back(0.1f);
	histograms.push_back(tmp);
	tmp[0] = 0.2f;
	tmp[1] = 0.2f;
	tmp[2] = 0.2f;
	histograms.push_back(tmp);
	tmp[0] = 0.3f;
	tmp[1] = 0.3f;
	tmp[2] = 0.3f;
	histograms.push_back(tmp);
	tmp[0] = 0.4f;
	tmp[1] = 0.4f;
	tmp[2] = 0.4f;
	histograms.push_back(tmp);
}

void FACE::shotASample(cv::Mat image)
{	
	cv::Mat cache;
	image.convertTo(cache,CV_32F);
	if (overwriteAccess == true) 
	{
		samples.push_back(cache);
		numberOfSample++;
	}
	else 
	{
		samples.clear();
		samples.push_back(cache);
		overwriteAccess = true;
		numberOfSample = 0;
		numberOfSample++;
	}
	if (numberOfSample >= 10) 
	{
		readyToComputeCodebook = true;
	}
}

void FACE::configureSetting(char* _userId, char* _userDescription, int _dimOfVector, int _sizeOfCodebook) 
{	
	memset(userId,0,sizeof(userId));
	strcat(userId, _userId);
	memset(userDescription, 0, sizeof(userDescription));
	strcat(userDescription, _userDescription);
	dimOfVector = _dimOfVector;
	sizeOfCodebook = _sizeOfCodebook;
}

cv::Mat FACE::image2Blocks(vector<cv::Mat> imgs_in) 
{
	printf("\n\npreprocessing...\n\n");

	int N = imgs_in.size();
	//compute size of output matrix;
	int count = 0;
	int dim2 = (int)sqrt(dimOfVector);
	int offset = dim2 - 1;

	for (int i = 0; i < N; i++) 
	{
		count += (imgs_in[i].rows - offset) * (imgs_in[i].cols - offset);
	}
	//allocate mat for output
	cv::Mat outMat = cv::Mat(count,dimOfVector,CV_32F,cv::Scalar(0.0f));
	count = 0;
	cv::Mat blur_img;
	float min = 99999.9f;
	for (int i = 0; i < N; i++) 
	{
		cv::blur(imgs_in[i],blur_img,cv::Size(dim2, dim2));
		for (int row = 0; row < imgs_in[i].rows - offset; row++) 
		{
			for (int col = 0; col < imgs_in[i].cols - offset; col++) 
			{
				
				for (int r = 0; r < dim2; r++)
				{
					for (int c = 0; c < dim2; c++)
					{
						outMat.at<float>(count, r * dim2 + c) = blur_img.at<float>(row + r, col + c);
						//outMat.at<float>(count, r * dim2 + c) = imgs_in[i].at<float>(row + r, col + c);					
					}
				}
				//substract min in block
				min = 99999.9f;
				for (int s = 0; s < dimOfVector; s++)
				{
					if (outMat.at<float>(count, s) < min)
					{
						min = outMat.at<float>(count, s);
					}
				}
				for (int s = 0; s < dimOfVector; s++)
				{
					outMat.at<float>(count, s) -= min;
				}
				count++;
			}
		}
	}
	return outMat;

}

void FACE::computeCenters(cv::Mat data) 
{	
	printf("\n\nk_means ing...\n\n");
	codeBook.clear();
	cv::Mat centers,labels;
	cv::TermCriteria criteria = cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 400, 0.1);
	cv::kmeans(data,sizeOfCodebook,labels, criteria,3,2,centers);
	vector<float>tmp;
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		tmp.clear();
		for (int j = 0; j < dimOfVector; j++) 
		{
			tmp.push_back(centers.at<float>(i,j));
		}
		codeBook.push_back(tmp);
	}

}

float FACE::computeL1Distance(vector<float>v1, vector<float>v2) 
{
	/*if (v1.size() != v2.size()) 
	{
		cout << "v1: " << v1.size() << " v2: " << v2.size() << endl;
		while (1);
	}*/
	float outValue = 0.0f;
	for (int i = 0; i < v1.size(); i++) 
	{
		outValue += abs(v1[i] - v2[i]);
	}
	return outValue;
}

float FACE::computeL2Distance(vector<float> v1, vector<float> v2) 
{
	/*if (v1.size() != v2.size())
	{
		cout << "v1: " << v1.size() << " v2: " << v2.size() << endl;
		while (1);
	}*/
	float outValue = 0.0f;
	for (int i = 0; i < v1.size(); i++) 
	{
		outValue += (v1[i] - v2[i]) * (v1[i] - v2[i]);
	}
	outValue = sqrt(outValue);
	return outValue;
}

vector<float> FACE::computeHistogram(cv::Mat block, vector<vector<float>> codeBook) 
{
	/*vector<vector<float>> debug;
	for (int i = 0; i < block.rows; i++) 
	{
		vector<float> t;
		for (int j = 0; j < block.cols; j++) 
		{
			t.push_back(block.at<float>(i, j));
		}	
		debug.push_back(t);
		t.clear();
	}*/
	vector<float> outVector;
	outVector.clear();
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		outVector.push_back(0.0f);
	}
	int index = 0;
	int distance = 99999.9f;
	vector<float>tmp;
	
	//for (int i = 0; i < ; i++)
	for (int i = 0; i < block.rows; i++)
	{
		index = 0;
		distance = 99999.9f;
		tmp.clear();
		for (int j = 0; j < dimOfVector; j++) 
		{
			tmp.push_back(block.at<float>(i,j));
		}
		for (int j = 0; j < sizeOfCodebook; j++) 
		{
			float dis = computeL2Distance(tmp, codeBook[j]);
			if (dis < distance)
			{
				index = j;
				distance = dis;
			}
		}
		/*if (index < 0 | index >= 179) 
		{
			std::cout << index << endl;
			while (1);
		}*/
		outVector[index] += 1.0f;
	}
	for (int i = 0; i < sizeOfCodebook; i++) 
	{
		outVector[i] /= (float)block.rows;
	}
	//cout << "done." << endl;
	//while (1);
	return outVector;

}

void FACE::computeRefScore(vector<cv::Mat> imgs, vector<vector<float>> codeBook) 
{
	_cprintf("\n\ngenerate refScore...total number: ");
	_cprintf("%d\n\n",imgs.size());
	vector<cv::Mat> cache;
	histograms.clear();
	for (int i = 0; i < imgs.size(); i++) 
	{
		_cprintf("\nprocessing %d sample...\n", i + 1);
		cache.clear();
		cache.push_back(imgs[i]);
		cv::Mat blocks = FACE::image2Blocks(cache);
		histograms.push_back(FACE::computeHistogram(blocks, codeBook));
	}
	refScore.clear();
	for (int i = 0; i < imgs.size(); i++) 
	{
		refScore.push_back(0.0f);
	}
	float sum = 0.0f;
	for (int i = 0; i < imgs.size(); i++) 
	{
		sum = 0.0f;
		for (int j = 0; j < imgs.size(); j++) 
		{
			if( i != j)
			{
				sum += computeL1Distance(histograms[i], histograms[j]);
			}			
		}
		refScore[i] = sum / (imgs.size() - 1);
	}
	_cprintf("\n\ngenerate refScore finished.\n\n");
}

void FACE::train(void) 
{
	
	computeCenters(image2Blocks(samples));
	computeRefScore(samples, codeBook);
	writeDat();
}

float FACE:: computeScore(cv::Mat img) 
{
	vector<cv::Mat> cache;
	cache.clear();
	cache.push_back(img);
	cv::Mat blocks = FACE::image2Blocks(cache);
	vector<float> histogram = FACE::computeHistogram(blocks,codeBook);
	float sum = 0.0f;
	vector<float> debug;
	for (int i = 0; i < numberOfSample; i++) 
	{
		debug.push_back(computeL1Distance(histogram, histograms[i]));
		sum += computeL1Distance(histogram,histograms[i]);
	}
	sum /= numberOfSample;
	_cprintf("\n\n\n\n\n");
	_cprintf("\nscore: \n");
	for (int i = 0; i < numberOfSample; i++) 
	{
		_cprintf("%f ",debug[i]);
		//cout << " " << debug[i];
	}
	_cprintf("\n\navgScore: %f\n\n",sum);
	return sum;
}

void FACE::reset(void) 
{
	if (AfxMessageBox(_T("Do You Really Want To Start Over?"), MB_OKCANCEL, MB_ICONEXCLAMATION) == 1) 
	{
		memset(userId, 0, sizeof(userId));
		memset(userDescription, 0, sizeof(userDescription));
		sizeOfCodebook = 0;
		numberOfSample = 0;
		dimOfVector = 0;
		codeBook.clear();
		refScore.clear();
		histograms.clear();
		samples.clear();
		overwriteAccess = false;
		readyToComputeCodebook = false;
		_cprintf("\n\nuser is reset\n\n");
	}
	else 
	{
		_cprintf("\n\noperation cancelled.\n\n");
	}
}

void FACE::createHeader(char* _userId, char* _userDescription, int _sizeOfCodebook, int _dimOfVector) 
{
	memset(userId, 0, sizeof(userId));
	memset(userDescription, 0, sizeof(userDescription));
	strcat(userId, _userId);
	strcat(userDescription, _userDescription);
	sizeOfCodebook = _sizeOfCodebook;
	dimOfVector = _dimOfVector;
	_cprintf("\n\n update current user's configuration to: \n");
	_cprintf("userId: %s\nuserDescription: %s\ndimOfVector: %d\nsizeOfCodebook: %d\n\n", userId, userDescription, dimOfVector, sizeOfCodebook);

}

void FACE::goBack(void) 
{
	if (numberOfSample > 0) 
	{
		numberOfSample--;
		samples.pop_back();
	}
}

int FACE::checkNumberOfSamples(void) 
{
	return numberOfSample;
}

float FACE::getAverageScore(void) 
{
	float sum = 0.0f;
	for (int i = 0; i < refScore.size();i++)
	{
		sum += refScore[i];
	}
	sum = sum / refScore.size();
	return sum;
}

char* FACE::getId(void) 
{
	return userId;
}