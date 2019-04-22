#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
//InfoComponent methods


InfoComponent::InfoComponent()
{
    
}

void InfoComponent::paint (Graphics&)
{
    
}

void InfoComponent::resized()
{
    
}

const void InfoComponent::showInfo (String&& str)
{
    
}

const void InfoComponent::showIR (Reverberator::FdnDimension dimension, const std::vector<int>& delays)
{
    
}

//==============================================================================
//AuxComponent methods

AuxComponent::AuxComponent(FdnReverberationNewAudioProcessor& processor, MessageListener& msgListener, const InfoComponent& infoComp) :
        processor(processor),
        msgListenerToPost(msgListener),
        InfoComp(infoComp)
{
}

//==============================================================================
//MatrixComponent methods

MatrixComponent::MatrixComponent(FdnReverberationNewAudioProcessor& processor, const InfoComponent& infoComp, MessageListener& msgListener, Reverberator::FdnDimension matrixDim):
        AuxComponent(processor, msgListener, infoComp),
        currentMatrixDim(matrixDim)
{
    std::vector<Reverberator::FdnDimension> allDimValues =
    {Reverberator::FdnDimension::matrix2d, Reverberator::FdnDimension::matrix4d,
        Reverberator::FdnDimension::matrix8d, Reverberator::FdnDimension::matrix16d};
    
    for (Reverberator::FdnDimension iDim : allDimValues)
    {
        ToggleButton* newButton = new ToggleButton(String(int(iDim)) + "x" + String(int(iDim)));
        addAndMakeVisible(*newButton);
        newButton->setToggleState((iDim == matrixDim) ? true : false, dontSendNotification);
        newButton->setRadioGroupId(GroupID);
        newButton->addListener(this);
        matrixButtons.emplace_back(newButton);
    }
    
    addAndMakeVisible(nameLabel);
    nameLabel.setText("Matrix Dimensions", dontSendNotification);
    nameLabel.setFont(Font(22.0f));
    nameLabel.setJustificationType(Justification::centred);
}

void MatrixComponent::paint (Graphics&)
{
    
}

void MatrixComponent::resized()
{
    auto r = getLocalBounds();
    nameLabel.setBounds(0, r.getHeight() / 4, r.getWidth(), r.getHeight() / 4);
    
    decltype(r.getWidth()) buttonZoneWidth = r.getWidth() / matrixButtons.size();
    decltype(r.getWidth()) buttonWShift = buttonZoneWidth / 2;
    decltype(r.getWidth()) buttonHShift = r.getHeight() / 2 + r.getHeight() / 4;
    decltype(r.getWidth()) buttonSize = 100;
    
    for (auto &it : matrixButtons)
    {
        it->setBounds(buttonWShift - buttonSize / 2, buttonHShift - buttonSize / 2, buttonSize, buttonSize);
        buttonWShift += buttonZoneWidth;
    }
}

void MatrixComponent::buttonClicked (Button* button)
{
    Reverberator::FdnDimension newMatrix = currentMatrixDim;
    if (button == matrixButtons[0].get())
        newMatrix = Reverberator::FdnDimension::matrix2d;
    else if (button == matrixButtons[1].get())
        newMatrix = Reverberator::FdnDimension::matrix4d;
    else if (button == matrixButtons[2].get())
        newMatrix = Reverberator::FdnDimension::matrix8d;
    else if (button == matrixButtons[3].get())
        newMatrix = Reverberator::FdnDimension::matrix16d;
    
    if (currentMatrixDim == newMatrix)
        return;
    
    MatrixMessage* matrixMsg = new MatrixMessage(newMatrix);
    msgListenerToPost.postMessage(matrixMsg);
    currentMatrixDim = newMatrix;
}

//==============================================================================
//DelayComponent methods

DelayComponent::DelayComponent(FdnReverberationNewAudioProcessor& processor, const InfoComponent& infoComp, MessageListener& msgListener, std::vector<int>& delays):
        AuxComponent(processor, msgListener, infoComp),
        randomButton("Randomize"),
        applyButton("Apply")
{
    std::srand(std::time(nullptr));
    
    auto delayQuantity = delays.size();
    
    for (auto i = 0; i < delayQuantity; ++i)
    {
        Slider* newSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextEntryBoxPosition::TextBoxLeft);
        addAndMakeVisible(*newSlider);
        newSlider->setRange(1, MaxDelayValue, 1);
        newSlider->setValue(correctSliderValue(delays[i]));
        newSlider->addListener(this);
        delaySliders.emplace_back(newSlider);
    }
    
    addAndMakeVisible(randomButton);
    randomButton.onClick = [this]() {randomizeSliders();};
    
    addAndMakeVisible(applyButton);
    applyButton.onClick = [this]() {sendMessage();};
    
    addAndMakeVisible(nameLabel);
    nameLabel.setText("Delays", dontSendNotification);
    nameLabel.setFont(Font(22.0f));
    nameLabel.setJustificationType(Justification::centred);
}

int DelayComponent::correctSliderValue(int value)
{
    auto newValue = value % MaxDelayValue;
    return (newValue) ? newValue : MaxDelayValue;
}

void DelayComponent::paint (Graphics&)
{
    
}

void DelayComponent::resized()
{
    auto r = getLocalBounds();
    decltype(r.getWidth()) nameZoneHeigth = r.getHeight() / 4;
    nameLabel.setBounds(0, 0, r.getWidth(), nameZoneHeigth);
    
    decltype(r.getWidth()) sliderZoneHeigth = r.getHeight() / 2;
    
    if (delaySliders.size() <= 8) // one row of sliders
    {
        decltype(r.getWidth()) sliderZoneWidth = r.getWidth() / delaySliders.size();
        decltype(r.getWidth()) sliderWShift = sliderZoneWidth / 2;
        decltype(r.getWidth()) sliderHShift = nameZoneHeigth + sliderZoneHeigth / 2;
        decltype(r.getWidth()) sliderSize = std::min(150, (int)(r.getWidth() / delaySliders.size()));
        
        for (auto &it : delaySliders)
        {
            it->setBounds(sliderWShift - sliderSize / 2, sliderHShift - sliderSize / 2, sliderSize, sliderSize);
            sliderWShift += sliderZoneWidth;
        }
    }
    else // two rows of sliders
    {
        decltype(r.getWidth()) sliderZoneWidth = r.getWidth() / delaySliders.size() * 2;
        decltype(r.getWidth()) sliderWShift = sliderZoneWidth / 2;
        decltype(r.getWidth()) sliderHShift = nameZoneHeigth + sliderZoneHeigth / 4;
        decltype(r.getWidth()) sliderSize = std::min(150, int(r.getWidth() / (delaySliders.size() / 2)));
        
        for (auto &it : delaySliders)
        {
            it->setBounds(sliderWShift - sliderSize / 2, sliderHShift - sliderSize / 2, sliderSize, sliderSize);
            sliderWShift += sliderZoneWidth;
            if (sliderWShift >= r.getWidth())
            {
                sliderWShift = sliderZoneWidth / 2;
                sliderHShift += sliderZoneHeigth / 2;
            }
        }
    }
    
    auto buttonZoneHeigth = r.getHeight() - sliderZoneHeigth - nameZoneHeigth;
    decltype(r.getWidth()) buttonWidth = 150;
    decltype(r.getWidth()) buttonHeight = std::min(50, buttonZoneHeigth);
    randomButton.setBounds(r.getWidth() / 4 - buttonWidth / 2, sliderZoneHeigth + nameZoneHeigth + buttonZoneHeigth / 2 - buttonHeight / 2, buttonWidth, buttonHeight);
    applyButton.setBounds(3 * r.getWidth() / 4 - buttonWidth / 2, sliderZoneHeigth + nameZoneHeigth + buttonZoneHeigth / 2 - buttonHeight / 2, buttonWidth, buttonHeight);
}

void DelayComponent::randomizeSliders()
{
    // TODO add stop processing!
    for (auto &it : delaySliders)
    {
        auto sliderValue = correctSliderValue(std::rand());
        it.get()->setValue(sliderValue);
    }
}

void DelayComponent::updateSliders(std::vector<int>& inDelays)
{
    processor.setProcessingFlag(FdnReverberationNewAudioProcessor::ProcessingFlag::forbidden);
    
    delaySliders.clear();
    
    auto delayQuantity = inDelays.size();
    for (auto i = 0; i < delayQuantity; ++i)
    {
        Slider* newSlider = new Slider(Slider::RotaryVerticalDrag, Slider::TextEntryBoxPosition::TextBoxLeft);
        addAndMakeVisible(*newSlider);
        newSlider->setRange(1, MaxDelayValue, 1);
        newSlider->setValue(correctSliderValue(MaxDelayValue));
        newSlider->addListener(this);
        delaySliders.emplace_back(newSlider);
    }
    
    resized();
}

void DelayComponent::sendMessage()
{
    std::vector<int> delays;
    for (auto &it : delaySliders)
        delays.push_back(it.get()->getValue());
    
    DelaysMessage* delaysMsg = new DelaysMessage(std::move(delays));
    msgListenerToPost.postMessage(delaysMsg);
}

void DelayComponent::sliderValueChanged (Slider* slider)
{
    processor.setProcessingFlag(FdnReverberationNewAudioProcessor::ProcessingFlag::forbidden);
}

//==============================================================================
//AdditionalComponent methods

AdditionalComponent::AdditionalComponent(FdnReverberationNewAudioProcessor& processor, const InfoComponent& infoComp, MessageListener& msgListener):
    AuxComponent(processor, msgListener, infoComp),
    saveButton("Save Preset"),
    showIrButton("Show IR"),
    drywetSlider(Slider::RotaryVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
{
    addAndMakeVisible(saveButton);
    saveButton.onClick = [this]() {savePreset();};
    
    addAndMakeVisible(showIrButton);
    showIrButton.onClick = [this]() {showIR();};
    
    addAndMakeVisible(drywetSlider);
    drywetSlider.setRange(0, 100, 1);
    drywetSlider.setValue(50);
    drywetSlider.onValueChange = [this]() { this->processor.setDryWet((float)drywetSlider.getValue() / 100.0f); };
}


void AdditionalComponent::paint (Graphics&)
{
    
}

void AdditionalComponent::resized()
{
    auto r = getLocalBounds();
    decltype(r.getWidth()) compomentWidth = r.getWidth() / 3;
    decltype(r.getWidth()) centeredHeight = r.getHeight() / 2;
    decltype(r.getWidth()) buttonHeigth = std::min(40, r.getHeight());
    decltype(r.getWidth()) buttonWidth = std::min(100, compomentWidth);
    decltype(r.getWidth()) sliderSize = std::min({120, compomentWidth, r.getHeight()});
    
    showIrButton.setBounds(compomentWidth / 2 - buttonWidth / 2, centeredHeight - buttonHeigth / 2, buttonWidth, buttonHeigth);
    saveButton.setBounds(2 * compomentWidth + compomentWidth / 2 - buttonWidth / 2, centeredHeight - buttonHeigth / 2, buttonWidth, buttonHeigth);
    drywetSlider.setBounds(compomentWidth + compomentWidth / 2 - sliderSize / 2, centeredHeight - sliderSize / 2, sliderSize, sliderSize);
}

void AdditionalComponent::savePreset()
{
    //msgToPost->postMessage(&msg);
}

void AdditionalComponent::showIR()
{
    //InfoComp.showIR();
}

//==============================================================================
//MainComponent methods

MainComponent::MainComponent(FdnReverberationNewAudioProcessor& inProcessor, const InfoComponent& infoComp):
    delays(std::vector<int>(4, 0)),
    matrixDim(Reverberator::FdnDimension::matrix4d),
    matrixComp(inProcessor, infoComp, *this, matrixDim),
    delayComp(inProcessor, infoComp, *this, delays),
    additionalComp(inProcessor, infoComp, *this),
    processor(inProcessor),
    InfoComp(infoComp)
{
    addAndMakeVisible(matrixComp);
    addAndMakeVisible(delayComp);
    addAndMakeVisible(additionalComp);
    
    addAndMakeVisible(nameLabel);
    nameLabel.setText("Feedback Delay Networks Reverberation", dontSendNotification);
    nameLabel.setFont(Font(26.0f));
    nameLabel.setJustificationType(Justification::centred);
    
    processor.setDimension(matrixDim);
    processor.setDelayPowers(delays);
}

void MainComponent::paint (Graphics&)
{
    
}

void MainComponent::resized()
{
    auto r = getLocalBounds();
    auto nameHeigth = 30;
    auto remainingHeigth = r.getHeight() - nameHeigth;
    
    nameLabel.setBounds(0, 0, r.getWidth(), nameHeigth);
    matrixComp.setBounds(0, nameHeigth, r.getWidth(), remainingHeigth / 3);
    delayComp.setBounds(0, nameHeigth + remainingHeigth / 3, r.getWidth(), remainingHeigth / 3);
    additionalComp.setBounds(0, nameHeigth + 2 * remainingHeigth / 3, r.getWidth(), remainingHeigth / 3);
}

void MainComponent::handleMessage (const Message & message)
{
    try
    {
        const CommonMessage& commonMsg = dynamic_cast<const CommonMessage&>(message);
        if (commonMsg.getType() == CommonMessage::MsgType::matrix)
        {
            const MatrixMessage& matrixMsg = dynamic_cast<const MatrixMessage&>(commonMsg);
            matrixDim = matrixMsg.info;
            processor.setDimension(matrixDim);
            updateDelays();
        }
        else if (commonMsg.getType() == CommonMessage::MsgType::delays)
        {
            delays.clear();
            const DelaysMessage& matrixMsg = dynamic_cast<const DelaysMessage&>(commonMsg);
            delays = matrixMsg.info;
            processor.setDelayPowers(delays);
        }
    }
    catch (const std::bad_cast &e)
    {
        Logger* log = Logger::getCurrentLogger();
        log->writeToLog("Cannot do dynamic_cast!");
    }
    processor.setProcessingFlag(FdnReverberationNewAudioProcessor::ProcessingFlag::allowed);
}

void MainComponent::updateDelays()
{
    int necessaryDelaysQuantity = (int)matrixDim;
    if (necessaryDelaysQuantity < delays.size())
        while (delays.size() != necessaryDelaysQuantity)
            delays.pop_back();
    if (necessaryDelaysQuantity > delays.size())
        while (delays.size() != necessaryDelaysQuantity)
            delays.push_back(1);
    delayComp.updateSliders(delays);
    processor.setDelayPowers(delays);
}



//==============================================================================
//FdnReverberationNewAudioProcessorEditor methods

FdnReverberationNewAudioProcessorEditor::FdnReverberationNewAudioProcessorEditor (FdnReverberationNewAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p),
      infoComp(),
      mainComp(p, infoComp)
{
    addAndMakeVisible(infoComp);
    addAndMakeVisible(mainComp);
    setSize (1000, 600);
}

FdnReverberationNewAudioProcessorEditor::~FdnReverberationNewAudioProcessorEditor()
{
}

void FdnReverberationNewAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::darkgrey);
    getLookAndFeel().setColour(Slider::thumbColourId, Colours::white);
    getLookAndFeel().setColour(Slider::trackColourId, Colours::lightgrey);
    getLookAndFeel().setColour(Slider::textBoxHighlightColourId, Colours::transparentBlack);
    getLookAndFeel().setColour(Label::textColourId, Colours::whitesmoke);
}

void FdnReverberationNewAudioProcessorEditor::resized()
{
    mainComp.setBounds(0, 0, getWidth()*3/4, getHeight());
    infoComp.setBounds(getWidth()*1/4, 0, getWidth(), getHeight());
}
