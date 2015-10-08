#pragma once

namespace Wish
{

	struct FrameInfo
	{
		//Last frame time
		unsigned long long frameDelta;

		//The time when this frame started
		unsigned long long frameTimestamp;

		//The frames per second count
		unsigned int fps;

		//Whether to perform fixed update this frame or not
		bool doFixedUpdate;

		//Whether to skip this frame or not
		bool skipFrame;

		//Simple Ctor
		FrameInfo() :
			frameDelta(0L),
			frameTimestamp(0L),
			fps(0),
			doFixedUpdate(false),
			skipFrame(false)
		{}

	};

}