#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

#include <memory>
#include <string>
#include <stdexcept>
#include <thread>

const std::string frameDir = ("./Frames/Frame_");
const auto videoDir = "..//TestVideo.mp4";
const int apiID = cv::CAP_FFMPEG;

void ProcessFrames(int count, float mod, float mod2)
{
	const int start = count * mod;
	const int end = count * mod2;

	cv::VideoCapture cap(videoDir, apiID);
	cv::Mat frame;
	cap.set(cv::CAP_PROP_POS_FRAMES, start);

	//cap >> frame;
	for (int index = start; index < end; ++index)
	{

		// If the frame is empty, break immediately
		if (!cap.grab())
			break;

		cap.retrieve(frame);

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		imwrite(frameDir + std::to_string(index) + ".png", frame);
	}
	cap.release();
}


int main()
{
	// open selected camera using selected API
	cv::VideoCapture cap(videoDir, apiID);
	cv::Mat frame;
	const double framecounttotal = cap.get(cv::CAP_PROP_FRAME_COUNT);

	// This thread is launched by using
	// function pointer as callable
	std::thread th2(ProcessFrames, framecounttotal, 0.2f, 0.4f);
	std::thread th3(ProcessFrames, framecounttotal, 0.4f, 0.6f);
	std::thread th4(ProcessFrames, framecounttotal, 0.6f, 0.8f);
	std::thread th1(ProcessFrames, framecounttotal, 0.8f, 1.0f);

	//cap >> frame;
	for (int index = 0; index < framecounttotal * 0.2f; ++index)
	{
		// If the frame is empty, break immediately
		cap.grab();
		cap.retrieve(frame);

		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		imwrite(frameDir + std::to_string(index) + ".png", frame);
	}
	cap.release();


	th1.join();
	th2.join();
	th3.join();
	th4.join();

	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}