#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ReferenceCountedBuffer.h"
#include "FilterGui.h"
#include "../Builds/VisualStudio2013/VolumeGui.h"

using namespace juce;

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

	int getTotalHight() const;
	int getTotalWidth() const;
	void setSampleRate(double rate);
	void setMainBuffer(ReferenceCountedArray<DrumGui>* arr);
	void click(float velocity);

	ReferenceCountedBuffer::Ptr getBuffToPlay() const;
	ReferenceCountedBuffer* process();

private:
	//funcs:
	void changeState(PlayState newState);
	void openButtonClicked();
	void playButtonClicked();
	void checkForPathToOpen();
	void start();
	void stop();
	ReferenceCountedBuffer* createBuffToSend();

	//GUI elements:
	TextButton openFileButton;
	TextButton playButton;
	VolumeGui volume;
	FilterGui filter;

	//vars:
	bool shouldPlay = false;
	String chosenFilePath;
	double sampleRate;
	float currVelovity;

	//others:
	AudioFormatManager formatManager;
	PlayState state;
	ReferenceCountedArray<ReferenceCountedBuffer>* mainBuffer;

	ReferenceCountedBuffer::Ptr buffToPlay;
	
};

