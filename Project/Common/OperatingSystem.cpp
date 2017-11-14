// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#include <thread>
#include <chrono>
#include "OperatingSystem.h"

namespace ApiWithoutSecrets {

  namespace OS {

    Window::Window() :
      Parameters() {
    }

    WindowParameters Window::GetParameters() const {
      return Parameters;
    }

    Window::~Window() {
      XDestroyWindow( Parameters.DisplayPtr, Parameters.Handle );
      XCloseDisplay( Parameters.DisplayPtr );
    }

    bool Window::Create( const char *title ) {
      Parameters.DisplayPtr = XOpenDisplay( nullptr );
      if( !Parameters.DisplayPtr ) {
        return false;
      }

      int default_screen = DefaultScreen( Parameters.DisplayPtr );

      Parameters.Handle = XCreateSimpleWindow(
        Parameters.DisplayPtr,
        DefaultRootWindow( Parameters.DisplayPtr ),
        20,
        20,
        500,
        500,
        1,
        BlackPixel( Parameters.DisplayPtr, default_screen ),
        WhitePixel( Parameters.DisplayPtr, default_screen ) );

      // XSync( Parameters.DisplayPtr, false );
      XSetStandardProperties( Parameters.DisplayPtr, Parameters.Handle, title, title, None, nullptr, 0, nullptr );
      XSelectInput( Parameters.DisplayPtr, Parameters.Handle, ExposureMask | KeyPressMask | StructureNotifyMask );

      return true;
    }

    bool Window::RenderingLoop( TutorialBase &tutorial ) const {
      // Prepare notification for window destruction
      Atom delete_window_atom;
      delete_window_atom = XInternAtom( Parameters.DisplayPtr, "WM_DELETE_WINDOW", false );
      XSetWMProtocols( Parameters.DisplayPtr, Parameters.Handle, &delete_window_atom, 1);

      // Display window
      XClearWindow( Parameters.DisplayPtr, Parameters.Handle );
      XMapWindow( Parameters.DisplayPtr, Parameters.Handle );

      // Main message loop
      XEvent event;
      bool loop = true;
      bool resize = false;
      bool result = true;

      while( loop ) {
        if( XPending( Parameters.DisplayPtr ) ) {
          XNextEvent( Parameters.DisplayPtr, &event );
          switch( event.type ) {
            //Process events
          case ConfigureNotify: {
              static int width = event.xconfigure.width;
              static int height = event.xconfigure.height;

              if( ((event.xconfigure.width > 0) && (event.xconfigure.width != width)) ||
                ((event.xconfigure.height > 0) && (event.xconfigure.width != height)) ) {
                width = event.xconfigure.width;
                height = event.xconfigure.height;
                resize = true;
              }
            }
            break;
          case KeyPress:
            loop = false;
            break;
          case DestroyNotify:
            loop = false;
            break;
          case ClientMessage:
            if( static_cast<unsigned int>(event.xclient.data.l[0]) == delete_window_atom ) {
              loop = false;
            }
            break;
          }
        } else {
          // Draw
          if( resize ) {
            resize = false;
            if( !tutorial.OnWindowSizeChanged() ) {
              result = false;
              break;
            }
          }
          if( tutorial.ReadyToDraw() ) {
            if( !tutorial.Draw() ) {
              result = false;
              break;
            }
          } else {
            std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
          }
        }
      }

      return result;
    }
  } // namespace OS

} // namespace ApiWithoutSecrets
