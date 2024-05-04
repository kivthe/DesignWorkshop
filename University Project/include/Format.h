#ifndef DW_FORMAT_H
#define DW_FORMAT_H

#include <string>
#include <type_traits>
#include <array>

namespace kiv {

template<char EscChar = '%', typename... Args>
constexpr std::string Format(std::string format, Args&&... args)
{
    static_assert((std::is_convertible<Args, std::string>::value && ...), "Can not convert parameter pack arguments");
    if (sizeof...(Args) <= 0) return format;
    std::array<std::string, sizeof...(Args)> arr{static_cast<std::string>(std::forward<Args>(args))...};
    std::string result = format;
    for(std::size_t index = 0; index < result.size(); ++index)
    {
        if(result[index] != EscChar || index + 1 >= result.size()) continue;
        if(result[index + 1] == EscChar) { result.erase(index + 1, 1); continue; }
        if(!std::isdigit(result[index + 1])) continue;
        std::size_t last_digit = index + 1;
        while(std::isdigit(last_digit) && last_digit + 1 < result.size()) {++last_digit;}
        std::size_t arr_index = std::stoul(result.substr(index + 1, last_digit - index)) - 1;
        if(arr_index >= arr.size()) continue;
        result.replace(index, last_digit - index + 1, arr[arr_index]);
    }
    return result;
}

template<typename... Args>
constexpr std::string CFormat(const std::string& c_format, Args... args) {
  if (c_format.empty() || sizeof...(Args) == 0) return c_format;
  int num = std::snprintf(nullptr, 0, c_format.c_str(), std::forward<Args>(args)...);
  if (num < 0) return c_format;
  try {
    std::unique_ptr<char[]> buffer(new char[num + 1]);
    int check = std::snprintf(buffer.get(), num + 1, c_format.c_str(), std::forward<Args>(args)...);
    if (check < 0) return c_format;
    std::string result(buffer.get(), buffer.get() + num);
    return result;
  } catch (std::exception& except) {
    return c_format;
  }
}

}

#endif