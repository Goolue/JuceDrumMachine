#include "filterGui.h"

FilterGui::FilterGui() :  type(LowPass), isOn(false), freq(1000), gain(1.0), q(1.6)
{
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
    filterQSlider->setRange (0.2, 3.0, 0.01);
	filterQSlider->setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    filterQSlider->setTextBoxStyle (Slider::TextBoxAbove, true, 80, 20);
    filterQSlider->addListener (this);

    addAndMakeVisible (qLbl = new Label ("Q lable",
                                         TRANS("Filter Q")));
	configLable(qLbl);

    addAndMakeVisible (gainSlider = new Slider ("Q"));
    gainSlider->setRange (0.001, 5.0, 0.001);
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
	filter.makeInactive();

    setSize (1400, 2000);
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
    filterOnBtn->setBounds (0, 0, 60, 40);
	
	freqLbl->setBounds(filterOnBtn->getX() + 5, filterOnBtn->getY() + filterOnBtn->getHeight(), 90, 24);
	
	filterFreqSlider->setBounds(freqLbl->getX(), freqLbl->getY() + freqLbl->getHeight(),
		100, 50);
	
	qLbl->setBounds(freqLbl->getX() + freqLbl->getWidth() + 5, freqLbl->getY(), 103, 24);
    
	filterQSlider->setBounds(qLbl->getX() + 5, qLbl->getY() + qLbl->getHeight(), 
		70, 50);
    
	typeLbl->setBounds(filterFreqSlider->getX(), filterFreqSlider->getY() + 
		filterFreqSlider->getHeight(), filterFreqSlider->getWidth() - 5, 24);
	
	filterTypeCombo->setBounds(typeLbl->getX(), typeLbl->getY() + typeLbl->getHeight(),
		typeLbl->getWidth(), 20);
	
	gainLbl->setBounds(filterTypeCombo->getX() + filterTypeCombo->getWidth(), filterQSlider->getY() +
		filterQSlider->getHeight(), 103, 24);
    
	gainSlider->setBounds (gainLbl->getX() + 10, gainLbl->getY() + gainLbl->getHeight(), 
		70, 50);
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
		if (filterOnBtn->getToggleState() == true)
		{
			int id = filterTypeCombo->getSelectedId();
			DBG(String(id));
			switch (id)
			{
			case 1:
				type = LowPass;
				filterQSlider->setEnabled(false);
				gainSlider->setEnabled(false);
				break;
			case 2:
				type = HighPass;
				filterQSlider->setEnabled(false);
				gainSlider->setEnabled(false);
				break;
			case 3:
				type = LowShelf;
				enableButtonsAndSliders();
				break;
			case 4:
				type = HighShelf;
				enableButtonsAndSliders();
				break;
			case 5:
				type = Peak;
				enableButtonsAndSliders();
				break;
			default:
				DBG("Ilegale ID!");
				break;
			}
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
	filterTypeCombo->setEnabled(true);
	if (type != LowPass && type != HighPass)
	{
		filterQSlider->setEnabled(true);
		gainSlider->setEnabled(true);
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
	filterQSlider->setValue(1.6);
	gainSlider->setValue(1.0);
}

void FilterGui::calcCoef(double sampleRate)
{
	if (filterOnBtn->getToggleState() == true)
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
	else
	{
		filter.makeInactive();
	}
}

int FilterGui::getTotalWidth() const
{
	return filterFreqSlider->getWidth() + qLbl->getWidth() + 10;
}

int FilterGui::getTotalHight() const
{
	return filterOnBtn->getHeight() + freqLbl->getHeight() + filterQSlider->getHeight() + 
		qLbl->getHeight() + gainLbl->getHeight() + gainSlider->getHeight();
}