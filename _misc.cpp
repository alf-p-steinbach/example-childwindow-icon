    inline void set_font( const HWND window, const HFONT font )
    {
        ::SendMessage( window, WM_SETFONT, reinterpret_cast<WPARAM>( font ), true );
    }

    inline auto get_ui_font_spec()   // Costly
        -> LOGFONT
    {
        NONCLIENTMETRICS info = {sizeof( NONCLIENTMETRICS )};
        
        ::SystemParametersInfo( SPI_GETNONCLIENTMETRICS, info.cbSize, &info, {} )
            or SM_FAIL( "::SystemParametersInfo failed" );
        return info.lfMessageFont;
    }
    
    inline auto ui_font_spec()
        -> const LOGFONT&
    {
        static const LOGFONT the_spec = get_ui_font_spec();
        return the_spec;
    }

    inline auto ui_font()
        -> HFONT
    {
        static const HFONT the_font = ::CreateFontIndirect( &ui_font_spec() );
        hopefully( the_font != 0 )
            or SM_FAIL( "::CreateFontIndirect failed" );
        return the_font;
    }
    
