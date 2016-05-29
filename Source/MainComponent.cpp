/*
 * MainComponent.cpp
 *
 *  Created on: 26 May 2016
 *      Author: bone
 */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent : public AudioAppComponent
{
public:
	MainContentComponent();
	~MainContentComponent();

	void prepareToPlay (int samplesPerBlock, double sampleRate);

	void releaseResources();

	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

	void resized();
private:
	const size_t numberOfBands = 5;
	Slider* sliderArray;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

MainContentComponent::MainContentComponent()
{
	sliderArray = new Slider[numberOfBands];

	for(size_t i = 0; i < numberOfBands; ++i)
	{
		sliderArray[i].setSliderStyle(Slider::LinearVertical);
		sliderArray[i].setTextBoxStyle(Slider::TextBoxAbove,true,100,20);
		addAndMakeVisible(&sliderArray[i]);
	}

	setSize (1000, 1000);
	setAudioChannels (0, 2);
}

MainContentComponent::~MainContentComponent()
{
	delete[] sliderArray;
	shutdownAudio();
}

void MainContentComponent::prepareToPlay (int samplesPerBlock, double sampleRate)
{
    String message;
    message << "Preparing to play audio...\n";
    message << " samplesPerBlockExpected = " << samplesPerBlock << "\n";
    message << " sampleRate = " << sampleRate << "\n";
	message << "This is an extra message";
    Logger::getCurrentLogger()->writeToLog (message);
}

void MainContentComponent::releaseResources()
{
	Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	//Do nothing
}

void MainContentComponent::resized()
{
	int windowWidth = getWidth();
	int windowHeight = getHeight();
	int x = windowWidth*0.1;
	int y = windowHeight/2;
	for(size_t i = 0; i < numberOfBands; ++i)
	{
		int slider_x,slider_y,width,height;

		height = windowHeight-y;
		width = (windowWidth-2*x)/numberOfBands;
		slider_x = x+i*width;
		slider_y = y;
		sliderArray[i].setBounds(slider_x,slider_y,width,height);
	}
}

Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
