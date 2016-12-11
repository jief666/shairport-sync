/*
 * class_audio_ao.h
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */

#ifndef CLASS_AUDIO_BACKEND_DUMMY_H_
#define CLASS_AUDIO_BACKEND_DUMMY_H_

#include "class_audio_backend.h"

class class_audio_backend_dummy : public class_audio_backend
{
protected:

public:
	class_audio_backend_dummy();
	class_audio_backend_dummy(int argc, char **argv);
	virtual ~class_audio_backend_dummy();

	virtual void start(int sample_rate);
	virtual void stop(void);
	virtual void play(short buf[], int samples);

};

#endif /* CLASS_AUDIO_BACKEND_DUMMY_H_ */
