#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "DrumGui.h"

#define FIRST_C 36

class MidiHandler : public Thread,
					public Component,
					public ComboBox::Listener,
					public MidiInputCallback
{
public:
	MidiHandler();
	~MidiHandler();
	void handleIncomingMidiMessage(juce::MidiInput* source, const MidiMessage& message) override;
	void run() override;
	void matchMidiNoteToDrum(ReferenceCountedArray<DrumGui> drums);
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void resized() override;

private:
	//funcs:
	void setMidiInput(int index);
	void checkMidiBuffer();

	//GUI elements:
	ScopedPointer<ComboBox> comboBox;
	ScopedPointer<Label> comboBoxLbl;

	//vars:
	AudioDeviceManager deviceManager;
	MidiBuffer midiBuffWrite;
	MidiBuffer midiBuffRead;
	HashMap<int, DrumGui*> midiToDrumMap;
	int lastInputIndex;
};

