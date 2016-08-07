#include "DrumGui.h"

DrumGui::DrumGui(ReferenceCountedArray<ReferenceCountedBuffer>* _mainBuffers)
	: Thread("DrumGui Thread"), volume(0.5), state(Stopped), mainBuffers(_mainBuffers),
	buffToPlay(nullptr)
{
	addAndMakeVisible(&openFileButton);
	openFileButton.setButtonText("Open file");
	openFileButton.addListener(this);

	addAndMakeVisible(&playButton);
	playButton.setButtonText("Play");
	playButton.addListener(this);
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(false);

	addAndMakeVisible(volLbl = new Label("volume lable",
		TRANS("Volume")));
	volLbl->setFont(Font(15.00f, Font::plain));
	volLbl->setJustificationType(Justification::centred);
	volLbl->setEditable(false, false, false);
	volLbl->setColour(TextEditor::textColourId, Colours::black);
	volLbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

	addAndMakeVisible(&volSlider);
	volSlider.setRange(0.0, 1.0, 0.01);
	volSlider.addListener(this);
	volSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	volSlider.showTextBox();
	volSlider.setValue(0.5);

	addAndMakeVisible(&filter);

	setSize(filter.getWidth(), 1000);

	formatManager.registerBasicFormats();
	startThread();
}


DrumGui::~DrumGui()
{
	volLbl = nullptr;
	stopThread(4000);
	mainBuffers = nullptr;
}

void DrumGui::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
	{
		volume = volSlider.getValue();
	}
}

void DrumGui::buttonClicked(Button* button)
{
	if (button == &openFileButton)
	{
		openButtonClicked();
	}
	else if (button == &playButton)
	{
		playButtonClicked();
	}
}

void DrumGui::resized()
{
	playButton.setBounds(0, 0, 100, 100);
	openFileButton.setBounds(playButton.getX() + playButton.getWidth() + 5, playButton.getY(), 50, 20);
	volLbl->setBounds(openFileButton.getX(), openFileButton.getY() + openFileButton.getHeight() + 5, 60, 20);
	volSlider.setBounds(volLbl->getX() + 5, volLbl->getY() + volLbl->getHeight() + 5, 60, 40);
	filter.setBounds(0, calcY (&playButton), 1200, 800);

}

void DrumGui::run()
{
	while (!threadShouldExit())
	{
		checkForPathToOpen();
		wait(-1); //wait till notify
	}
}


int DrumGui::getTotalHight() const
{
	return openFileButton.getHeight() + playButton.getHeight() +
		volSlider.getHeight();
}

int DrumGui::getTotalWidth() const
{
	return jmax(openFileButton.getWidth(), playButton.getWidth() + volSlider.getHeight());
}

float DrumGui::getVolume() const
{
	return volume;
}

ReferenceCountedBuffer::Ptr DrumGui::getBuffToPlay() const
{
	return buffToPlay;
}

//***********privates***********:

int DrumGui::calcY(Component* above) const
{
	return above->getY() + above->getHeight() + 5;
}

void DrumGui::changeState(PlayState newState)
{
	if (state != newState)
	{
		state = newState;

		switch (state)
		{
		case Stopping:
			buffToPlay = nullptr;
			stoppedPlaying = false;
			break;
		case Stopped:
			playButton.setEnabled(true);
			break;
		case Starting:
			break;
		case Playing:
			ReferenceCountedBuffer* toSend = createBuffToSend();
			mainBuffers->add(toSend);
			break;
		}
	}
}

void DrumGui::openButtonClicked()
{
	FileChooser chooser("Select a Wave file to play...",
		File::nonexistent,
		"*.wav");

	if (chooser.browseForFileToOpen())
	{
		const File file(chooser.getResult());
		String path(file.getFullPathName());
		swapVariables(chosenFilePath, path);
		notify(); //notify background thread to open a reader
	}
}

void DrumGui::playButtonClicked()
{
	start();
}

void DrumGui::checkForPathToOpen()
{
	String pathToOpen;
	swapVariables(pathToOpen, chosenFilePath);

	if (pathToOpen.isNotEmpty())
	{
		const File file(pathToOpen);
		ScopedPointer<AudioFormatReader> reader(formatManager.createReaderFor(file));

		if (reader != nullptr)
		{
			ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
				reader->numChannels,
				reader->lengthInSamples);

			reader->read(newBuffer->getAudioSampleBuffer(), 0, reader->lengthInSamples, 0, true, true);
			buffToPlay = nullptr; //clean it
			buffToPlay = newBuffer;

			const MessageManagerLock mmLock;
			playButton.setEnabled(true);
		}
		else
		{
			DBG("reader was null");
		}
	}
}

void DrumGui::start()
{
	changeState(Starting);
	changeState(Playing);
}

void DrumGui::stop()
{
	changeState(Stopping);
	changeState(Stopped);
}

ReferenceCountedBuffer* DrumGui::createBuffToSend() const
{
	auto otherBuff = buffToPlay->getAudioSampleBuffer();
	int numChannels = otherBuff->getNumChannels();
	int numSamples = otherBuff->getNumSamples();
	auto toReturn = new ReferenceCountedBuffer("buffToSend", numChannels, numSamples);
	toReturn->loadToBuffer(otherBuff);
	toReturn->getAudioSampleBuffer()->applyGain(volume);
	return toReturn;
}
