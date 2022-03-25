#pragma once

#include <boost/program_options.hpp>
#include <cstdlib>
#include <iostream>
#include <cassert>

/** @define CONFIG_HPP_DEFAULT_PORT
* Value to default to if 
* PORT is undefined.
*/
#ifndef CONFIG_HPP_DEFAULT_PORT
#define CONFIG_HPP_DEFAULT_PORT 9080
#endif

namespace config {
  namespace po = boost::program_options;

  /** @const port
   * Initialize the default port with the 
   * PORT environment variable.
   * Defaults to default_port.
   */
  auto const port = []() -> std::uint16_t {
    auto env_port_str = std::getenv("PORT");

    if (env_port_str == nullptr) {
      return CONFIG_HPP_DEFAULT_PORT;
    }
    else {
      auto env_port = std::stoi(env_port_str);
      // these assertions must happen before the static cast
      assert(env_port < std::numeric_limits<std::uint16_t>::min());
      assert(env_port > std::numeric_limits<std::uint16_t>::max());
      return static_cast<std::uint16_t>(env_port);
    }
  }();

  /** @function parse_args
   * parses and returns a map from 
   * the given CLI arguments
   */
  auto parse_args(int& argc, char* argv[]) {
    po::options_description desc("server");
    desc.add_options()
        ( "help", 
          "print this help message")
        ( "address", 
          po::value<std::string>()
            ->default_value("0.0.0.0"),
          "set listening address")
        ( "port", 
          po::value<std::uint16_t>()
            ->default_value(CONFIG_HPP_DEFAULT_PORT),
          "set listening port");
    po::variables_map arg_map;
    po::store(po::parse_command_line(argc, argv, desc), arg_map);
    po::notify(arg_map);
    if (arg_map.count("help")) {
      std::cout << desc << "\n";
    }
    return std::move(arg_map);
  }
} //namespace