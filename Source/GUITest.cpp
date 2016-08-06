/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 4.2.3

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "GUITest.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
GUITest::GUITest ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    addAndMakeVisible (btnPlay = new TextButton ("new button"));
    btnPlay->setButtonText (TRANS("Play"));
    btnPlay->addListener (this);

    addAndMakeVisible (btnLoad = new TextButton ("new button"));
    btnLoad->setButtonText (TRANS("Load File"));
    btnLoad->setConnectedEdges (Button::ConnectedOnTop);
    btnLoad->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

GUITest::~GUITest()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    btnPlay = nullptr;
    btnLoad = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GUITest::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void GUITest::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    btnPlay->setBounds (56, 112, proportionOfWidth (0.3989f), proportionOfHeight (0.1996f));
    btnLoad->setBounds (56, 248, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void GUITest::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == btnPlay)
    {
        //[UserButtonCode_btnPlay] -- add your button handler code here..
        //[/UserButtonCode_btnPlay]
    }
    else if (buttonThatWasClicked == btnLoad)
    {
        //[UserButtonCode_btnLoad] -- add your button handler code here..
        //[/UserButtonCode_btnLoad]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GUITest" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TEXTBUTTON name="new button" id="70c9d179ab7e13e9" memberName="btnPlay"
              virtualName="" explicitFocusOrder="0" pos="56 112 39.894% 19.964%"
              buttonText="Play" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="9c493612c002aef4" memberName="btnLoad"
              virtualName="" explicitFocusOrder="0" pos="56 248 150 24" buttonText="Load File"
              connectedEdges="4" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
