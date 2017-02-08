/*
 * class_audio_ao.h
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */
#ifndef CLASS_AUDIO_BACKEND_AO_H_
#define CLASS_AUDIO_BACKEND_AO_H_


#include "class_audio_backend.h"
#include "ao/ao.h"



class class_audio_backend_ao : public class_audio_backend
{
protected:
	ao_device *dev;

public:
	class_audio_backend_ao();
	class_audio_backend_ao(int argc, char **argv);
	virtual ~class_audio_backend_ao();

	static void help(void);

	virtual void start(int sample_rate);
	virtual void play(short buf[], int samples);

};

#endif /* CLASS_AUDIO_BACKEND_H_ */
