/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
class MainContentComponent : public AudioAppComponent
{
public:
	//==============================================================================
	MainContentComponent()
	{
		setSize(800, 600);

 
		// specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}

	~MainContentComponent()
	{
		shutdownAudio();
	}

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	void releaseResources() override;

	//==============================================================================
	void paint(Graphics& g) override;

	void resized() override;


private:
	//==============================================================================

	// Your private member variables go here...


};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
