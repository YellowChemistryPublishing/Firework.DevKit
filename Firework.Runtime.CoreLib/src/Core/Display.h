#pragma once

#include "Firework.Runtime.CoreLib.Exports.h"

#include <atomic>
#include <Mathematics.h>

namespace Firework
{
    namespace Internal
    {
        class CoreEngine;
    }

    class __firework_corelib_api Window final
    {
        static int width;
        static int height;
        static bool resizing;
    public:
        /// @brief Query the width of the window.
        /// @return Width of the window in pixels.
        /// @note Main thread only.
        inline static int pixelWidth()
        {
            return Window::width;
        }
        /// @brief Query the height of the window.
        /// @return Height of the window in pixels.
        /// @note Main thread only.
        inline static int pixelHeight()
        {
            return Window::height;
        }
        /// @brief Query whether the window is resizing this frame.
        /// @return Whether the window is resizing.
        /// @note Main thread only.
        inline static bool isResizing()
        {
            return Window::resizing;
        }

        friend class Firework::Internal::CoreEngine;
    };
    
    class __firework_corelib_api Screen final
    {
        static int width;
        static int height;
        static int screenRefreshRate;
    public:
        /// @brief Query the width of the primary display.
        /// @return Width of the display in pixels.
        /// @note Main thread only.
        inline static int pixelWidth()
        {
            return Screen::width;
        }
        /// @brief Query the height of the primary display.
        /// @return Height of the display in pixels.
        /// @note Main thread only.
        inline static int pixelHeight()
        {
            return Screen::height;
        }
        /// @brief Query the refresh rate of the primary dislay.
        /// @return Refresh rate of the display in Hz.
        /// @note Main thread only.
        inline static int refreshRate()
        {
            return Screen::screenRefreshRate;
        }

        friend class Firework::Internal::CoreEngine;
    };
}
