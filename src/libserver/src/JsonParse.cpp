#include <jsonparse/JsonParse.hpp>

namespace json {
	std::string PredictionConverter::get_inputs_type(const SharedPoolPtr<PredictionInput> &input) {
		std::string inputs = input->get();
		auto json_parsed = json_parser::parse(inputs);
		if (json_parsed["input_type"] != NULL) {
			return json_parsed["input_type"];
		} else {
			throw "input_type is not specified";
		}
	}

	std::string PredictionConverter::get_model_name(const SharedPoolPtr<PredictionInput> &input) {
		std::cout << "hit1" << std::endl;
		auto json_parsed = json_parser::parse(input->get());
		if (json_parsed["model_name"] != NULL) {
			return json_parsed["model_name"];
		} else {
			std::cout << "hit8" << std::endl;
			throw "model_name is not specified";
		}
	}

	std::string PredictionConverter::get_model_signature_name(const SharedPoolPtr<PredictionInput> &input) {
		auto json_parsed = json_parser::parse(input->get());
		if (json_parsed["model_signature_name"] != NULL) {
			return json_parsed["model_signature_name"];
		} else {
			throw "model_signature_name is not specified";
		}
	}
}
