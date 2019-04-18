#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Reverberator.h"
#include "vector"

//==============================================================================
/**
*/
class CommonMessage : public Message
{
public:
    enum class MsgTypes {matrix = 0, delays};
    
    CommonMessage(MsgTypes inputType) : type(inputType) {};
    
    MsgTypes type;
};

class MatrixMessage : public CommonMessage
{
public:
    MatrixMessage(Reverberator::FdnDimentions& inputInfo) : CommonMessage(CommonMessage::MsgTypes::matrix), info(inputInfo) {};
    
    Reverberator::FdnDimentions info;
};

class DelaysMessage : public CommonMessage
{
public:
    DelaysMessage(std::vector<int>& inputInfo) : CommonMessage(CommonMessage::MsgTypes::delays), info(inputInfo) {};
    
    std::vector<int> info;
};


//==============================================================================
class InfoComponent : public Component
{
public:
    InfoComponent();
    
    void paint (Graphics&) override;
    void resized() override;
    
    const void showInfo (String&& str);
    const void showIR (Reverberator::FdnDimentions dimention, std::vector<int>& delays);
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoComponent)
};

//==============================================================================
class AuxComponent : public Component
{
public:
    AuxComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComp, MessageListener& msg);
    
protected:
    FdnReverberationNewAudioProcessor& processor;
    MessageListener& msgToPost;
    
    const InfoComponent& InfoComp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxComponent)
};

//==============================================================================
class MatrixComponent : public AuxComponent, public Button::Listener
{
public:
    MatrixComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComp, MessageListener& msg, Reverberator::FdnDimentions inMatrix);
    void paint (Graphics&) override;
    void resized() override;
    
private:
    void buttonClicked (Button* button) override;
    
    std::vector<std::unique_ptr<ToggleButton>> matrixButtons;
    Label nameLabel;
    
    Reverberator::FdnDimentions currentMatrix;
    const int GroupID = 1;
};

//==============================================================================
class DelayComponent : public AuxComponent, public Slider::Listener
{
public:
    DelayComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComp, MessageListener& msg, std::vector<int>& inDelays);
    void paint (Graphics&) override;
    void resized() override;
    
    void updateSliders(std::vector<int>& inDelays);
    
private:
    int correctSliderValue(int value);
    void sliderValueChanged (Slider* slider) override;
    void randomizeSliders();
    void sendMessage();
    
    std::vector<std::unique_ptr<Slider>> delaySliders;
    TextButton randomButton;
    TextButton applyButton;
    Label nameLabel;
    
    const int MaxDelayValue = 10;
};

//==============================================================================
class AdditionalComponent : public AuxComponent
{
public:
    AdditionalComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComponent, MessageListener& msg);
    void paint (Graphics&) override;
    void resized() override;
    
private:
    
    void savePreset();
    void showIR();
    
    TextButton saveButton;
    TextButton showIrButton;
    Slider drywetSlider;
};

//==============================================================================
class MainComponent : public Component, public MessageListener
{
public:
    MainComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComp);
    
    void paint (Graphics&) override;
    void resized() override;
    
    virtual void handleMessage (const Message & message) override;
    
private:
    void updateDelays();
    
    std::vector<int> delays;
    Reverberator::FdnDimentions matrix;
    
    MatrixComponent matrixComp;
    DelayComponent delayComp;
    AdditionalComponent additionalComp;
    
    Label nameLabel;
    
    FdnReverberationNewAudioProcessor& processor;
    const InfoComponent& InfoComp;
    
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
