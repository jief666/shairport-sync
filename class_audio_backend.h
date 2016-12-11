/*
 * class_audio_backend.h
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */

#ifndef CLASS_AUDIO_BACKEND_H_
#define CLASS_AUDIO_BACKEND_H_

#include "audio.h"
#include "jieftools/XString.h"
#include "jieftools/XObjArray.h"

class class_audio_backend;

typedef class_audio_backend *(NewAudioBackendInstanceFunctionType)(int argc, char **argv);
typedef void (audio_backend_help_function)(void);



class class_audio_backend_entry_points
{
  public:
	XString _ClassName;
	NewAudioBackendInstanceFunctionType* NewInstanceFunction;
	audio_backend_help_function* audio_backend_help_function_ptr;

	class_audio_backend_entry_points(const XString &aClassName, NewAudioBackendInstanceFunctionType *aNewInstanceFunction, audio_backend_help_function* a_audio_backend_help_function_ptr)
	{
		_ClassName = aClassName;
		NewInstanceFunction = aNewInstanceFunction;
		audio_backend_help_function_ptr = a_audio_backend_help_function_ptr;
	}
	virtual ~class_audio_backend_entry_points() {};
	virtual const XString& ClassName() const { return _ClassName; }
};


extern int register_audio_backend(class_audio_backend_entry_points *ClassObject);
class_audio_backend* new_instance_of_class_audio_backend(const XString& ClassName, int argc, char **argv);


class class_audio_backend {
public:
	static void ls_outputs(void);
	static const XString& get_defualt(void);

	class_audio_backend() {}; // default constructor
	class_audio_backend(int argc, char **argv);
	virtual ~class_audio_backend();

	const char* name;

	virtual void start(int sample_rate) {};
	virtual void stop(void) {};
	virtual void flush(void) {};
	virtual int delay(long* the_delay) { return 0; };
	virtual void play(short buf[], int samples) {};
	virtual void mute(int do_mute) {};
	virtual void volume(double vol) {};
	virtual void parameters(audio_parameters *info) {};

	virtual bool implementsDelay(void) { return false; };
	virtual bool implementsParameters(void) { return false; };


};

#endif /* CLASS_AUDIO_BACKEND_H_ */
