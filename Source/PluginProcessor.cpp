#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FdnReverberationNewAudioProcessor::FdnReverberationNewAudioProcessor(Reverberator::FdnDimension dim, std::vector<int>&& pow) :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    dimension(dim),
    powers(pow)
{
    channelsNum = getTotalNumInputChannels();
}

FdnReverberationNewAudioProcessor::~FdnReverberationNewAudioProcessor()
{
}

//==============================================================================
const String FdnReverberationNewAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FdnReverberationNewAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FdnReverberationNewAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FdnReverberationNewAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FdnReverberationNewAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FdnReverberationNewAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FdnReverberationNewAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FdnReverberationNewAudioProcessor::setCurrentProgram (int index)
{
}

const String FdnReverberationNewAudioProcessor::getProgramName (int index)
{
    return {};
}

void FdnReverberationNewAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FdnReverberationNewAudioProcessor::setDimension (Reverberator::FdnDimension dim)
{
    suspendProcessing (true);
    state = ProcessingState::pending;
    for (auto i = 0; i < channelsNum; ++i)
        reverberators[i].SetDimension(dim);
    dimension = dim;
    checkProcessingState();
    suspendProcessing (false);
}

void FdnReverberationNewAudioProcessor::setDelayPowers (const std::vector<int>& pow)
{
    suspendProcessing (true);
    state = ProcessingState::pending;
    for (auto i = 0; i < channelsNum; ++i)
        reverberators[i].GenerateDelayValues(pow);
    powers = pow;
    checkProcessingState();
    suspendProcessing (false);
}

void FdnReverberationNewAudioProcessor::setProcessingFlag (ProcessingFlag flag)
{
    this->flag = flag;
    state = ProcessingState::pending;
}

void FdnReverberationNewAudioProcessor::checkProcessingState ()
{
    // the processing is pended in case of the delay lines (assigned by powers) quantity is not corresponded to the dimension
    // in that case the program is waiting for the correct setting of all the parameters
    
    if (flag == ProcessingFlag::allowed)
        state = (powers.size() == (int)dimension) ? ProcessingState::running : ProcessingState::pending;
}

void FdnReverberationNewAudioProcessor::setDryWet (float drywet)
{
    this->drywet = drywet;
}

const Reverberator::FdnDimension FdnReverberationNewAudioProcessor::getDimension ()
{
    return dimension;
}

const std::vector<int>& FdnReverberationNewAudioProcessor::getDelayPowers ()
{
    return powers;
}

//==============================================================================
void FdnReverberationNewAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    blockLength = samplesPerBlock;
    channelsNum = getTotalNumInputChannels();
    for (auto i = 0; i < channelsNum; ++i)
        reverberators.emplace_back(Reverberator(dimension, powers));
    checkProcessingState();
}

void FdnReverberationNewAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FdnReverberationNewAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void FdnReverberationNewAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    checkProcessingState();
    if (state == ProcessingState::pending)
        return;
    
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        if (reverberators.size() <= channel)
            reverberators.emplace_back(Reverberator(dimension, powers));
        reverberators[channel].Reverberate(channelData, blockLength, drywet);
    }
}

//==============================================================================
bool FdnReverberationNewAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FdnReverberationNewAudioProcessor::createEditor()
{
    return new FdnReverberationNewAudioProcessorEditor (*this);
}

//==============================================================================
void FdnReverberationNewAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FdnReverberationNewAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FdnReverberationNewAudioProcessor();
}
