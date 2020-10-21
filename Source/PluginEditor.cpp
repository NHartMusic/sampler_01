/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Sampler_2020AudioProcessorEditor::Sampler_2020AudioProcessorEditor (Sampler_2020AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    mLoadButton.onClick = [&]() { processor.loadFile(); };
    addAndMakeVisible(mLoadButton);
    
        //Attack Slider
    mAttackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    mAttackSlider.setRange(0.0f, 5.0f, 0.01f);
    mAttackSlider.addListener(this);
    addAndMakeVisible(mAttackSlider);
    
    mAttackLabel.setFont(10.0f);
    mAttackLabel.setText("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mAttackLabel.setJustificationType(juce::Justification::centredTop);
    mAttackLabel.attachToComponent(&mAttackSlider, false);
    
        //Decay Slider
    mDecaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    mDecaySlider.setRange(0.0f, 5.0f, 0.01f);
    mDecaySlider.addListener(this);
    addAndMakeVisible(mDecaySlider);
    
    mDecayLabel.setFont(10.0f);
    mDecayLabel.setText("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mDecayLabel.setJustificationType(juce::Justification::centredTop);
    mDecayLabel.attachToComponent(&mDecaySlider, false);

        //Sustain Slider
    mSustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    mSustainSlider.setRange(0.0f, 1.0f, 0.01f);
    mSustainSlider.addListener(this);
    addAndMakeVisible(mSustainSlider);
    
    mSustainLabel.setFont(10.0f);
    mSustainLabel.setText("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mSustainLabel.setJustificationType(juce::Justification::centredTop);
    mSustainLabel.attachToComponent(&mSustainSlider, false);
    
        //Release Slider
    mReleaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    mReleaseSlider.setRange(0.0f, 1.0f, 0.01f);
    mReleaseSlider.addListener(this);
    addAndMakeVisible(mReleaseSlider);
    
    mReleaseLabel.setFont(10.0f);
    mReleaseLabel.setText("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::green);
    mReleaseLabel.setJustificationType(juce::Justification::centredTop);
    mReleaseLabel.attachToComponent(&mReleaseSlider, false);
    
    setSize (600, 300);
}

Sampler_2020AudioProcessorEditor::~Sampler_2020AudioProcessorEditor()
{
    
}

//==============================================================================
void Sampler_2020AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::turquoise);
    
    g.setColour(juce::Colours::white);
    
    if (mShouldBePainting)
    {
        juce::Path p;
        mAudioPoints.clear();
        
        auto waveform = processor.getWaveForm();
        auto ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);
        
        //scale audio file to window on x axis
        for (int sample = 0; sample < waveform.getNumSamples(); sample+=ratio)
        {
            mAudioPoints.push_back(buffer[sample]);
        }
        
        p.startNewSubPath(0, getHeight() / 2);
        
        //scale on y axis
        for (int sample = 0; sample < mAudioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float> (mAudioPoints[sample], -1.0f, 1.0f, 200, 0);
            
            p.lineTo(sample, point);
        }
        
        g.strokePath(p, juce::PathStrokeType (2));
        
        mShouldBePainting = false;
    }
    
//    g.setColour(juce::Colours::white);
//    g.setFont(15.0f);
//
//    if (processor.getNumSamplerSounds() > 0)
//    {
//        g.fillAll(juce::Colours::turquoise);
//        g.drawText("Audio File Loaded", getWidth() / 2 - 50 , getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
//    }
//    else
//    {
//        g.drawText("Load an Audio File", getWidth() / 2 - 50 , getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
//    }
}

void Sampler_2020AudioProcessorEditor::resized()
{
    //mLoadButton.setBounds(getWidth() / 2 - 50 , getHeight() / 2 - 50, 100, 100);
    
    const auto startX = 0.6f;
    const auto startY = 0.6f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.4f;
    

    mAttackSlider.setBoundsRelative(startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative(startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative(startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative(startX + (dialWidth * 3), startY, dialWidth, dialHeight);
    
}

bool Sampler_2020AudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto file : files)
    {
        if (file.contains (".aif") || file.contains(".wav") || file.contains(".mp3"))
        {
            return true;
        }
    }
    return false;
}
 
void Sampler_2020AudioProcessorEditor::filesDropped(const juce::StringArray &files, int x, int y)
{
    for (auto file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            mShouldBePainting = true;
            processor.loadFile (file);
        }
    }
    
    repaint();
}

void Sampler_2020AudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &mAttackSlider)
    {
        processor.getADSRParams().attack = mAttackSlider.getValue();
    } else if (slider == &mDecaySlider)
    {
        processor.getADSRParams().decay = mAttackSlider.getValue();
    } else if (slider == &mSustainSlider)
    {
        processor.getADSRParams().sustain = mAttackSlider.getValue();
    }  else if (slider == &mReleaseSlider)
    {
        processor.getADSRParams().release = mAttackSlider.getValue();
    }
    
    processor.updateADSR();
}
