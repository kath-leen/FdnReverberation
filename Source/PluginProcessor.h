#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Reverberator.h"

//==============================================================================
/**
*/
class FdnReverberationNewAudioProcessor  : public AudioProcessor
{
public:
    enum class ProcessingFlag { forbidden = false, allowed = true };
    
    //==============================================================================
    FdnReverberationNewAudioProcessor(Reverberator::FdnDimentions dim = Reverberator::FdnDimentions::matrix4d, std::vector<int>&& pow = std::vector<int>{1, 2, 3, 4});
    ~FdnReverberationNewAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    void setDimention (Reverberator::FdnDimentions dim);
    void setDelayPowers (std::vector<int>& pow);
    void setProcessingFlag (ProcessingFlag inFlag);
    void setDryWet (float inDryWet);

private:
    //==============================================================================
    enum class ProcessingState
    {
        pending, // waiting for a good conditions to process (see checkProcessingState() )
        running, // processing, but can be pended if the conditions become bad (see checkProcessingState() )
    };
    void checkProcessingState ();
    
    std::vector<Reverberator> reverberators; //according to the amount of channels
    ProcessingState state = ProcessingState::pending;
    ProcessingFlag flag = ProcessingFlag::allowed;
    Reverberator::FdnDimentions dimention;
    std::vector<int> powers;
    int channelsNum;
    int blockLength = 0;
    float drywet = 0.5f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FdnReverberationNewAudioProcessor)
};
