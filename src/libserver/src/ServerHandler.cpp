#include <http/ServerHandler.hpp>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <http/ServerStats.hpp>
#include <jsonparse/JsonParse.hpp>

using namespace proxygen;

namespace ServerService {

	ServerHandler::ServerHandler(ServerStats* stats): stats_(stats), prediction_converter_() {

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
		const char* data = reinterpret_cast<const char*>(body_->data());
		json::PredictionInput prediction_input(std::string(data, body_->length()));

		std::string input_type =
				prediction_converter_.get_inputs_type(std::make_shared<json::PredictionInput>(prediction_input));

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
