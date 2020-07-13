#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

using namespace std;
int main(int argc, char *argv[])
{
	boost::asio::io_context ioc;
	boost::asio::ip::tcp::resolver resolver(ioc);
	boost::beast::tcp_stream stream(ioc);

	auto const results = resolver.resolve("localhost", "8086");

	stream.connect(results);

	boost::beast::http::request<boost::beast::http::empty_body> req(boost::beast::http::verb::head, "/ping", 11);
	req.set(boost::beast::http::field::host, "localhost");
	req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

	boost::beast::http::write(stream, req);

	boost::beast::http::response_parser<boost::beast::http::empty_body> parser;
	parser.skip(true);

	boost::beast::flat_buffer buffer;
	boost::beast::http::read(stream, buffer, parser);

	auto res = parser.release();
	if (res.result() == boost::beast::http::status::no_content) {
		cout << "no content\n";
	}

	stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	return 0;
}