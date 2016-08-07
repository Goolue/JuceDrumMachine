#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "ReferenceCountedBuffer.h"
#include "FilterGui.h"

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

	explicit DrumGui(ReferenceCountedArray<ReferenceCountedBuffer>* _mainBuffers);
	~DrumGui();

	void sliderValueChanged(Slider* slider) override;
	void buttonClicked(Button* button) override;
	void resized() override;
	void run() override;

	int getTotalHight() const;
	int getTotalWidth() const;

	float getVolume() const;
	ReferenceCountedBuffer::Ptr getBuffToPlay() const;

private:
	//funcs:
	void changeState(PlayState newState);
	void openButtonClicked();
	void playButtonClicked();
	void checkForPathToOpen();
	void start();
	void stop();
	int calcY(Component* above) const;
	ReferenceCountedBuffer* createBuffToSend() const;

	//GUI elements:
	TextButton openFileButton;
	TextButton playButton;
	ScopedPointer<Label> volLbl;
	Slider volSlider;
	FilterGui filter;

	//vars:
	bool stoppedPlaying = false;
	float volume = 0.5;
	String chosenFilePath;

	//others:
	AudioFormatManager formatManager;
	PlayState state;
	ReferenceCountedArray<ReferenceCountedBuffer>* mainBuffers; //freed from MainComponent

	ReferenceCountedBuffer::Ptr buffToPlay;
	
};

