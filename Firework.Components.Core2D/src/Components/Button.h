#pragma once

#include "Firework.Components.Core2D.Exports.h"

#include <Drawing/Core.h>
#include <GL/Renderer.h>
#include <Objects/Component2D.h>

namespace Firework
{
    namespace Internal
    {
        struct ComponentCoreStaticInit;
    }

    enum class ButtonState
    {
        Idle = 0b00,
        Highlighted = 0b01,
        Pressed = 0b11
    };

    class __firework_componentcore2d_api ColorHighlightButton final : public Internal::Component2D
    {
        ButtonState buttonState;

        void renderOffload();
    public:
        Color idleColor { 0, 0, 0, 0 };
        Color highlightColor { 125, 125, 125, 125 };
        Color pressedColor { 255, 255, 255, 255 };

        inline ButtonState state()
        {
            return this->buttonState;
        }

        friend struct Firework::Internal::ComponentCoreStaticInit;
    };
}