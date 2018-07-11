#include <xgboostcxx/XGBoostServingClient.hpp>

int main() {

	xgboost::XgBoostServingClient client;
	std::shared_ptr<json::PredictionOutput> output = client.callPredict("test.cpb", "xgboost", "pima-indians-diabetes.csv");
	std::cout << output->get() << std::endl;

	return 0;
}
