#ifndef SIMPLE_JSON_HPP
#define SIMPLE_JSON_HPP

#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <cstdio>

namespace simple_json {


  inline std::string escape_json_string(const std::string &s) {
      std::string result;
      for (char c : s) {
          switch (c) {
              case '"':  result += "\\\""; break;
              case '\\': result += "\\\\"; break;
              case '\b': result += "\\b";  break;
              case '\f': result += "\\f";  break;
              case '\n': result += "\\n";  break;
              case '\r': result += "\\r";  break;
              case '\t': result += "\\t";  break;
              default:
                  if (static_cast<unsigned char>(c) < 0x20) {
                      char buf[7];
                      std::snprintf(buf, sizeof(buf), "\\u%04x", c);
                      result += buf;
                  } else {
                      result.push_back(c);
                  }
          }
      }
      return result;
  }


  inline std::string serialize(const std::map<std::string, std::vector<std::vector<std::string>>> &data) {
      std::ostringstream oss;
      oss << "{";
      bool firstEntry = true;
      for (const auto &kv : data) {
          if (!firstEntry) {
              oss << ",";
          }
          firstEntry = false;
          oss << "\"" << escape_json_string(kv.first) << "\":";
          oss << "[";
          bool firstRow = true;
          for (const auto &row : kv.second) {
              if (!firstRow) {
                  oss << ",";
              }
              firstRow = false;
              oss << "[";
              bool firstElem = true;
              for (const auto &elem : row) {
                  if (!firstElem) {
                      oss << ",";
                  }
                  firstElem = false;
                  oss << "\"" << escape_json_string(elem) << "\"";
              }
              oss << "]";
          }
          oss << "]";
      }
      oss << "}";
      return oss.str();
  }

  class JSONParser {
  public:
      JSONParser(const std::string &input) : s(input), pos(0) {}

      std::map<std::string, std::vector<std::vector<std::string>>> parse() {
          skip_whitespace();
          auto result = parse_object();
          skip_whitespace();
          if (pos != s.size()) {
              throw std::runtime_error("Extra characters after JSON object");
          }
          return result;
      }

  private:
      const std::string &s;
      size_t pos;


      void skip_whitespace() {
          while (pos < s.size() && std::isspace(static_cast<unsigned char>(s[pos])))
              pos++;
      }

      char peek() {
          if (pos < s.size())
              return s[pos];
          throw std::runtime_error("Unexpected end of input");
      }


      char get() {
          if (pos < s.size())
              return s[pos++];
          throw std::runtime_error("Unexpected end of input");
      }


      void expect(char ch) {
          skip_whitespace();
          if (get() != ch) {
              throw std::runtime_error(std::string("Expected '") + ch + "'");
          }
      }

      std::string parse_string() {
          skip_whitespace();
          if (get() != '"')
              throw std::runtime_error("Expected '\"' at beginning of string");
          std::string result;
          while (true) {
              if (pos >= s.size())
                  throw std::runtime_error("Unterminated string");
              char c = get();
              if (c == '"')
                  break;
              if (c == '\\') {
                  if (pos >= s.size())
                      throw std::runtime_error("Unterminated escape sequence in string");
                  char esc = get();
                  switch (esc) {
                      case '"':  result.push_back('"');  break;
                      case '\\': result.push_back('\\'); break;
                      case '/':  result.push_back('/');  break;
                      case 'b':  result.push_back('\b'); break;
                      case 'f':  result.push_back('\f'); break;
                      case 'n':  result.push_back('\n'); break;
                      case 'r':  result.push_back('\r'); break;
                      case 't':  result.push_back('\t'); break;
                      default:
                          throw std::runtime_error("Unsupported escape sequence");
                  }
              } else {
                  result.push_back(c);
              }
          }
          return result;
      }

      std::vector<std::string> parse_string_array() {
          std::vector<std::string> arr;
          skip_whitespace();
          expect('[');
          skip_whitespace();
          if (peek() == ']') {
              get(); 
              return arr;
          }
          while (true) {
              std::string str = parse_string();
              arr.push_back(str);
              skip_whitespace();
              char c = get();
              if (c == ']')
                  break;
              else if (c != ',')
                  throw std::runtime_error("Expected ',' or ']' in string array");
          }
          return arr;
      }

      std::vector<std::vector<std::string>> parse_array_of_arrays_of_strings() {
          std::vector<std::vector<std::string>> arr;
          skip_whitespace();
          expect('[');
          skip_whitespace();
          if (peek() == ']') {
              get();
              return arr;
          }
          while (true) {
              std::vector<std::string> inner = parse_string_array();
              arr.push_back(inner);
              skip_whitespace();
              char c = get();
              if (c == ']')
                  break;
              else if (c != ',')
                  throw std::runtime_error("Expected ',' or ']' in array of arrays");
          }
          return arr;
      }


      std::map<std::string, std::vector<std::vector<std::string>>> parse_object() {
          std::map<std::string, std::vector<std::vector<std::string>>> obj;
          skip_whitespace();
          expect('{');
          skip_whitespace();
          if (peek() == '}') {
              get(); 
              return obj;
          }
          while (true) {
              skip_whitespace();
              std::string key = parse_string();
              skip_whitespace();
              expect(':');
              skip_whitespace();
              std::vector<std::vector<std::string>> value = parse_array_of_arrays_of_strings();
              obj[key] = value;
              skip_whitespace();
              char c = get();
              if (c == '}')
                  break;
              else if (c != ',')
                  throw std::runtime_error("Expected ',' or '}' in object");
          }
          return obj;
      }
  };


  inline std::map<std::string, std::vector<std::vector<std::string>>>
  deserialize(const std::string &json_str) {
      JSONParser parser(json_str);
      return parser.parse();
  }

} 

#endif 