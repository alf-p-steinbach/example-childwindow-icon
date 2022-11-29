#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <windows-api-header-wrappers/windows-h.for-utf-8.hpp>  // `UNICODE` not defined.
#include <commctrl.h>

#include "support_machinery.hpp"        // SM_FAIL

namespace comctl32 {
    constexpr DWORD minimum_classes = ICC_STANDARD_CLASSES;

    inline void init_classes( const DWORD classes = minimum_classes )
    {
        INITCOMMONCONTROLSEX params = {sizeof( params ), classes };
        ::InitCommonControlsEx( &params )
            or SM_FAIL( "::InitCommonControlsEx failed." );
    }
}  // namespace comctl32
