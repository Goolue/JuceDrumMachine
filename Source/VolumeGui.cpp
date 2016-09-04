#include "VolumeGui.h"
VolumeGui::VolumeGui() : isOn(false), volume(1.0), attack(0), decay(3)
{
	addAndMakeVisible(&volSlider);
	volSlider.setRange(0.0, 5.0, 0.01);
	volSlider.addListener(this);
	volSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	volSlider.showTextBox();
	volSlider.setSkewFactor(0.6); //TODO: figure out better volume settings

	addAndMakeVisible(&attackSlider);
	attackSlider.setRange(0.0, 3.0, 0.01);
	attackSlider.addListener(this);
	attackSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	attackSlider.showTextBox();

	addAndMakeVisible(&decaySlider);
	decaySlider.setRange(0.0, 3.0, 0.01);
	decaySlider.addListener(this);
	decaySlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
	decaySlider.showTextBox();

	addAndMakeVisible(volLbl = new Label("volume lable",
		TRANS("Volume")));
	configLable(volLbl);

	addAndMakeVisible(attackLbl = new Label("attack lable",
		TRANS("Attack")));
	configLable(attackLbl);

	addAndMakeVisible(decayLbl = new Label("decay lable",
		TRANS("Decay")));
	configLable(attackLbl);
	
	reset();

	setSize(130, 120);
}


VolumeGui::~VolumeGui()
{
	volLbl = nullptr;
	attackLbl = nullptr;
	decayLbl = nullptr;
}

void VolumeGui::resized()
{
	volLbl->setBounds(0, 0, 60, 20);
	volSlider.setBounds(volLbl->getX(), volLbl->getY() + volLbl->getHeight(), 60, 30);
	attackLbl->setBounds(volLbl->getX() + volLbl->getWidth() + 5, volLbl->getY(), 60, 20);
	attackSlider.setBounds(attackLbl->getX(), attackLbl->getY() + attackLbl->getHeight(), 65, 40);
	decayLbl->setBounds(volSlider.getX(), volSlider.getY() + volSlider.getHeight(), 60, 20);
	decaySlider.setBounds(decayLbl->getX(), decayLbl->getY() + decayLbl->getHeight(), 65, 40);
}

void VolumeGui::process(juce::AudioSampleBuffer* buff, float velocity) const
{
	int buffSize = buff->getNumSamples();
	if (attack > 0)
	{
		int attackInSamples = secToSamples(attack);
		if (attackInSamples <= buffSize)
		{
			buff->applyGainRamp(0, attackInSamples, 0, 1);
		}
		else
		{
			buff->applyGainRamp(0, buffSize, 0, calcAttack(buffSize, attackInSamples));
		}
		DBG("attack applied: " + String(attackInSamples));
	}
	if (decay > 0)
	{
		int decayInSamples = secToSamples(decay);
		if (decayInSamples < buffSize)
		{
			buff->applyGainRamp(0, decayInSamples, 1, 0); //make gain slope
			buff->applyGain(decayInSamples, buffSize - decayInSamples, 0); //flatten rest
		}
		if (decayInSamples == buffSize)
		{
			buff->applyGainRamp(0, decayInSamples, 1, 0);
		}
		else
		{
			buff->applyGainRamp(0, buffSize, 1, calcDecay(buffSize, decayInSamples));
		}
		buff->applyGain(volume * velocity);
	}
	else
	{
		buff->applyGain(0);
	}
}

void VolumeGui::sliderValueChanged(Slider* slider)
{
	if (slider == &volSlider)
	{
		volume = volSlider.getValue();
	}
	else if (slider == &attackSlider)
	{
		attack = attackSlider.getValue();
	}
	else if (slider == & decaySlider)
	{
		decay = decaySlider.getValue();
	}
	else
	{
		DBG("Unknown slider changed");
	}

}

void VolumeGui::setSampleRate(double rate)
{
	sampleRate = rate;
}

int VolumeGui::getTotalWidth() const
{
	return volSlider.getWidth() + attackSlider.getWidth() + 10;
}

//*********privates*********

int VolumeGui::getTotalHight() const
{
	return volLbl->getHeight() + volSlider.getHeight() + decayLbl->getHeight() + decaySlider.getHeight();
}

void VolumeGui::reset()
{
	attackSlider.setValue(0);
	volSlider.setValue(1);
	decaySlider.setValue(5);

}

void VolumeGui::configLable(Label* lbl) const
{
	if (lbl != nullptr)
	{
		lbl->setFont(Font(15.00f, Font::plain));
		lbl->setJustificationType(Justification::centred);
		lbl->setEditable(false, false, false);
		lbl->setColour(TextEditor::textColourId, Colours::black);
		lbl->setColour(TextEditor::backgroundColourId, Colour(0x00000000));
	}
}

//if sampleRate was not set yet, returns -1
int VolumeGui::secToSamples(float secs) const
{
	if (sampleRate > 0)
	{
		return static_cast<int>(secs * sampleRate);
	}
	return -1;
}

float VolumeGui::calcAttack(int buffSize, int attackInSamples) const
{
	float angle = 1 / attackInSamples; // 1 = AIS * angle
	return angle * buffSize;
}

float VolumeGui::calcDecay(int buffSize, int decayInSamples) const
{
	float angle = 1 / decayInSamples; //that is the complimentary angle
	return 1 - angle * buffSize;
}
