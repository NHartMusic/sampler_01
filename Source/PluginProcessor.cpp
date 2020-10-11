/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Sampler_2020AudioProcessor::Sampler_2020AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mFormatManager.registerBasicFormats();
    
    for ( int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice (new juce::SamplerVoice());
    }
}

Sampler_2020AudioProcessor::~Sampler_2020AudioProcessor()
{
    mFormatReader = nullptr;
}

//==============================================================================
const juce::String Sampler_2020AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Sampler_2020AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Sampler_2020AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Sampler_2020AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Sampler_2020AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Sampler_2020AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Sampler_2020AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Sampler_2020AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Sampler_2020AudioProcessor::getProgramName (int index)
{
    return {};
}

void Sampler_2020AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Sampler_2020AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mSampler.setCurrentPlaybackSampleRate(sampleRate);
}

void Sampler_2020AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Sampler_2020AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Sampler_2020AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mSampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Sampler_2020AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Sampler_2020AudioProcessor::createEditor()
{
    return new Sampler_2020AudioProcessorEditor (*this);
}

//==============================================================================
void Sampler_2020AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Sampler_2020AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void Sampler_2020AudioProcessor::loadFile()
{
    mSampler.clearSounds();
    
    juce::FileChooser chooser { "Load Up A File Bitch" };
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        mFormatReader = mFormatManager.createReaderFor(file);
    }
    
    juce::BigInteger range;
    range.setRange(0, 128, true);
    
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10));
}

void Sampler_2020AudioProcessor::loadFile(const juce::String& path)
{
    mSampler.clearSounds();
    
    auto file = juce::File (path);
    mFormatReader = mFormatManager.createReaderFor(file);
    
    auto sampleLength = static_cast<int>(mFormatReader->lengthInSamples);
    
    mWaveForm.setSize(1, sampleLength);
    mFormatReader->read (&mWaveForm, 0, sampleLength, 0, true, false);
    
  
    juce::BigInteger range;
    range.setRange(0, 128, true);
       
    mSampler.addSound(new juce::SamplerSound("Sample", *mFormatReader, range, 60, 0.1, 0.1, 10));
}

// This creates new instances of the plugin..

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Sampler_2020AudioProcessor();
}
