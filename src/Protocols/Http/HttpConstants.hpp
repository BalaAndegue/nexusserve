#pragma once
#include <string_view>

namespace Protocols::Http {

inline constexpr std::string_view CRLF       = "\r\n";
inline constexpr std::string_view HEADER_SEP = ": ";
inline constexpr std::string_view BODY_SEP   = "\r\n\r\n";

inline constexpr int STATUS_OK         = 200;
inline constexpr int STATUS_NOT_FOUND  = 404;
inline constexpr int STATUS_BAD_REQ    = 400;
inline constexpr int STATUS_SERVER_ERR = 500;

} // namespace Protocols::Http
