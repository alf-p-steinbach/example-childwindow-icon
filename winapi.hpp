#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <windows-api-header-wrappers/windows-h.for-utf-8.hpp>  // `UNICODE` not defined.

#include "support_machinery.hpp"        // SM_FAIL

#include <stddef.h>         // offsetof
#include <stdint.h>         // intptr_t

#include <stdio.h> //! DEBUG

namespace winapi {
    namespace sm = support_machinery;
    using sm::hopefully, sm::C_string_ref;

    const HINSTANCE h_instance = ::GetModuleHandle( nullptr );

    inline auto as_pseudo_pointer( const ATOM value )
        -> const char*
    { return MAKEINTATOM( value ); }
    
    inline auto create_toplevel_window( const ATOM class_id, const C_string_ref title )
        -> HWND
    {
        const HWND handle = ::CreateWindow(
            as_pseudo_pointer( class_id ),      // lpClassName,
            title.pointer(),
            WS_OVERLAPPEDWINDOW,                // style bits
            CW_USEDEFAULT, CW_USEDEFAULT,       // x, y
            640, 400,                           // w, h
            {},                                 // hWndParent
            {},                                 // hMenu
            h_instance,                         // hInstance
            {}                                  // lpParam
            );
        hopefully( handle != 0 )
            or FAIL( "::CreateWindow failed." );
        // set_font( handle, ui_font() );
        return handle;
    }

    inline auto create_child_window_of( const HWND parent, const int id, const ATOM class_id, const C_string_ref title )
        -> HWND
    {
        const HWND handle = ::CreateWindow(
            as_pseudo_pointer( class_id ),      // lpClassName,
            title.pointer(),
            WS_CHILD | WS_BORDER | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_VISIBLE,    // style bits
            CW_USEDEFAULT, CW_USEDEFAULT,       // x, y
            240, 110,                           // w, h
            parent,                             // hWndParent
            reinterpret_cast<HMENU>( static_cast<intptr_t>( id ) ),      // hMenu
            h_instance,                         // hInstance
            {}                                  // lpParam
            );
        hopefully( handle != 0 )
            or FAIL( "::CreateWindow failed." );
        // set_font( handle, ui_font() );
        return handle;
    }
    
    inline auto set_icon_of( const HWND window, const HICON icon )
        -> HICON    // The old now replaced icon.
    {
        return reinterpret_cast<HICON>(
            ::SendMessage( window, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>( icon ) )
            );
    }

    inline auto icon_of( const HWND window )
        -> HICON
    {
        return reinterpret_cast<HICON>(
            ::SendMessage( window, WM_GETICON, ICON_SMALL, {} )
            );
    }
            
    inline auto set_app_icon_of( const HWND window, const HICON icon )
        -> HICON    // The old now replaced icon.
    {
        return reinterpret_cast<HICON>(
            ::SendMessage( window, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>( icon ) )
            );
    }

    inline auto app_icon_of( const HWND window )
        -> HICON
    {
        return reinterpret_cast<HICON>(
            ::SendMessage( window, WM_GETICON, ICON_BIG, {} )
            );
    }
            
}  // namespace winapi
