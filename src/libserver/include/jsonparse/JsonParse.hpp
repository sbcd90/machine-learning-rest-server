#ifndef TF_SERVING_REST_LIBSERVER_H
#define TF_SERVING_REST_LIBSERVER_H

#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>

namespace json {

	template <typename T>
	using SharedPoolPtr = std::shared_ptr<T>;

	using json_parser = nlohmann::json;

	class PredictionInput {
		public:

			PredictionInput(std::string x_input): x_input(x_input) {}

			~PredictionInput() {}

			std::string get() const { return x_input; }

		private:
			std::string x_input;
	};

	class PredictionOutput {
		public:

			PredictionOutput(std::string y_output): y_output(y_output) {}

			~PredictionOutput() {}

			std::string get() const {
				json_parser j_output;
				j_output["y_output"] = y_output;
				return j_output.dump();
			}

		private:
			std::string y_output;
	};

	template <typename D>
	class PredictionData {
		public:

			PredictionData(std::vector<D> x_transformed): x_transformed(x_transformed) {}

			~PredictionData() {}

			std::vector<D> get() const { return x_transformed; }

		private:
			std::vector<D> x_transformed;
	};

	class PredictionConverter {
		public:
			template <typename D>
			SharedPoolPtr<PredictionData<D>> get_input_data(const SharedPoolPtr<PredictionInput> &input);

			template <typename D>
			SharedPoolPtr<PredictionOutput> get_output_data(const SharedPoolPtr<PredictionData<D>> &output);

			std::string get_inputs_type(const SharedPoolPtr<PredictionInput> &input);

			std::string get_model_name(const SharedPoolPtr<PredictionInput> &input);

			std::string get_model_signature_name(const SharedPoolPtr<PredictionInput> &input);

			static std::map<std::string, int> get_input_types() {
				std::map<std::string, int> input_types;

				input_types["ints"] = 1;
				input_types["floats"] = 2;
				input_types["doubles"] = 3;
				input_types["strings"] = 4;

				return input_types;
			}
	};

	template <typename D>
	SharedPoolPtr<PredictionData<D>> PredictionConverter::get_input_data(const SharedPoolPtr<PredictionInput> &input) {
		std::string inputs = input->get();
		auto json_parsed = json_parser::parse(inputs);

		std::vector<D> data;
		for (auto const& input: json_parsed["inputs"]) {
			data.push_back(input);
		}

		PredictionData<D> prediction_data(data);
		return std::make_shared<PredictionData<D>>(prediction_data);
	}

	template <typename D>
	SharedPoolPtr<PredictionOutput> PredictionConverter::get_output_data(const SharedPoolPtr<PredictionData<D>> &output) {
		std::string s = "hello world";
		return SharedPoolPtr<std::string>(&s);
	}

}
#endif // TF_SERVING_REST_LIBSERVER_H
