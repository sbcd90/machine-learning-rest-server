#include <jsonparse/jsonparse.hpp>
#include <iostream>

int main() {
	json::PredictionConverter predictionConverter;
	std::string s = "{\"inputs\": [1, 2, 4]}";
	json::PredictionInput predictionInput(s);
	auto a = predictionConverter.get_input_data<int>(std::make_shared<json::PredictionInput>(predictionInput));

	std::vector<int> final_data = a->get();
	for (int i=0; i < final_data.size(); i++) {
		std::cout << final_data[i] << std::endl;
	}

}

