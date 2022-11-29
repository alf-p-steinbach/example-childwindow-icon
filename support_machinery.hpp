#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").

#include <stdexcept>
#include <string>

#define SM_FAIL( msg ) support_machinery::fail( std::string() + __func__ + " - " + (msg) )

namespace support_machinery {
    using   std::runtime_error,     // <stdexcept>
            std::string;
            
    template< class Type > using in_ = const Type&;
    
    constexpr auto hopefully( const bool condition ) -> bool { return condition; }
    inline auto fail( in_<string> message ) -> bool { throw runtime_error( message ); }
    
    class C_string_ref
    {
        const char*     m_str;
        
    public:
        constexpr C_string_ref( const char* s ): m_str( s ) {}
        constexpr auto pointer() const -> const char* { return m_str; }
        constexpr operator const char* () const { return m_str; }
    };
}  // namespace support_machinery
