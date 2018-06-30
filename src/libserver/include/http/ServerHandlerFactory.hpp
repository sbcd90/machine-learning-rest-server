#include <folly/Memory.h>
#include <folly/io/async/EventBaseManager.h>
#include <folly/portability/GFlags.h>
#include <folly/portability/Unistd.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>

#include <http/ServerStats.hpp>
#include <http/ServerHandler.hpp>

using namespace ServerService;
using namespace proxygen;

using folly::EventBase;
using folly::EventBaseManager;
using folly::SocketAddress;

class ServerHandlerFactory: public RequestHandlerFactory {

	public:
		void onServerStart(folly::EventBase*) noexcept override {
			stats_.reset(new ServerStats);
		}

		void onServerStop() noexcept override {
			stats_.reset();
		}

		RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override {
			return new ServerHandler(stats_.get());
		}

	private:
		folly::ThreadLocalPtr<ServerStats> stats_;
};
