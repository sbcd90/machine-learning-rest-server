#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <http/ServerHandler.hpp>
#include <http/ServerStats.hpp>

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
