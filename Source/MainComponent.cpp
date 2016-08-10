#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "DrumGui.h"
#include <vld.h> 

class MainContentComponent : public AudioAppComponent, public Thread
{
public:
	MainContentComponent()
		: Thread("Background Thread")
	{
		addAndMakeVisible(drum1); 
		drumArr.add(drum1);

		addAndMakeVisible(drum2);
		drumArr.add(drum2);

		addAndMakeVisible(drum3);
		drumArr.add(drum3);

		addAndMakeVisible(drum4);
		drumArr.add(drum4);

		setSize(500, 620);

		setAudioChannels(0, 2); //0 in, 2 out

		startThread();
	}

	~MainContentComponent()
	{
		DBG("shutting down");
		shutdownAudio();
		stopThread(4000);
		for (auto drum : drumArr)
		{
			drum1 = nullptr;
		}
	}


	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		this->sampleRate = sampleRate;
		for (DrumGui* drum : drumArr)
		{
			drum->setSampleRate(sampleRate);
		}
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		ReferenceCountedArray<ReferenceCountedBuffer> currBuffers(buffers);
		if (currBuffers.isEmpty()) //if the buff is null, output silence
		{
			bufferToFill.clearActiveBufferRegion();
			return;
		}
		const int outputSamplesRemaining = bufferToFill.numSamples;
		for (ReferenceCountedBuffer* buff : currBuffers)
		{
			if (buff != nullptr && !checkBuffIsAtEndPosition(buff))
			{
				const int position = buff->getPosition();
				const int buffNumOfSamples = buff->getAudioSampleBuffer()->getNumSamples() - position;
				const int samplesThisTime = jmin(outputSamplesRemaining, buffNumOfSamples); //min of ext buff and our buff
				const int numInputChannels = buff->getAudioSampleBuffer()->getNumChannels();
				const int numOutputChannels = bufferToFill.buffer->getNumChannels();
				for (int channel = 0; channel < numOutputChannels; ++channel)
				{
					float* buffToWriteTo = bufferToFill.buffer->getWritePointer(channel);
					const float* source = buff->getAudioSampleBuffer()->getReadPointer(channel % numInputChannels, position);
					for (int index = 0; index <= samplesThisTime; ++index)
					{
						buffToWriteTo[index] = source[index];
					}
				}
				buff->setPosition(position + samplesThisTime);
				stopped = true;
				notify();
			}
		}
	}

	void releaseResources() override
	{
	}

	void resized() override
	{
		drum1->setBounds(0, 0, drum1->getTotalWidth(), drum1->getTotalHight());
		drum2->setBounds(drum1->getX() + drum1->getTotalWidth() + 10, 0,
			drum2->getTotalWidth(), drum2->getTotalHight());
		drum3->setBounds(drum1->getX(), drum1->getY() + drum1->getTotalHight(),
			drum3->getTotalWidth(), drum3->getTotalHight());
		drum4->setBounds(drum2->getX(), drum3->getY(), drum4->getTotalWidth(), drum4->getTotalHight());
	}

	void run() override
	{
		while (!threadShouldExit())
		{
			checkStopped();
			wait(-1);
		}
	}

//*****************PRIVATE:*****************

private:

	bool checkBuffIsAtEndPosition(ReferenceCountedBuffer* buff) const
	{
		int position = buff->getPosition();
		int size = buff->getAudioSampleBuffer()->getNumSamples();
		if (position >= size)
		{
			return position;
		}
		return false;
	}

	void checkStopped()
	{
		if (stopped)
		{
			for (int i = buffers.size() - 1; i >= 0; i--)
			{
				auto obj = buffers.getObjectPointer(i);
				if (checkBuffIsAtEndPosition(obj))
				{
					buffers.removeObject(obj);
				}
			}
			stopped = false;
		}
	}

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;

	DrumGui* drum1 = new DrumGui(&buffers);
	DrumGui* drum2 = new DrumGui(&buffers);
	DrumGui* drum3 = new DrumGui(&buffers);
	DrumGui* drum4 = new DrumGui(&buffers);
	ReferenceCountedArray<DrumGui> drumArr;
	bool stopped = false;
	double sampleRate;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
