/*
 * MainComponent.cpp
 *
 *  Created on: 26 May 2016
 *      Author: bone
 */

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent : 	public AudioAppComponent,
								public ChangeListener,
								public Button::Listener
{
public:
	MainContentComponent();
	~MainContentComponent();

	void prepareToPlay (int samplesPerBlock, double sampleRate);

	void releaseResources();

	void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill);

	void resized();

	void changeListenerCallback (ChangeBroadcaster* source);

	void buttonClicked (Button* button);
private:

	typedef void (MainContentComponent::*ActionFunction)();

	enum TransportState
	{
		Stopped = 0,
		Starting,
		Playing,
		Stopping,
		MaxTransportState,
	};


	void stoppedAction()
	{
		stopButton.setEnabled (false);
		playButton.setEnabled (true);
		transportSource.setPosition (0.0);
	}

	void startingAction()
	{
		playButton.setEnabled (false);
		transportSource.start();
	}

	void playingAction()
	{
		stopButton.setEnabled (true);
	}

	void stoppingAction()
	{
		transportSource.stop();
	}

	ActionFunction transitionAction[MaxTransportState] =
	{
			&MainContentComponent::stoppedAction,
			&MainContentComponent::startingAction,
			&MainContentComponent::playingAction,
			&MainContentComponent::stoppingAction
	};

	void changeState (TransportState newState)
	{
		if (state != newState && newState < MaxTransportState)
		{
			state = newState;

			(this->*transitionAction[state])();
		}
	}

	void openButtonClicked()
	{
		FileChooser chooser ("Select a Wave file to play...",
							 File::nonexistent,
							 "*.wav");

		if (chooser.browseForFileToOpen())
		{
			File file (chooser.getResult());
			AudioFormatReader* reader = formatManager.createReaderFor (file);

			if (reader != nullptr)
			{
				ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true); // [11]
				transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);                         // [12]
				playButton.setEnabled (true);                                                                  // [13]
				readerSource = newSource.release();                                                            // [14]
			}
		}
	}

	void playButtonClicked()
	{
		changeState (Starting);
	}

	void stopButtonClicked()
	{
		changeState (Stopping);
	}

	static const size_t numberOfBands = 5;

	Slider* sliderArray;

    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;

    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

MainContentComponent::MainContentComponent() : state(Stopped)
{
	sliderArray = new Slider[numberOfBands];

	for(size_t i = 0; i < numberOfBands; ++i)
	{
		sliderArray[i].setSliderStyle(Slider::LinearVertical);
		sliderArray[i].setTextBoxStyle(Slider::TextBoxAbove,true,100,20);
		addAndMakeVisible(&sliderArray[i]);
	}

    addAndMakeVisible (&openButton);
    openButton.setButtonText ("Open...");
    openButton.addListener (this);

    addAndMakeVisible (&playButton);
    playButton.setButtonText ("Play");
    playButton.addListener (this);
    playButton.setColour (TextButton::buttonColourId, Colours::green);
    playButton.setEnabled (false);

    addAndMakeVisible (&stopButton);
    stopButton.setButtonText ("Stop");
    stopButton.addListener (this);
    stopButton.setColour (TextButton::buttonColourId, Colours::red);
    stopButton.setEnabled (false);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener (this);

	setSize (1000, 1000);
	setAudioChannels (2, 2);
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
    message << " samplesPerBlock = " << samplesPerBlock << "\n";
    message << " sampleRate = " << sampleRate << "\n";
	message << "This is an extra message";
    Logger::getCurrentLogger()->writeToLog (message);

    transportSource.prepareToPlay(samplesPerBlock,sampleRate);
}

void MainContentComponent::releaseResources()
{
	Logger::getCurrentLogger()->writeToLog ("Releasing audio resources");
	transportSource.releaseResources();
}

void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState (Playing);
		else
			changeState (Stopped);
	}
}

void MainContentComponent::buttonClicked (Button* button)
{
    if (button == &openButton)  openButtonClicked();
    if (button == &playButton)  playButtonClicked();
    if (button == &stopButton)  stopButtonClicked();
}

void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
	if (readerSource == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	transportSource.getNextAudioBlock (bufferToFill);
}

void MainContentComponent::resized()
{
	int windowWidth = getWidth();
	int windowHeight = getHeight();
	int x = windowWidth*0.05;
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

	openButton.setBounds (10, 10, getWidth() - 20, 20);
	playButton.setBounds (10, 40, getWidth() - 20, 20);
	stopButton.setBounds (10, 70, getWidth() - 20, 20);

}

Component* createMainContentComponent()     { return new MainContentComponent(); }

#endif  // MAINCOMPONENT_H_INCLUDED
