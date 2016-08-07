#include "filterGui.h"

FilterGui::FilterGui() :  type(LowPass), isOn(false), freq(1000), gain(0.0), q(0.0)
{
	filter.makeInactive();

	addAndMakeVisible (filterOnBtn = new ToggleButton ("On off btn"));
    filterOnBtn->setButtonText (TRANS("Filter"));
    filterOnBtn->addListener (this);

    addAndMakeVisible (filterFreqSlider = new Slider ("Frequncy"));
    filterFreqSlider->setRange (50, 20000, 1);
	filterFreqSlider->setSkewFactor(0.5);
    filterFreqSlider->setSliderStyle (Slider::LinearHorizontal);
    filterFreqSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    filterFreqSlider->addListener (this);

    addAndMakeVisible (freqLbl = new Label ("frequency lable",
                                            TRANS("Filter Frequency")));
	configLable(freqLbl);

    addAndMakeVisible (filterQSlider = new Slider ("Q"));
    filterQSlider->setRange (0, 20, 0.1);
	filterQSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterQSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    filterQSlider->addListener (this);

    addAndMakeVisible (qLbl = new Label ("Q lable",
                                         TRANS("Filter Q")));
	configLable(qLbl);

    addAndMakeVisible (gainSlider = new Slider ("Q"));
    gainSlider->setRange (-10, 10, 0.1);
    gainSlider->setSliderStyle (Slider::RotaryHorizontalVerticalDrag);
    gainSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    gainSlider->addListener (this);

	addAndMakeVisible(typeLbl = new Label("type lable",
		TRANS("Filter Type")));
	configLable(typeLbl);

	addAndMakeVisible(filterTypeCombo = new ComboBox("Q"));
	filterTypeCombo->addItemList({ "Low Pass", "High Pass", "Low Shelf", "High Shelf", "Peak" }, 1);
	filterTypeCombo->addListener(this);
	filterTypeCombo->setSelectedId(1, false);

    addAndMakeVisible (gainLbl = new Label ("gain lable",
                                            TRANS("Filter Gain")));
	configLable(gainLbl);

	disableButtonsAndSliders();
	reset();

    setSize (600, 400);
}

FilterGui::~FilterGui()
{
    filterFreqSlider = nullptr;
    filterOnBtn = nullptr;
    freqLbl = nullptr;
    filterQSlider = nullptr;
    qLbl = nullptr;
    gainSlider = nullptr;
    gainLbl = nullptr;
}

//==============================================================================

void FilterGui::process(float* buffer, int numOfSamples)
{
	filter.processSamples(buffer, numOfSamples);
}

void FilterGui::resized()
{
    filterFreqSlider->setBounds (8, 56, proportionOfWidth (0.1075f), proportionOfHeight (0.0594f));
	typeLbl->setBounds(filterFreqSlider->getX(), filterFreqSlider->getY() + 
		filterFreqSlider->getHeight() + 5, filterFreqSlider->getWidth(), 24);
	filterTypeCombo->setBounds(typeLbl->getX(), typeLbl->getY() +
		typeLbl->getHeight() + 5, typeLbl->getWidth(), 20);
    filterOnBtn->setBounds (8, 8, proportionOfWidth (0.0573f), proportionOfHeight (0.0339f));
    freqLbl->setBounds (28, 32, 103, 24);
    filterQSlider->setBounds (140, 56, proportionOfWidth (0.0688f), proportionOfHeight (0.0594f));
    qLbl->setBounds (140, 32, 103, 24);
	gainLbl->setBounds(filterTypeCombo->getX() + filterTypeCombo->getWidth() + 5, filterQSlider->getY() +
		filterQSlider->getHeight() + 5, 103, 24);
    gainSlider->setBounds (gainLbl->getX(), gainLbl->getY() + 30, proportionOfWidth (0.0688f), proportionOfHeight (0.0594f));
}

void FilterGui::sliderValueChanged (Slider* sliderThatWasMoved)
{
	
	if (sliderThatWasMoved != nullptr)
	{
		float value = sliderThatWasMoved->getValue();
		if (sliderThatWasMoved == filterFreqSlider)
		{
			freq = value;
		}
		else if (sliderThatWasMoved == filterQSlider)
		{
			q = value;
		}
		else if (sliderThatWasMoved == gainSlider)
		{
			gain = value;
		}
	}

}

void FilterGui::comboBoxChanged(ComboBox *comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged == filterTypeCombo)
	{
		int id = filterTypeCombo->getSelectedId();
		switch (id)
		{
		case 0:
			type = LowPass;
			filterQSlider->setEnabled(false);
			gainSlider->setEnabled(false);
			filterTypeCombo->setEnabled(false);
			break;
		case 1:
			type = HighPass;
			filterQSlider->setEnabled(false);
			gainSlider->setEnabled(false);
			filterTypeCombo->setEnabled(false);
			break;
		case 2:
			type = LowShelf;
			break;
		case 3:
			type = HighShelf;
			break;
		case 4:
			type = Peak;
			break;
		default:
			DBG("Ilegale ID!");
			break;
		}
	}
}

void FilterGui::buttonClicked (Button* buttonThatWasClicked)
{
	DBG("click");
    if (buttonThatWasClicked == filterOnBtn)
    {
		if (filterOnBtn->getToggleState() == false)
		{
			disableButtonsAndSliders();
			filter.makeInactive();
		}
		else
		{
			enableButtonsAndSliders();
		}
    }
}

void FilterGui::enableButtonsAndSliders() 
{
	filterFreqSlider->setEnabled(true);
	if (type != LowPass && type != HighPass)
	{
		filterQSlider->setEnabled(true);
		gainSlider->setEnabled(true);
		filterTypeCombo->setEnabled(true);
	}
	
}

void FilterGui::disableButtonsAndSliders()
{
	filterFreqSlider->setEnabled(false);
	filterQSlider->setEnabled(false);
	gainSlider->setEnabled(false);
	filterTypeCombo->setEnabled(false);
}

void FilterGui::configLable(Label* lbl)
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

void FilterGui::reset()
{
	filterFreqSlider->setValue(1000);
	filterQSlider->setValue(0.0);
	gainSlider->setValue(0.0);
}

void FilterGui::calcCoef(double sampleRate)
{
	switch (type)
	{
	case LowPass:
		filterCoef = filterCoef.makeLowPass(sampleRate, freq);
		break;
	case HighPass:
		filterCoef = filterCoef.makeHighPass(sampleRate, freq);
		break;
	case LowShelf:
		filterCoef = filterCoef.makeLowShelf(sampleRate, freq, q, gain);
		break;
	case HighShelf:
		filterCoef = filterCoef.makeHighShelf(sampleRate, freq, q, gain);
		break;
	case Peak:
		filterCoef = filterCoef.makePeakFilter(sampleRate, freq, q, gain);
		break;
	default:
		DBG("Unknown filter type!");
		break;
	}
	filter.setCoefficients(filterCoef);
}