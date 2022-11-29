#include "support_machinery.hpp"
#define FAIL SM_FAIL
namespace sm = support_machinery;

#include "winapi.hpp"
#include "winapi-lib.comctl32.hpp"

#include <functional>           // std::invoke

#include <stdio.h>              // fprintf
#include <stdlib.h>             // EXIT_SUCCESS, EXIT_FAILURE

namespace app {
    const HINSTANCE instance_handle = winapi::h_instance;

    namespace child_window {
        using   sm::hopefully, sm::C_string_ref;
        using   std::invoke;            // <functional>

        auto windowclass_id()
            -> ATOM
        {
            static const ATOM the_id = invoke( []() -> ATOM
            {
                WNDCLASS params = {};
                params.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
                params.lpfnWndProc      = &DefWindowProc;
                params.hInstance        = app::instance_handle;
                params.hCursor          = ::LoadCursor( {}, IDC_ARROW );
                params.hbrBackground    = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 );
                params.lpszClassName    = "ChildWindow";
                
                const ATOM result = ::RegisterClass( &params );
                hopefully( result != 0 )
                    or FAIL( "::RegisterClass failed." );
                return result;
            } );
            return the_id;
        }
        
        auto create_with_parent( const HWND parent, const C_string_ref title )
            -> HWND
        {
            const HWND window = winapi::create_child_window_of( parent, -1, windowclass_id(), title );
            const HMENU system_menu = ::GetSystemMenu( window, false );
            ::EnableMenuItem( system_menu, SC_CLOSE, MF_BYCOMMAND | MF_DISABLED );
            return window;
        }
    }  // namespace child_window

    namespace main_window {
        using   sm::hopefully, sm::C_string_ref;
        using   std::invoke;            // <functional>

        void on_wm_close()
        {
            ::PostQuitMessage( 0 );
        }
        
        auto message_handler(
            const HWND      window,
            const UINT      msg_id,
            const WPARAM    w_param,
            const LPARAM    ell_param
            )
            -> LRESULT
        {
            switch( msg_id ) {
                case WM_CLOSE:  return (on_wm_close(), 0);
            }
            return ::DefWindowProc( window, msg_id, w_param, ell_param );
        }

        auto windowclass_id()
            -> ATOM
        {
            static const ATOM the_id = invoke( []() -> ATOM
            {
                WNDCLASS params = {};
                params.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
                params.lpfnWndProc      = message_handler;
                params.hInstance        = app::instance_handle;
                params.hCursor          = ::LoadCursor( {}, IDC_ARROW );
                params.hbrBackground    = reinterpret_cast<HBRUSH>( COLOR_WINDOW + 1 );
                params.lpszClassName    = "MainWindow";
                
                const ATOM result = ::RegisterClass( &params );
                hopefully( result != 0 )
                    or FAIL( "::RegisterClass failed." );
                return result;
            } );
            return the_id;
        }

        auto create( const C_string_ref title )
            -> HWND
        {
            const HWND window = winapi::create_toplevel_window( windowclass_id(), title );
            winapi::set_icon_of( window, ::LoadIcon( 0, IDI_INFORMATION ) );
            const HWND child = child_window::create_with_parent( window, "Child window" );
            winapi::set_icon_of( child, winapi::icon_of( window ) );
            return window;
        }
    }  // namespace main_window
    
    void run()
    {
        using   sm::hopefully;

        comctl32::init_classes();   // Initialization for modern look and feel.
        const HWND window = main_window::create( "The main window" );
        ShowWindow( window, SW_SHOWDEFAULT );
        MSG msg = {};
        while( ::GetMessage( &msg, 0, 0, 0 ) > 0 ) {
            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
        }

        hopefully( msg.message == WM_QUIT and msg.wParam == 0 )
            or FAIL( "Something (that's all I know) failed, terminating; really sorry!" );
    }
}  // namespace app

auto main() -> int
{
    using   sm::in_;
    using   std::exception;

    try {
        app::run();
        return EXIT_SUCCESS;
    } catch( in_<exception> x ) {
        fprintf( stderr, "!%s\n", x.what() );
    }
    return EXIT_FAILURE;
}
