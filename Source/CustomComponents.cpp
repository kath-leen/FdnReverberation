/*
  ==============================================================================

    CustomComponents.cpp
    Created: 3 May 2019 11:41:41am
    Author:  Ekaterina Poklonskaya

  ==============================================================================
*/

#include "CustomComponents.h"

CustomTextButton::CustomTextButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info) :
        TextButton(name),
        InfoSender(infoSender),
        Info(info)
{}

CustomTextButton::CustomTextButton(const String& name, const String& toolTip, const std::function<void(const String&)> infoSender, const String&& info) :
        TextButton(name, toolTip),
        InfoSender(infoSender),
        Info(info)
{}

CustomTextButton::CustomTextButton(const std::function<void(const String&)> infoSender, const String&& info) :
        TextButton(),
        InfoSender(infoSender),
        Info(info)
{}

void CustomTextButton::mouseEnter(const MouseEvent&)
{
    InfoSender(Info);
    
}
void CustomTextButton::mouseExit(const MouseEvent&)
{
    InfoSender("");
}

//-------------------------------------------------------

CustomToggleButton::CustomToggleButton(const String& name, const std::function<void(const String&)> infoSender, const String&& info) :
        ToggleButton(name),
        InfoSender(infoSender),
        Info(info)
{}

CustomToggleButton::CustomToggleButton(const std::function<void(const String&)> infoSender, const String&& info) :
        ToggleButton(),
        InfoSender(infoSender),
        Info(info)
{}

void CustomToggleButton::mouseEnter(const MouseEvent&)
{
    InfoSender(Info);
    
}
void CustomToggleButton::mouseExit(const MouseEvent&)
{
    InfoSender("");
}

//-------------------------------------------------------

CustomSlider::CustomSlider(Slider::SliderStyle style, Slider::TextEntryBoxPosition textBoxPosition, const std::function<void(const String&)> infoSender, const String&& info) :
        Slider(style, textBoxPosition),
        InfoSender(infoSender),
        Info(info)
{}

CustomSlider::CustomSlider(const String& componentName, const std::function<void(const String&)> infoSender, const String&& info) :
        Slider(componentName),
        InfoSender(infoSender),
        Info(info)
{}

CustomSlider::CustomSlider(const std::function<void(const String&)> infoSender, const String&& info) :
        Slider(),
        InfoSender(infoSender),
        Info(info)
{}

void CustomSlider::mouseEnter(const MouseEvent&)
{
    InfoSender(Info);
    
}
void CustomSlider::mouseExit(const MouseEvent&)
{
    InfoSender("");
}
