#ifndef stx_debug_logger
#define stx_debug_logger

// *** DO NOT EDIT THIS FILE ***

#include <string>
#include <iostream>

namespace stxDebug {

template<typename T>
struct GetDbgStringT;

template<typename T>
std::wstring GetDbgString(const T& t)
{
  return GetDbgStringT<T>::GetDbgString(t);
}

template<typename T>
struct GetDbgString_Trivial
{
  static std::wstring GetDbgString(const T& t) { return t; }
};

template<>
struct GetDbgStringT<std::wstring> : public GetDbgString_Trivial<std::wstring> {};

template<int len>
struct GetDbgStringT<wchar_t[len]> : public GetDbgString_Trivial<wchar_t[len]> {};

template<>
struct GetDbgStringT<std::string> {
  static std::wstring GetDbgString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
  }
};

template<int len>
struct GetDbgStringT<char[len]> : public GetDbgStringT<std::string> {};

/////////////////////////////////////////////////////////////////////////////

template <typename WriterFunctor>
class DebugLogger
{
  template<typename T>
  inline void Write(const T& t)
  {
    msg += GetDbgString(t);
  }

  std::wstring msg;
  WriterFunctor m_writer;

public:

  inline ~DebugLogger()
  {
    if (!msg.empty())
      m_writer(msg);
  }

  template<typename T>
  inline DebugLogger<WriterFunctor>& operator<<(const T& t)
  {
    Write(t);
    return *this;
  }
};
/////////////////////////////////////////////////////////////////////////////

struct StdErrWriter
{
  inline void operator()(const std::wstring& msg) {
    std::wcerr << L"[err]: " << msg << std::endl; 
  }
};

stxDebug::DebugLogger<StdErrWriter> Err() {
  return DebugLogger<StdErrWriter>();
}
/////////////////////////////////////////////////////////////////////////////

struct StdOutWriter
{
  inline void operator()(const std::wstring& msg) {
    std::wcout << L"[out]: " << msg << std::endl; 
  }
};

stxDebug::DebugLogger<StdOutWriter> Out() {
  return DebugLogger<StdOutWriter>();
}
/////////////////////////////////////////////////////////////////////////////

} // namespace stxDebug

#endif
