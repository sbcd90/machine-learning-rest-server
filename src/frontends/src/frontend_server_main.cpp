#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/GFlags.h>
#include <folly/portability/Unistd.h>
#include <proxygen/httpserver/HTTPServer.h>

#include <http/ServerHandler.hpp>
#include <http/ServerStats.hpp>
#include <http/ServerHandlerFactory.hpp>

using namespace ServerService;
using namespace proxygen;

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

using Protocol = HTTPServer::Protocol;

DEFINE_int32(http_port, 11000, "Port to listen on with HTTP protocol");
DEFINE_int32(spdy_port, 11001, "Port to listen on with SPDY protocol");
DEFINE_int32(h2_port, 11002, "Port to listen on with HTTP/2 protocol");
DEFINE_string(ip, "localhost", "IP/Hostname to bind to");
DEFINE_int32(threads, 0, "Number of threads to listen on. Numbers <= 0 "
             "will use the number of cores on this machine.");

int main(int argc, char* argv[]) {
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	google::InitGoogleLogging(argv[0]);
	google::InstallFailureSignalHandler();

	std::vector<HTTPServer::IPConfig> IPs = {
			{SocketAddress(FLAGS_ip, FLAGS_http_port, true), Protocol::HTTP},
			{SocketAddress(FLAGS_ip, FLAGS_spdy_port, true), Protocol::SPDY},
			{SocketAddress(FLAGS_ip, FLAGS_h2_port, true), Protocol::HTTP2}
	};

	if (FLAGS_threads <= 0) {
		FLAGS_threads = sysconf(_SC_NPROCESSORS_ONLN);
		CHECK(FLAGS_threads > 0);
	}

	HTTPServerOptions options;
	options.threads = static_cast<size_t>(FLAGS_threads);
	options.idleTimeout = std::chrono::milliseconds(60000);
	options.shutdownOn = {SIGINT, SIGTERM};
	options.enableContentCompression = false;
	options.handlerFactories = RequestHandlerChain()
			.addThen<ServerHandlerFactory>()
			.build();
	options.h2cEnabled = true;

	HTTPServer server(std::move(options));
	server.bind(IPs);

	// Start HTTPServer mainloop in a separate thread
	std::thread t([&] () {
		server.start();
	});

	t.join();
	return 0;
}
