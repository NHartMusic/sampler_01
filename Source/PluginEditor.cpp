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
    
    setSize (600, 300);
}

Sampler_2020AudioProcessorEditor::~Sampler_2020AudioProcessorEditor()
{
    
}

//==============================================================================
void Sampler_2020AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    
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
