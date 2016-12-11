/*
 * dummy output driver. This file is part of Shairport.
 * Copyright (c) James Laird 2013
 * All rights reserved.
 *
 * Modifications for audio synchronisation
 * and related work, copyright (c) Mike Brady 2014
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "common.h"
#include "audio.h"

#include "class_audio_backend_dummy.h"

// Registering
class_audio_backend *class_audio_dummy_new_instance(int argc, char **argv) { return new class_audio_backend_dummy(argc, argv); }
int class_audio_dummy_global = register_audio_backend(new class_audio_backend_entry_points("class_audio_dummy", class_audio_dummy_new_instance, NULL));



class_audio_backend_dummy::class_audio_backend_dummy(int argc, char **argv)
{
}

class_audio_backend_dummy::~class_audio_backend_dummy()
{
}

void class_audio_backend_dummy::start(int sample_rate) {
  debug(1, "dummy audio output started at Fs=%d Hz\n", sample_rate);
}

void class_audio_backend_dummy::play(short buf[], int samples)
{
}

void class_audio_backend_dummy::stop(void)
{
	debug(1, "dummy audio stopped\n");
}

