/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SampleExplorerAudioProcessor::SampleExplorerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
    variableTree = {
        
        "Variables", {},
        {
          { "Group", {{ "name", "Vars" }},
            {
              { "Parameter", {{ "id", "sampleSaveLocation" }, { "value", "/" }}},
                { "Parameter", {{ "id", "sampleRoot" }, { "value", "/" }}}
            }
          }
        }
      };
    
    exportEnabled.store(false);
}

SampleExplorerAudioProcessor::~SampleExplorerAudioProcessor()
{
    transportSource.setSource(nullptr);
    
    // Delete temp file if the user didn't save anything to it
    auto leftOverFile = juce::File(sampleSaveLocation.getChildFile("temp.wav"));
    
    if (leftOverFile.existsAsFile())
    {
        leftOverFile.deleteFile();
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout SampleExplorerAudioProcessor::createParameterLayout()
{
    std::vector <std::unique_ptr<juce::RangedAudioParameter>> params;
    
    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String SampleExplorerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SampleExplorerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SampleExplorerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SampleExplorerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SampleExplorerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SampleExplorerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SampleExplorerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SampleExplorerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SampleExplorerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SampleExplorerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SampleExplorerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    transportSource.prepareToPlay(samplesPerBlock, sampleRate);
    transportSource.setLooping(false);
}

void SampleExplorerAudioProcessor::releaseResources()
{
    transportSource.releaseResources();
    transportSource.setSource(nullptr);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SampleExplorerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void SampleExplorerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    juce::AudioSourceChannelInfo channelInfo(buffer);

    if (readerSource.get() == nullptr)
    {
        channelInfo.clearActiveBufferRegion();
        return;
    }
    
    else
    {
        transportSource.getNextAudioBlock (channelInfo);
    }
    
    auto channelBuffers = buffer.getArrayOfWritePointers();
        
    for (auto sample {0}; sample < buffer.getNumSamples(); sample++)
    {
        for (auto channel {0}; channel < buffer.getNumChannels(); channel++)
        {
            channelBuffers[channel][sample] *= 0.1;
        }
    }
    
    // Write audio while the export button is on
    if (exportEnabled.load())
    {
        // File is already loaded into the writer on the gui side
        writeBufferToFile(buffer);
    }
}

void SampleExplorerAudioProcessor::writeBufferToFile(juce::AudioBuffer<float>& buffer)
{
    if (writer != nullptr)
    {
        DBG("Writing");
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SampleExplorerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SampleExplorerAudioProcessor::createEditor()
{
    return new SampleExplorerAudioProcessorEditor (*this);
}

//==============================================================================
void SampleExplorerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    treeState.state.appendChild(variableTree, nullptr);
    juce::MemoryOutputStream stream(destData, false);
    treeState.state.writeToStream (stream);
}

void SampleExplorerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, size_t(sizeInBytes));
    variableTree = tree.getChildWithName("Variables");
    
    if (tree.isValid())
    {
        treeState.state = tree;
        sampleSaveLocation = variableTree.getProperty("sampleSaveLocation");
        sampleRoot = variableTree.getProperty("sampleRoot");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SampleExplorerAudioProcessor();
}
