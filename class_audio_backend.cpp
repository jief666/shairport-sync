/*
 * class_audio_backend.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: jief
 */

#include "class_audio_backend.h"




ObjArray<class_audio_backend_entry_points> *audio_backend_entry_points_array = NULL;

class class_delete_audio_backend_entry_points_array
{
  public:
	~class_delete_audio_backend_entry_points_array() { delete audio_backend_entry_points_array; }
};

class_delete_audio_backend_entry_points_array delete_audio_backend_entry_points_array;



int register_audio_backend(class_audio_backend_entry_points *audio_backend_entry_points)
{
	if ( !audio_backend_entry_points_array ) audio_backend_entry_points_array = new ObjArray<class_audio_backend_entry_points>;
	audio_backend_entry_points_array->AddRef(audio_backend_entry_points, true);
	return 0;
}

const class_audio_backend_entry_points* get_audio_backend_entry_points_ptr(const XString& ClassName)
{
  unsigned int ui;

	if ( !audio_backend_entry_points_array ) return NULL;

	if ( ClassName.BeginEqual("class ") )
	{
		for ( ui=0 ; ui<audio_backend_entry_points_array->Length()  &&  !ClassName.SubStringEqual( 6, (*audio_backend_entry_points_array)[ui].ClassName() )  ; ui+=1 ) ;
	}else{
		for ( ui=0 ; ui<audio_backend_entry_points_array->Length()  &&  !ClassName.Equal( (*audio_backend_entry_points_array)[ui].ClassName() )  ; ui+=1 ) ;
	}
	if ( ui<audio_backend_entry_points_array->Length()  &&  ClassName.Equal( (*audio_backend_entry_points_array)[ui].ClassName() ) )
	{
		return &(*audio_backend_entry_points_array)[ui];
	}
	return NULL;
}

class_audio_backend* new_instance_of_class_audio_backend(const XString& ClassName, int argc, char **argv)
{
	const class_audio_backend_entry_points* audio_backend_entry_points = get_audio_backend_entry_points_ptr(ClassName);
	if ( !audio_backend_entry_points ) return NULL;
	return audio_backend_entry_points->NewInstanceFunction(argc, argv);
}




void class_audio_backend::ls_outputs(void)
{
  printf("Available audio outputs:\n");
  for (int idx = 0 ; idx < audio_backend_entry_points_array->Length() ; idx+=1) {
    printf("    %s%s\n", (*audio_backend_entry_points_array)[idx]._ClassName.c, idx == 0 ? " (default)" : "");
  }
  for (int idx = 0 ; idx < audio_backend_entry_points_array->Length() ; idx+=1)
  {
    printf("\n");
    if ( (*audio_backend_entry_points_array)[idx].audio_backend_help_function_ptr != NULL ) {
        printf("Options for output %s:\n", (*audio_backend_entry_points_array)[idx]._ClassName.c);
    	(*audio_backend_entry_points_array)[idx].audio_backend_help_function_ptr();
    }else{
        printf("No options for output %s:\n", (*audio_backend_entry_points_array)[idx]._ClassName.c);
    }
  }
}

const XString& class_audio_backend::get_defualt(void)
{
	if ( audio_backend_entry_points_array->Length() == 0 ) return NullXString;
	return (*audio_backend_entry_points_array)[0]._ClassName;
}




class_audio_backend::class_audio_backend(int argc, char **argv)
{
}

class_audio_backend::~class_audio_backend()
{
}

