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
    CustomTextButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info = "This is a text button") :
        TextButton(name), InfoSender(infoSender), Info(info) {};
    
    CustomTextButton(const String& name, const String& toolTip, const std::function<void(const String&)> infoSender, const String&& info = "This is a text button") :
        TextButton(name, toolTip), InfoSender(infoSender), Info(info) {};
    
    CustomTextButton(const std::function<void(const String&)> infoSender, const String&& info = "This is a text button") :
        TextButton(), InfoSender(infoSender), Info(info) {};
    
private:
    void mouseEnter(const MouseEvent&) override { InfoSender(Info); };
    void mouseExit(const MouseEvent&) override { InfoSender(""); };
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};


class CustomToggleButton : public ToggleButton
{
public:
    CustomToggleButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info = "This is a toggle button") :
        ToggleButton(name), InfoSender(infoSender), Info(info) {};
    
    CustomToggleButton(const std::function<void(const String&)> infoSender, const String&& info = "This is a toggle button") :
        ToggleButton(), InfoSender(infoSender), Info(info) {};
    
private:
    void mouseEnter(const MouseEvent&) override { InfoSender(Info); };
    void mouseExit(const MouseEvent&) override { InfoSender(""); };
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};


class CustomSlider : public Slider
{
public:
    CustomSlider (Slider::SliderStyle style, Slider::TextEntryBoxPosition textBoxPosition, const std::function<void(const String&)> infoSender, const String&& info = "This is a slider") :
        Slider(style, textBoxPosition), InfoSender(infoSender), Info(info) {};
    
    CustomSlider (const String& componentName, const std::function<void(const String&)> infoSender, const String&& info = "This is a slider") :
        Slider(componentName), InfoSender(infoSender), Info(info) {};
    
    CustomSlider (const std::function<void(const String&)> infoSender, const String&& info = "This is a slider") :
        Slider(), InfoSender(infoSender), Info(info) {};
    
private:
    void mouseEnter(const MouseEvent&) override { InfoSender(Info); };
    void mouseExit(const MouseEvent&) override { InfoSender(""); };
    
    const std::function<void(const String&)> InfoSender;
    const String Info;
};
