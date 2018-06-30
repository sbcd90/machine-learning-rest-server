#include <jsonparse/JsonParse.hpp>

namespace json {
	std::string PredictionConverter::get_inputs_type(const SharedPoolPtr<PredictionInput> &input) {
		std::string inputs = input->get();
		auto json_parsed = json_parser::parse(inputs);
		return json_parsed["input_type"];
	}
}
