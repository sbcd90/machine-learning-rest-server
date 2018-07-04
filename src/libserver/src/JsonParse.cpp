#include <jsonparse/JsonParse.hpp>

namespace json {
	std::string PredictionConverter::get_inputs_type(const SharedPoolPtr<PredictionInput> &input) {
		try {
			std::string inputs = input->get();
			auto json_parsed = json_parser::parse(inputs);
			return json_parsed["input_type"];
		} catch (json_parser::exception& e) {
			throw std::string("input_type is not specified");
		}
	}

	std::string PredictionConverter::get_model_name(const SharedPoolPtr<PredictionInput> &input) {
		try {
			auto json_parsed = json_parser::parse(input->get());
			return json_parsed["model_name"];
		} catch (json_parser::exception& e) {
			throw std::string("model_name is not specified");
		}
	}

	std::string PredictionConverter::get_model_signature_name(const SharedPoolPtr<PredictionInput> &input) {
		try {
			auto json_parsed = json_parser::parse(input->get());
			return json_parsed["model_signature_name"];
		} catch (json_parser::exception& e) {
			throw std::string("model_signature_name is not specified");
		}
	}
}
