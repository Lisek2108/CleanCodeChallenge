#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
constexpr unsigned char FIRST_PRINTABLE_CHARACTER = 32;
constexpr unsigned char LAST_PRINTABLE_CHARACTER = 126;
constexpr unsigned char NUMBER_OF_CHARACTERS = (LAST_PRINTABLE_CHARACTER - FIRST_PRINTABLE_CHARACTER) + 1;
using Key = std::array<unsigned char, NUMBER_OF_CHARACTERS>;

// TODO: Your implementation goes here
Key generateKey()
{
  Key key;
  std::generate(key.begin(), key.end(), [n = FIRST_PRINTABLE_CHARACTER]() mutable { return n++; });
  const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  std::shuffle(key.begin(), key.end(), generator);
  return key;
}

template<typename T>
std::string transformPrintableString(const std::string &a_string, T a_transformation)
{
  std::string transformedString;
  std::transform(a_string.cbegin(), a_string.cend(), std::back_inserter(transformedString),
                 [&a_transformation](unsigned char a_character) {
                   assert(isprint(a_character));
                   return a_transformation(a_character);
                 });
  return transformedString;
}

std::string encrypt(const std::string &a_message, const Key &a_key)
{
  return transformPrintableString(a_message, [a_key](unsigned char a_character) {
    const auto it = std::find(a_key.cbegin(), a_key.cend(), a_character);
    return (it - a_key.cbegin()) + FIRST_PRINTABLE_CHARACTER;
  });
}
std::string decrypt(const std::string &a_message, const Key &a_key)
{
  return transformPrintableString(
      a_message, [a_key](unsigned char a_character) { return a_key[a_character - FIRST_PRINTABLE_CHARACTER]; });
}

void encryptDecryptAndPrint(const std::string &a_message)
{
  const Key key = generateKey();
  const std::string cypher = encrypt(a_message, key);
  const std::string result = decrypt(cypher, key);

  std::cout << "Message: " << a_message << '\n';
  std::cout << "Cypher:  " << cypher << '\n';
  std::cout << "Result:  " << result << '\n';

  assert(a_message == result);
}

int main()
{
  // Feel free change below code if needed.
  const std::string message = "Hello, Coders School!";
  encryptDecryptAndPrint(message);
  encryptDecryptAndPrint("<#$0me_+m{}r3+_[print@ble]_'&&char*a*cters()'?>//~");
  return 0;
}
