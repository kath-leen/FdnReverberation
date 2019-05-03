#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Reverberator.h"
#include "CustomComponents.h"
#include "vector"
#include "array"

//==============================================================================
/**
*/
class CommonMessage : public Message
{
public:
    enum class MsgType {matrix, delays};
    
    CommonMessage(MsgType inputType) : type(inputType) {};
    MsgType getType() const {return type;};
    
private:
    MsgType type;
};

class MatrixMessage : public CommonMessage
{
public:
    MatrixMessage(const Reverberator::FdnDimension inputInfo) : CommonMessage(CommonMessage::MsgType::matrix), info(inputInfo) {};
    
    Reverberator::FdnDimension info;
};

class DelaysMessage : public CommonMessage
{
public:
    DelaysMessage(const std::vector<int>&& inputInfo) : CommonMessage(CommonMessage::MsgType::delays), info(inputInfo) {};
    
    std::vector<int> info;
};


//==============================================================================
class InfoComponent : public Component
{
public:
    InfoComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void showInfo (const String& str);
    void showIR (Reverberator::FdnDimension dimension, const std::vector<int>& delays);
    
private:
    float* setPulse ();
    std::pair<float, float> findDataBoundaries ();
    void drawData (Graphics&);
    
    Label infoLabel;
    bool toShowIR = false;
    std::array<float, 44100> data;
    
    const int SamplesQuantity;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoComponent)
};

//==============================================================================
class AuxComponent : public Component
{
public:
    AuxComponent(FdnReverberationNewAudioProcessor& processor, MessageListener& msgListener, InfoComponent& infoComp);
    
protected:
    FdnReverberationNewAudioProcessor& processor;
    MessageListener& msgListenerToPost;
    
    InfoComponent& infoComp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxComponent)
};

//==============================================================================
class MatrixComponent : public AuxComponent, public Button::Listener
{
public:
    MatrixComponent(FdnReverberationNewAudioProcessor& processor, InfoComponent& infoComp, MessageListener& msgListener, Reverberator::FdnDimension matrixDim);
    void paint (Graphics&) override;
    void resized() override;
    
private:
    void buttonClicked (Button* button) override;
    
    std::vector<std::unique_ptr<CustomToggleButton>> matrixButtons;
    Label nameLabel;
    
    Reverberator::FdnDimension currentMatrixDim;
    const int GroupID = 1;
};

//==============================================================================
class DelayComponent : public AuxComponent, public Slider::Listener
{
public:
    DelayComponent(FdnReverberationNewAudioProcessor& processor, InfoComponent& infoComp, MessageListener& msgListener, const std::vector<int>& delays);
    void paint (Graphics&) override;
    void resized() override;
    
    void updateSliders(const std::vector<int>& inDelays);
    
private:
    int correctSliderValue(int value);
    void sliderValueChanged (Slider* slider) override;
    void randomizeSliders();
    void sendMessage();
    
    std::vector<std::unique_ptr<CustomSlider>> delaySliders;
    CustomTextButton randomButton;
    CustomTextButton applyButton;
    Label nameLabel;
    
    const int MaxDelayValue = 10;
};

//==============================================================================
class AdditionalComponent : public AuxComponent
{
public:
    AdditionalComponent(FdnReverberationNewAudioProcessor& processor, InfoComponent& infoComponent, MessageListener& msgListener);
    void paint (Graphics&) override;
    void resized() override;
    
private:
    
    void savePreset();
    void showIR();
    
    CustomTextButton saveButton;
    CustomTextButton showIrButton;
    CustomSlider drywetSlider;
};

//==============================================================================
class MainComponent : public Component, public MessageListener
{
public:
    MainComponent(FdnReverberationNewAudioProcessor& processor, InfoComponent& infoComp);
    
    void paint (Graphics&) override;
    void resized() override;
    
    virtual void handleMessage (const Message & message) override;
    
private:
    void updateDelays();
    
    std::vector<int> delays;
    Reverberator::FdnDimension matrixDim;
    
    MatrixComponent matrixComp;
    DelayComponent delayComp;
    AdditionalComponent additionalComp;
    
    Label nameLabel;
    
    FdnReverberationNewAudioProcessor& processor;
    InfoComponent& infoComp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


//==============================================================================
class FdnReverberationNewAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    FdnReverberationNewAudioProcessorEditor (FdnReverberationNewAudioProcessor&);
    ~FdnReverberationNewAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;

private:
    FdnReverberationNewAudioProcessor& processor;
    
    InfoComponent infoComp;
    MainComponent mainComp;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FdnReverberationNewAudioProcessorEditor)
};
