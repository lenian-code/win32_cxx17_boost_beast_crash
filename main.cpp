#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>

/*
In Win32 Release config this will crash"
Visual Studio 15.9.24
Win32
Release
Optimizations /O2 or /Ox
C++17
Boost 1.73.0
*/
class client {
public:
	client(boost::asio::io_context &ioc) :_iocontext(ioc), _resolver(ioc), _stream(ioc) {}
	bool connect(const std::string host, const std::string &port) {
		_host = host;
		_port = port;

		auto const results = _resolver.resolve(_host, _port);

		boost::beast::error_code ec;
		_stream.connect(results, ec);
		return !ec;
	}
	void disconnect() {
		boost::beast::error_code ec;
		_stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
	}
	bool ping() {
		//auto resp = http_head_request("/");			// <- just a '/' causes an exception
		auto resp = http_head_request("/ping");			// <- triggers a breakpoint/crashes
		return resp == boost::beast::http::status::no_content;
	}

protected:
	boost::asio::io_context &_iocontext;
	boost::asio::ip::tcp::resolver _resolver;
	boost::beast::tcp_stream _stream;

	std::string _host;
	std::string _port;

	boost::beast::http::status http_head_request(const std::string &path) {
		boost::beast::http::request<boost::beast::http::empty_body> req(boost::beast::http::verb::head, path, 11);
		req.set(boost::beast::http::field::host, "localhost");
		req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		boost::beast::http::write(_stream, req);
		boost::beast::http::response_parser<boost::beast::http::empty_body> parser;
		parser.skip(true);
		boost::beast::flat_buffer buffer;
		boost::beast::http::read(_stream, buffer, parser);
		auto res = parser.release();
		return res.result();
	}
	boost::beast::http::status http_post_request(const std::string &path) {
		boost::beast::http::request<boost::beast::http::string_body> req(boost::beast::http::verb::post, path, 11);
		req.set(boost::beast::http::field::host, "localhost");
		req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		boost::beast::http::write(_stream, req);
		boost::beast::http::response_parser<boost::beast::http::dynamic_body> parser;
		parser.skip(true);
		boost::beast::flat_buffer buffer;
		boost::beast::http::read(_stream, buffer, parser);
		auto res = parser.release();
		return res.result();
	}
};

using namespace std;
int main(int argc, char *argv[])
{
	boost::asio::io_context ioc;

	client c(ioc);
	c.connect("www.example.org", "80");

	try {
		c.ping();
	}
	catch (std::exception &e) {
		cout << "Exception " << e.what() << endl;
	}
	catch (...) {
		cout << "not an std::exception...but at least something got cought " << endl;
	}
	cout << "In Win32 Rlease this will not be reached." << endl;

	c.disconnect();

	return 0;
}