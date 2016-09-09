#include "../Builds/VisualStudio2013/PitchShifter.h"

class VolumeGui : public Component,  public Slider::Listener
{
public:
	VolumeGui();
	~VolumeGui();
	void resized() override;
	void process(juce::AudioSampleBuffer* buff, float velocity) const;
	void sliderValueChanged(Slider* slider) override;
	void setSampleRate(double rate);
	int getTotalWidth() const;
	int getTotalHight() const;
	void initPitchShifter(int sampleRate, int buffSize) const;
	void setMaxAttackAndDecay(double value);

private:
	//funcs:
	void reset();
	void configLable(Label* lbl) const;
	int secToSamples(float secs) const;
	float calcAttack(int buffSize, int attackInSamples) const;
	float calcDecay(int buffSize, int decayInSamples) const;
	void setMaxDecay(double value);
	void setMaxAttack(double value);

	//GUI elements:
	ScopedPointer<Label> volLbl;
	Slider volSlider;
	ScopedPointer<Label> attackLbl;
	Slider attackSlider;
	ScopedPointer<Label> decayLbl;
	Slider decaySlider;
	ScopedPointer<PitchShifter> pitchShifter;

	//vars:
	bool isOn;
	float volume;
	float attack;
	float decay;
	double sampleRate = -1;
};

