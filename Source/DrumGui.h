#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ReferenceCountedBuffer.h"
#include "FilterGui.h"
#include "../Builds/VisualStudio2013/VolumeGui.h"

using namespace juce;

//This class represent a single drum instance in the system. Each drum works seperatly.
class DrumGui : public Component,
				public Button::Listener,
				public Slider::Listener,
				public juce::ReferenceCountedObject,
				public Thread
{
public:
	enum PlayState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	explicit DrumGui(ReferenceCountedArray<ReferenceCountedBuffer>* _arr);
	~DrumGui();

	void sliderValueChanged(Slider* slider) override;
	void buttonClicked(Button* button) override;
	void resized() override;
	void run() override;

	//use these two functions to get the correct hight / width
	int getTotalHight() const;
	int getTotalWidth() const;

	void setSampleRate(double rate);
	void setMainBuffer(ReferenceCountedArray<DrumGui>* arr);
	void click(float velocity); //called by the midiHandler to click the playBtn

private:
	//funcs:
	void changeState(PlayState newState);
	void openButtonClicked();
	void playButtonClicked();
	void checkForPathToOpen(); //called by background thread to check if a file has been opened
							  // and if so loads it into buffToPlay
	void start();
	void stop();
	ReferenceCountedBuffer* createBuffToSend(); //creates a buffer with the sample
												//and aplies the FX to it
	AudioSampleBuffer* adjustSampleRate(AudioSampleBuffer* buffer, float fileSampleRate);

	//GUI elements:
	TextButton openFileButton;
	TextButton playButton;
	VolumeGui volume;
	FilterGui filter;

	//vars:
	String chosenFilePath;
	double sampleRate;
	float currVelovity;

	//others:
	AudioFormatManager formatManager;
	PlayState state;
	ReferenceCountedArray<ReferenceCountedBuffer>* mainBuffer;
	LagrangeInterpolator interpolator;
	ReferenceCountedBuffer::Ptr buffToPlay;
	
};

