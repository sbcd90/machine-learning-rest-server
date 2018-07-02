#include <http/ServerHandler.hpp>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <http/ServerStats.hpp>
#include <jsonparse/JsonParse.hpp>

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
		const char* data = reinterpret_cast<const char*>(body_->data());
		json::PredictionInput prediction_input(std::string(data, body_->length()));

		std::string input_type =
				prediction_converter_.get_inputs_type(std::make_shared<json::PredictionInput>(prediction_input));

		std::map<std::string, int> input_types = json::PredictionConverter::get_input_types();
//		auto inputs;
//		std::ostringstream oss;
		switch(input_types.find(input_type)->second) {

			case 1:
			{
				auto inputs_ints = prediction_converter_.get_input_data<int>(std::make_shared<json::PredictionInput>(prediction_input));
				std::vector<int> a1 = inputs_ints->get();
				std::ostringstream oss1;
				std::copy(a1.begin(), a1.end(), std::ostream_iterator<int>(oss1, ", "));
				this->sendResponse(folly::IOBuf::copyBuffer(oss1.str()));
				break;
			}

			case 2:
			{
				auto inputs_floats = prediction_converter_.get_input_data<float>(std::make_shared<json::PredictionInput>(prediction_input));
				std::vector<float> a2 = inputs_floats->get();
				std::ostringstream oss2;
				std::copy(a2.begin(), a2.end(), std::ostream_iterator<float>(oss2, ", "));
				this->sendResponse(folly::IOBuf::copyBuffer(oss2.str()));
				break;
			}

			case 3:
			{
				auto inputs_doubles = prediction_converter_.get_input_data<double>(std::make_shared<json::PredictionInput>(prediction_input));
				std::vector<double> a4 = inputs_doubles->get();
				std::ostringstream oss3;
				std::copy(a4.begin(), a4.end(), std::ostream_iterator<double>(oss3, ", "));
				this->sendResponse(folly::IOBuf::copyBuffer(oss3.str()));
				break;
			}

			case 4:
			{
				auto inputs_strings = prediction_converter_.get_input_data<std::string>(std::make_shared<json::PredictionInput>(prediction_input));
				std::vector<std::string> a5 = inputs_strings->get();
				std::ostringstream oss4;
				std::copy(a5.begin(), a5.end(), std::ostream_iterator<std::string>(oss4, ", "));
				this->sendResponse(folly::IOBuf::copyBuffer(oss4.str()));
				break;
			}

			default:
			{
				throw input_type + " is not supported, the supported types are: ints, floats, doubles, strings";
			}
		}
	}

	void ServerHandler::onUpgrade(UpgradeProtocol /*protocol*/) noexcept {
	  // handler doesn't support upgrades
	}

	void ServerHandler::requestComplete() noexcept {
	  delete this;
	}

	void ServerHandler::onError(ProxygenError /*err*/) noexcept { delete this; }

	void ServerHandler::sendResponse(std::unique_ptr<folly::IOBuf> response_body) {
	  ResponseBuilder(downstream_)
		.status(200, "OK")
		.header("Request-Number",
				folly::to<std::string>(stats_->getRequestCount()))
		.body(std::move(response_body))
		.sendWithEOM();
	}
}
