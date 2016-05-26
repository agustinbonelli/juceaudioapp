/*
 * JuceHeader.h
 *
 *  Created on: 26 May 2016
 *      Author: bone
 */

#ifndef JUCELIBRARYCODE_JUCEHEADER_H_
#define JUCELIBRARYCODE_JUCEHEADER_H_

#include "AppConfig.h"
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_cryptography/juce_cryptography.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include <juce_opengl/juce_opengl.h>
#include <juce_video/juce_video.h>

#if ! DONT_SET_USING_JUCE_NAMESPACE
 // If your code uses a lot of JUCE classes, then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_JUCE_NAMESPACE.
 using namespace juce;
#endif

#if ! JUCE_DONT_DECLARE_PROJECTINFO
namespace ProjectInfo
{
    const char* const  projectName    = "BasicAudioApp";
    const char* const  versionString  = "1.0.0";
    const int          versionNumber  = 0x10000;
}
#endif

#endif /* JUCELIBRARYCODE_JUCEHEADER_H_ */
