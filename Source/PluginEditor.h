/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Sampler_2020AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                          public juce::FileDragAndDropTarget,
                                          public juce::Slider::Listener
{
public:
    Sampler_2020AudioProcessorEditor (Sampler_2020AudioProcessor&);
    ~Sampler_2020AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;
    
    void sliderValueChanged (juce::Slider* slider) override;

private:
    
    juce::TextButton mLoadButton { "Load" };
    std::vector<float> mAudioPoints;
    bool mShouldBePainting { false };
    
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    Sampler_2020AudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Sampler_2020AudioProcessorEditor)
};
