#include <softserve/softserve.hpp>
#include <softserve/config.hpp>

namespace ip = boost::asio::ip;
namespace http = boost::beast::http;

auto main(int argc, char* argv[]) -> int
try {
  auto arg_map = config::parse_args(argc, argv);

  if (arg_map.count("help")) {
    return 0;
  }

  auto address = ip::make_address(arg_map["address"].as<std::string>());
  auto port = arg_map["port"].as<std::uint16_t>();
  std::cout << "Listening on " << address << ":" << port << std::endl;

  /**********************/
  auto handle_session = [](ip::tcp::socket socket) {
    auto report_error = [](boost::beast::error_code ec, char const* what) {
      std::cerr << what << ": " << ec.message() << "\n";
    };

    boost::beast::error_code ec;
    for (;;) {
      boost::beast::flat_buffer buffer;

      /*********************
       * 
       *********************/
      // Read a request
      http::request<http::string_body> request;
      http::read(socket, buffer, request, ec);
      if (ec == http::error::end_of_stream) break;
      if (ec) return report_error(std::move(ec), "read");

      // Send the response
      // Respond to any request with a "Hello World" message.
      http::response<http::string_body>
      response {http::status::ok,
                request.version()};
      response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
      response.set(http::field::content_type, "text/plain");
      response.keep_alive(request.keep_alive());
      std::string greeting = "Hello ";
      auto const* target = std::getenv("TARGET");
      greeting += target == nullptr ? "World" : target;
      greeting += "\n";
      response.body() = std::move(greeting);
      response.prepare_payload();
      http::write(socket, response, ec);
      if (ec) return report_error(std::move(ec), "write");
    }
    socket.shutdown(ip::tcp::socket::shutdown_send, ec);
  };


  /** @var ioc
   * make this a private variable of server
   * or otherwise limit its scope.
   * these are only necessary for the `serve` 
   * function's loop.
   * 
   * Maybe make them static inside the `serve` fn
   */
  boost::asio::io_context ioc{/*concurrency_hint=*/1};
  /** @var acceptor
   * make this a private variable of server
   * or otherwise limit its scope.
   * these are only necessary for the `serve` 
   * function's loop.
   * 
   * Maybe make them static inside the `serve` fn
   */
  ip::tcp::acceptor acceptor{ioc, {address, port}};

  /** @function serve
   * one thread for each session:
   * creates a thread and a socket, transfers ownership 
   * of the socket to the thread, and detaches the thread.
   */
  for (;;) {
    auto socket = acceptor.accept(ioc);
    if (!socket.is_open()) break;
    std::thread{handle_session, std::move(socket)}.detach();
  }
  /**********************/

  return 0;
}
catch (std::exception const& ex) {
  std::cerr << "Standard exception caught " << ex.what() << '\n';
  return 1;
}