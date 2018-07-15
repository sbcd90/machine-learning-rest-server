#include <xgboostcxx/XGBoostServingClient.hpp>

int main() {

	xgboost::XgBoostServingClient client;
	std::vector<float> v = {1,2,3,4,5,6,7,8,9,10, 11,12,13,14,15,16,17,18,19,20};

	std::shared_ptr<json::PredictionOutput> output = client.callPredict("test.cpb", "xgboost", std::make_shared<std::vector<float>>(v));
	std::cout << output->get() << std::endl;

	return 0;
}
