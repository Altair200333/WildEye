#pragma once
#include <string_view>

//As shown here https://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c/56766138#56766138
//this code can be used to get compile-time type name
namespace Reflections
{
    template <typename T>
    constexpr auto type_name() noexcept
	{
        std::string_view name = "Error: unsupported compiler", prefix, suffix;
		#ifdef __clang__
	        name = __PRETTY_FUNCTION__;
	        prefix = "auto type_name() [T = ";
	        suffix = "]";
		#elif defined(__GNUC__)
	        name = __PRETTY_FUNCTION__;
	        prefix = "constexpr auto type_name() [with T = ";
	        suffix = "]";
		#elif defined(_MSC_VER)
	        name = __FUNCSIG__;
	        prefix = "auto __cdecl type_name<";
	        suffix = ">(void) noexcept";
		#endif
        name.remove_prefix(prefix.size());
        name.remove_suffix(suffix.size());
        return name;
    }
}
