#include <http/ServerHandler.hpp>

#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseBuilder.h>

#include <http/ServerStats.hpp>
#include <jsonparse/JsonParse.hpp>

using namespace proxygen;

namespace ServerService {

	ServerHandler::ServerHandler(ServerStats* stats): stats_(stats),
			tf_serving_client_(grpc::CreateChannel("localhost:9000", grpc::InsecureChannelCredentials())),
			http_method_("") {
	}

	void ServerHandler::onRequest(std::unique_ptr<HTTPMessage> message) noexcept {
		http_method_ = message->getMethodString();
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
		try {
			if (http_method_.compare("POST") == 0) {
				const char* data = reinterpret_cast<const char*>(body_->data());
				json::PredictionInput prediction_input(std::string(data, body_->length()));

				std::string input_type =
						prediction_converter_.get_inputs_type(std::make_shared<json::PredictionInput>(prediction_input));

				std::string model_name =
						prediction_converter_.get_model_name(std::make_shared<json::PredictionInput>(prediction_input));
				std::string model_signature_name =
						prediction_converter_.get_model_signature_name(std::make_shared<json::PredictionInput>(prediction_input));


				std::map<std::string, int> input_types = json::PredictionConverter::get_input_types();
				switch(input_types.find(input_type)->second) {

					case 1:
					{
						auto inputs_ints = prediction_converter_.get_input_data<int>(std::make_shared<json::PredictionInput>(prediction_input));
						std::vector<int> input_intv = inputs_ints->get();
						tensorflow::Tensor inp_int(tensorflow::DT_INT32, tensorflow::TensorShape({input_intv.size()}));
						auto inp_int_mapped = inp_int.tensor<int, 1>();
						for (int count = 0; count < input_intv.size(); count++) {
							inp_int_mapped(count) = input_intv[count];
						}

						std::shared_ptr<json::PredictionOutput> output_int =
								tf_serving_client_.callPredict(model_name, model_signature_name, inp_int, "X");

						this->sendResponse(folly::IOBuf::copyBuffer(output_int->get()));
						break;
					}

					case 2:
					{
						auto inputs_floats = prediction_converter_.get_input_data<float>(std::make_shared<json::PredictionInput>(prediction_input));
						std::vector<float> input_floatv = inputs_floats->get();
						tensorflow::Tensor inp_float(tensorflow::DT_FLOAT, tensorflow::TensorShape({input_floatv.size()}));
						auto inp_float_mapped = inp_float.tensor<float, 1>();
						for (int count = 0; count < input_floatv.size(); count++) {
							inp_float_mapped(count) = input_floatv[count];
						}

						std::shared_ptr<json::PredictionOutput> output_float =
								tf_serving_client_.callPredict(model_name, model_signature_name, inp_float, "X");
						this->sendResponse(folly::IOBuf::copyBuffer(output_float->get()));
						break;
					}

					case 3:
					{
						auto inputs_doubles = prediction_converter_.get_input_data<double>(std::make_shared<json::PredictionInput>(prediction_input));
						std::vector<double> input_doublev = inputs_doubles->get();
						tensorflow::Tensor inp_double(tensorflow::DT_DOUBLE, tensorflow::TensorShape({input_doublev.size()}));
						auto inp_double_mapped = inp_double.tensor<double, 1>();
						for (int count = 0; count < input_doublev.size(); count++) {
							inp_double_mapped(count) = input_doublev[count];
						}

						std::shared_ptr<json::PredictionOutput> output_double =
								tf_serving_client_.callPredict(model_name, model_signature_name, inp_double, "X");
						this->sendResponse(folly::IOBuf::copyBuffer(output_double->get()));
						break;
					}

					case 4:
					{
						auto inputs_strings = prediction_converter_.get_input_data<std::string>(std::make_shared<json::PredictionInput>(prediction_input));
						std::vector<std::string> input_stringv = inputs_strings->get();
						tensorflow::Tensor inp_string(tensorflow::DT_STRING, tensorflow::TensorShape({input_stringv.size()}));
						auto inp_string_mapped = inp_string.tensor<std::string, 1>();
						for (int count = 0; count < input_stringv.size(); count++) {
							inp_string_mapped(count) = input_stringv[count];
						}

						std::shared_ptr<json::PredictionOutput> output_string =
								tf_serving_client_.callPredict(model_name, model_signature_name, inp_string, "X");
						this->sendResponse(folly::IOBuf::copyBuffer(output_string->get()));
						break;
					}

					default:
					{
						throw std::string(input_type + " is not supported, the supported types are: ints, floats, doubles, strings");
					}
				}
			} else {
				this->sendResponse(folly::IOBuf::copyBuffer(http_method_ + " is not supported"));
			}
		} catch (std::string& e) {
			this->sendError(folly::IOBuf::copyBuffer(e));
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

	void ServerHandler::sendError(std::unique_ptr<folly::IOBuf> response_body) {
		ResponseBuilder(downstream_)
		  .status(400, "BAD Request")
		  .header("Request-Number",
				  folly::to<std::string>(stats_->getRequestCount()))
		  .body(std::move(response_body))
		  .sendWithEOM();
	}
}
