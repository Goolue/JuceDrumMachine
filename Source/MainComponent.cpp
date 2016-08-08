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
		drum1->setBounds(10, 10, drum1->getTotalWidth(), 2 * drum1->getTotalHight());
		//drum1->setMainBuffer(&buffers);
		drumArr.add(drum1);

		setSize(600, 400);

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
			if (buff != nullptr)
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
				const int buffPosition = buff->getPosition();
				if (buffPosition >= buff->getAudioSampleBuffer()->getNumSamples())
				{
					buffers.removeObject(buff);
				}
			}
		}
		//ReferenceCountedArray<DrumGui> currBuffers(buffers);
		//if (currBuffers.isEmpty()) //if the buff is null, output silence
		//{
		//	bufferToFill.clearActiveBufferRegion();
		//	return;
		//}
		//const int outputSamplesRemaining = bufferToFill.numSamples;
		//for (DrumGui* drum : currBuffers)
		//{
		//	if (drum != nullptr)
		//	{
		//		ReferenceCountedBuffer* buff = drum->process();
		//		const int position = buff->getPosition();
		//		const int buffNumOfSamples = buff->getAudioSampleBuffer()->getNumSamples() - position;
		//		const int samplesThisTime = jmin(outputSamplesRemaining, buffNumOfSamples); //min of ext buff and our buff
		//		const int numInputChannels = buff->getAudioSampleBuffer()->getNumChannels();
		//		const int numOutputChannels = bufferToFill.buffer->getNumChannels();
		//		for (int channel = 0; channel < numOutputChannels; ++channel)
		//		{
		//			float* buffToWriteTo = bufferToFill.buffer->getWritePointer(channel);
		//			const float* source = buff->getAudioSampleBuffer()->getReadPointer(channel % numInputChannels, position);
		//			for (int index = 0; index <= samplesThisTime; ++index)
		//			{
		//				buffToWriteTo[index] = source[index];
		//			}
		//		}
		//		drum->getBuffToPlay()->setPosition(position + samplesThisTime);
		//		const int buffPosition = buff->getPosition();
		//		const int buffSize = buff->getAudioSampleBuffer()->getNumSamples();
		//		if (buffPosition >= buffSize)
		//		{
		//			buffers.removeObject(drum);
		//		}
		//	}

		//}
	}

	void releaseResources() override
	{
	}

	void resized() override
	{
		/*openFileButton.setBounds(10, 10, getWidth() - 20, 20);
		playButton.setBounds(10, 40, getWidth() - 20, 20);
		volSlider.setBounds(10, 60, playButton.getWidth() / 2, 40);*/
		drum1->setBounds(10, 10, 4 * drum1->getTotalWidth(), 2 * drum1->getTotalHight());
	}

	void run() override
	{
		while (!threadShouldExit())
		{
			//checkStopped();
			wait(-1);
		}
	}


//*****************PRIVATE:*****************

private:

	//void checkStopped()
	//{
	//	DBG("checkStopped");
	//	if (stopped)
	//	{
	//		/*for (ReferenceCountedBuffer::Ptr* buff : buffers)
	//		{
	//			auto instance = *buff;
	//			if (instance->getPosition() >= instance->getAudioSampleBuffer()->getNumSamples())
	//			{
	//				buffers.removeObject()
	//			}
	//		}*/
	//		for (int i = buffers.size() - 1; i >= 0; i--)
	//		{
	//			auto obj = buffers.getObjectPointer(i);
	//			int position = obj->getPosition();
	//			int size = obj->getAudioSampleBuffer()->getNumSamples();
	//			if (position >= size)
	//			{
	//				buffers.removeObject(obj);
	//			}
	//		}
	//		stopped = false;
	//	}
	//}

	ReferenceCountedArray<ReferenceCountedBuffer> buffers;

	DrumGui* drum1 = new DrumGui(&buffers);
	ReferenceCountedArray<DrumGui> drumArr;
	bool stopped = false;
	double sampleRate;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
