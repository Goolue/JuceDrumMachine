#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"
class ReferenceCountedBuffer : public juce::ReferenceCountedObject
{
public:

	typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;

	ReferenceCountedBuffer(const String& nameToUse,	int numChannels, int numSamples);
	~ReferenceCountedBuffer();
	juce::AudioSampleBuffer* getAudioSampleBuffer();
	int getPosition() const;
	void setPosition(int newPosition);
	void resetPosition();
	String getName() const;
	void loadToBuffer(juce::AudioSampleBuffer* other);

private:
	int position;
	String name;
	juce::AudioSampleBuffer buffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
};
	

