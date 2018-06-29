#include <http/ServerHandler.hpp>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <http/ServerStats.hpp>

using namespace proxygen;

namespace ServerService {

	ServerHandler::ServerHandler(ServerStats* stats): stats_(stats) {

	}

	void ServerHandler::onRequest(std::unique_ptr<HTTPMessage>) noexcept {
		stats_->recordRequest();
	}

	void ServerHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
		if (body_) {
			body_->prependChain(std::move(body));
		} else {
			body_ = std::move(body);
		}
	}

	void ServerHandler::onEOM() noexcept {
		ResponseBuilder(downstream_)
				.status(200, "OK")
				.header("Request-Number",
						folly::to<std::string>(stats_->getRequestCount()))
				.body(std::move(body_))
				.sendWithEOM();
	}

	void ServerHandler::onUpgrade(UpgradeProtocol /*protocol*/) noexcept {
	  // handler doesn't support upgrades
	}

	void ServerHandler::requestComplete() noexcept {
	  delete this;
	}

	void ServerHandler::onError(ProxygenError /*err*/) noexcept { delete this; }
}
