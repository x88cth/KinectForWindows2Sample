// Standard Library
#include <iostream>

// OpenCV Header
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

// Kinect for Windows SDK Header
#include <Kinect.h>

using namespace std;

int main(int argc, char** argv)
{
	// 1a. Get default Sensor
	cout << "Try to get default sensor" << endl;
	IKinectSensor* pSensor = nullptr;
	if (GetDefaultKinectSensor(&pSensor) != S_OK)
	{
		cerr << "Get Sensor failed" << endl;
		return -1;
	}

	// 1b. Open sensor
	cout << "Try to open sensor" << endl;
	if (pSensor->Open() != S_OK)
	{
		cerr << "Can't open sensor" << endl;
		return -1;
	}

	// 2a. Get frame source
	cout << "Try to get color source" << endl;
	IColorFrameSource* pFrameSource = nullptr;
	if (pSensor->get_ColorFrameSource(&pFrameSource) != S_OK)
	{
		cerr << "Can't get color frame source" << endl;
		return -1;
	}

	// 2b. Get frame description
	cout << "get color frame description" << endl;
	int		iWidth = 0;
	int		iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	if (pFrameSource->get_FrameDescription(&pFrameDescription) == S_OK)
	{
		pFrameDescription->get_Width(&iWidth);
		pFrameDescription->get_Height(&iHeight);
	}
	pFrameDescription->Release();
	pFrameDescription = nullptr;

	// 3a. get frame reader
	cout << "Try to get color frame reader" << endl;
	IColorFrameReader* pFrameReader = nullptr;
	if (pFrameSource->OpenReader(&pFrameReader) != S_OK)
	{
		cerr << "Can't get color frame reader" << endl;
		return -1;
	}

	// 2c. release Frame source
	pFrameSource->Release();
	pFrameSource = nullptr;

	// CV.1 Prepare OpenCV data
	cv::Mat	mImg(iHeight,iWidth,CV_8UC4);
	UINT iBufferSize = iHeight * iWidth * mImg.elemSize();

	// Enter main loop
	while (true)
	{
		// 4a. Get last frame
		IColorFrame* pFrame = nullptr;
		if (pFrameReader->AcquireLatestFrame(&pFrame) == S_OK)
		{
			// 4c. Copy to OpenCV image
			pFrame->CopyConvertedFrameDataToArray(iBufferSize, mImg.data, ColorImageFormat_Bgra);
			cv::imshow("Color Map", mImg);

			// 4e. release frame
			pFrame->Release();
		}

		// 4f. check keyboard input
		if (cv::waitKey(30) == VK_ESCAPE){
			break;
		}
	}

	// 3b. release frame reader
	pFrameReader->Release();
	pFrameReader = nullptr;

	// 1c. Close Sensor
	pSensor->Close();

	// 1d. Release Sensor
	pSensor->Release();
	pSensor = nullptr;

	return 0;
}