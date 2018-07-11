#include <xgboostcxx/XGBoostServingClient.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

namespace xgboost {

	std::shared_ptr<json::PredictionOutput> XgBoostServingClient::callPredict(const std::string& model_name,
																			const std::string& model_signature_name,
																			const std::string& input) {
		std::ifstream is(input);
		boost::spirit::istream_iterator first(is), last;
		std::vector<std::vector<float>> input_values;

		bool r = boost::spirit::qi::phrase_parse(first, last, +(qi::float_ % ','), qi::space, input_values);

		if (r) {
			xgboostcpp::XGBooster xgb;

			load_model(model_name, xgb);

			std::vector<float> output(input_values.size());
			for (int idx = 0; idx < input_values.size(); idx++) {
				output[idx] = xgb(input_values[idx]);
			}

			std::ostringstream os;
			std::copy(output.begin(), output.end(), std::ostream_iterator<float>(os, "\n"));

			json::PredictionOutput prediction_output(os.str());
			return std::make_shared<json::PredictionOutput>(prediction_output);
		}

		return {};
	}
}
