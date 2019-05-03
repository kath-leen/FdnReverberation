/*
  ==============================================================================

    CustomComponents.h
    Created: 30 Apr 2019 12:37:14pm
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CustomTextButton : public TextButton
{
public:
    CustomTextButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info = "This is a text button");
    
    CustomTextButton(const String& name, const String& toolTip, const std::function<void(const String&)> infoSender, const String&& info = "This is a text button");
    
    CustomTextButton(const std::function<void(const String&)> infoSender, const String&& info = "This is a text button");
    
private:
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};


class CustomToggleButton : public ToggleButton
{
public:
    CustomToggleButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info = "This is a toggle button");
    
    CustomToggleButton(const std::function<void(const String&)> infoSender, const String&& info = "This is a toggle button");
    
private:
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};


class CustomSlider : public Slider
{
public:
    CustomSlider(Slider::SliderStyle style, Slider::TextEntryBoxPosition textBoxPosition, const std::function<void(const String&)> infoSender, const String&& info = "This is a slider");
    
    CustomSlider(const String& componentName, const std::function<void(const String&)> infoSender, const String&& info = "This is a slider");
    
    CustomSlider(const std::function<void(const String&)> infoSender, const String&& info = "This is a slider");
    
private:
    void mouseEnter(const MouseEvent&) override;
    void mouseExit(const MouseEvent&) override;
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};
