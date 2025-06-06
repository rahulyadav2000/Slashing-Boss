#pragma once

#include "frame.h"

class Animation 
{
public:
	string name;
	list<Frame> frames;

	Animation(string name = "");

	int getNextFrameNumber(int frameNumber);
	Frame* getNextFrame(Frame* frame);
	int getEndFrameNumber();
	Frame* getFrame(int frameNumber);

	void loadAnimation(ifstream& file, list<DataGroupType>& groupTypes);
};