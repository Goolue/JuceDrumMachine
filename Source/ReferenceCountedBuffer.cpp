//#ifndef MAINCOMPONENT_H_INCLUDED
//#define MAINCOMPONENT_H_INCLUDED

#include "ReferenceCountedBuffer.h"

ReferenceCountedBuffer::ReferenceCountedBuffer(const String& nameToUse, int numChannels, int numSamples)
	: position(0), name(nameToUse),	buffer(numChannels, numSamples)
{
}

ReferenceCountedBuffer::~ReferenceCountedBuffer()
{
}

AudioSampleBuffer* ReferenceCountedBuffer::getAudioSampleBuffer()
{
	return &buffer;
}

int ReferenceCountedBuffer::getPosition() const
{
	return position;
}

void ReferenceCountedBuffer::setPosition(int newPosition)
{
	position = newPosition;
}

void ReferenceCountedBuffer::resetPosition()
{
	position = 0;
}

String ReferenceCountedBuffer::getName() const
{
	return name;
}

void ReferenceCountedBuffer::loadToBuffer(AudioSampleBuffer* other)
{
	int numChannels = other->getNumChannels();
	int numSamples= other->getNumSamples();
	//buffer.setSize(numChannels, numSamples, false, true, false);
	for (int channelIndex = 0; channelIndex < numChannels; channelIndex++)
	{
		buffer.copyFrom(channelIndex, 0, other->getReadPointer(channelIndex), numSamples);
	}
}

//#endif


