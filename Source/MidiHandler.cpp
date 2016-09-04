#include "MidiHandler.h"


MidiHandler::MidiHandler() : Thread("Background Thread"), 
							lastInputIndex(0)
{
	addAndMakeVisible(comboBoxLbl = new Label("comboBox lable",
		TRANS("MIDI input")));
	comboBoxLbl->setFont(Font(15.00f, Font::plain));
	comboBoxLbl->setJustificationType(Justification::centred);
	comboBoxLbl->setEditable(false, false, false);
	comboBoxLbl->setColour(TextEditor::textColourId, Colours::black);
	comboBoxLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(comboBox = new ComboBox);
	comboBox->setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
	const StringArray midiInputs(MidiInput::getDevices());
	comboBox->addItemList(midiInputs, 1);
	comboBox->addListener(this);

	// find the first enabled device and use that by default
	for (int i = 0; i < midiInputs.size(); ++i)
	{
		if (deviceManager.isMidiInputEnabled(midiInputs[i]))
		{
			setMidiInput(i);
			break;
		}
	}

	startThread();

	setSize(150, 200);
}


MidiHandler::~MidiHandler()
{
	stopThread(1000);
	comboBox = nullptr;
	comboBoxLbl = nullptr;
}

void MidiHandler::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	String sourceName = source->getName();
	String selectedDeviceName = comboBox->getItemText(lastInputIndex);
	if (lastInputIndex > 0 && 
		sourceName.equalsIgnoreCase(selectedDeviceName) && message.isNoteOn(false))
	{
		midiBuffWrite.addEvent(message, 0); //place the new event ar the end of the buff
		notify();
	}
}

void MidiHandler::run()
{
	while (!threadShouldExit())
	{
		checkMidiBuffer();
		wait(-1);
	}
}

void MidiHandler::matchMidiNoteToDrum(ReferenceCountedArray<DrumGui> drums)
{
	int currNum = FIRST_C;
	for (auto drum : drums)
	{
		midiToDrumMap.set(currNum, drum);
		currNum++;
	}
}

void MidiHandler::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == comboBox)
	{
		setMidiInput(comboBox->getSelectedItemIndex());
	}
}

void MidiHandler::resized()
{
	comboBoxLbl->setBounds(10, 0, 150, 20);
	comboBox->setBounds(10, comboBoxLbl->getY() + comboBoxLbl->getHeight() + 5, 150, 20);
}

//**********privates:**********
void MidiHandler::setMidiInput(int index)
{
	const StringArray list(MidiInput::getDevices());

	deviceManager.removeMidiInputCallback(list[lastInputIndex], this);

	const String newInput(list[index]);

	if (!deviceManager.isMidiInputEnabled(newInput))
	{
		deviceManager.setMidiInputEnabled(newInput, true);
	}

	deviceManager.addMidiInputCallback(newInput, this);
	comboBox->setSelectedId(index + 1, dontSendNotification);

	lastInputIndex = index;
}

void MidiHandler::checkMidiBuffer()
{
	if (!midiBuffWrite.isEmpty())
	{
		midiBuffWrite.swapWith(midiBuffRead);
		MidiMessage messagePtr;
		int positionPtr = 0;
		MidiBuffer::Iterator it(midiBuffRead);
		while (it.getNextEvent(messagePtr, positionPtr))
		{
			int noteNum = messagePtr.getNoteNumber();
			DrumGui* drum = midiToDrumMap.operator[](noteNum);
			if (drum != nullptr)
			{
				drum->click(messagePtr.getFloatVelocity());
			}
		}
		midiBuffRead.clear();
	}
}
