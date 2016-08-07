#ifndef __JUCE_HEADER_C326BB340869AD7A__
#define __JUCE_HEADER_C326BB340869AD7A__

#include "juce_core/juce_core.h"
#include "../JuceLibraryCode/JuceHeader.h"

class FilterGui  : public Component,
                   public SliderListener,
                   public ButtonListener,
				   public ComboBox::Listener
{
public:
	enum filterType
	{
		LowPass,
		HighPass,
		LowShelf,
		HighShelf,
		Peak
	};

    //==============================================================================
    FilterGui ();
    ~FilterGui();

    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
	void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
	void process(float* buffer, int numOfSamples);

private:

    //==============================================================================
	
	//funcs:
	void enableButtonsAndSliders();
	void disableButtonsAndSliders();
	void configLable(Label* lbl);
	void reset();
	void calcCoef(double sampleRate);

	//GUI elements:
    ScopedPointer<Slider> filterFreqSlider;
    ScopedPointer<ToggleButton> filterOnBtn;
    ScopedPointer<Label> freqLbl;
    ScopedPointer<Slider> filterQSlider;
    ScopedPointer<Label> qLbl;
    ScopedPointer<Slider> gainSlider;
    ScopedPointer<Label> gainLbl;
	ScopedPointer<Label> typeLbl;
	ScopedPointer<ComboBox> filterTypeCombo;
	
	//vars:
	IIRCoefficients filterCoef;
	filterType type;
	bool isOn;
	float freq;
	float gain;
	float q;
	IIRFilter filter;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterGui)
};


#endif   // __JUCE_HEADER_C326BB340869AD7A__
