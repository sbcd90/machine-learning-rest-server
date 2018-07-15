#include <xgboostcxx/XGBoostServingClient.hpp>

namespace xgboost {

	std::shared_ptr<json::PredictionOutput> XgBoostServingClient::callPredict(const std::string& model_name,
																			  const std::string& model_signature_name,
																			  const std::shared_ptr<std::vector<float>> input) {
		xgboostcpp::XGBooster xgb;

		load_model(model_name, xgb);

		float output = xgb(*input.get());

		json::PredictionOutput prediction_output(std::to_string(output));
		return std::make_shared<json::PredictionOutput>(prediction_output);
	}
}
