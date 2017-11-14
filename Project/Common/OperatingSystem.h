// Copyright 2016 Intel Corporation All Rights Reserved
// 
// Intel makes no representations about the suitability of this software for any purpose.
// THIS SOFTWARE IS PROVIDED ""AS IS."" INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES,
// EXPRESS OR IMPLIED, AND ALL LIABILITY, INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES,
// FOR THE USE OF THIS SOFTWARE, INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY
// RIGHTS, AND INCLUDING THE WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Intel does not assume any responsibility for any errors which may appear in this software
// nor any responsibility to update it.

#if !defined(OPERATING_SYSTEM_HEADER)
#define OPERATING_SYSTEM_HEADER

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <cstdlib>

#include <cstring>
#include <iostream>

namespace ApiWithoutSecrets {

  namespace OS {

    // ************************************************************ //
    // LibraryHandle                                                //
    //                                                              //
    // Dynamic Library OS dependent type                            //
    // ************************************************************ //
    // 
    typedef void* LibraryHandle;


    // ************************************************************ //
    // OnWindowSizeChanged                                          //
    //                                                              //
    // Base class for handling window size changes                  //
    // ************************************************************ //
    class TutorialBase {
    public:
      virtual bool OnWindowSizeChanged() = 0;
      virtual bool Draw() = 0;

      virtual bool ReadyToDraw() const final {
        return CanRender;
      }

      TutorialBase() :
        CanRender( false ) {
      }

      virtual ~TutorialBase() {
      }

    protected:
      bool CanRender;
    };

    // ************************************************************ //
    // WindowParameters                                             //
    //                                                              //
    // OS dependent window parameters                               //
    // ************************************************************ //
    struct WindowParameters {
      Display            *DisplayPtr;
      Window              Handle;

      WindowParameters() :
        DisplayPtr(),
        Handle() {
      }
    };

    // ************************************************************ //
    // Window                                                       //
    //                                                              //
    // OS dependent window creation and destruction class           //
    // ************************************************************ //
    class Window {
    public:
      Window();
      ~Window();

      bool              Create( const char *title );
      bool              RenderingLoop( TutorialBase &tutorial ) const;
      WindowParameters  GetParameters() const;

    private:
      WindowParameters  Parameters;
    };

  } // namespace OS

} // namespace ApiWithoutSecrets

#endif // OPERATING_SYSTEM_HEADER
