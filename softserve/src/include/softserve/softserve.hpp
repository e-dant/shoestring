#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <optional>
#include <thread>
#include <iostream>
#include <fstream>

//class Softserve {
//public:
//  Softserve();
//  ~Softserve();
//private:
//
////auto getContent() {
////    std::ifstream f("file.txt");
////
////    if (f.is_open())
////        std::cout << f.rdbuf();
////}
//   
//}; //class

//namespace Softserve {
//  namespace ip = boost::asio::ip;
//} //namespace