#ifndef XGBOOST_SERVING_CLIENT_H
#define XGBOOST_SERVING_CLIENT_H

#include <assert.h>
#include <util/to_string.h>
#include <jsonparse/JsonParse.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>

#include <iostream>
#include <fstream>
#include <xgboostcxx/XGBooster.hpp>
#include <xgboostcxx/XGBoosterImpl.hpp>
#include <boost/lexical_cast.hpp>

namespace xgboost {

	class XgBoostServingClient {

		public:
			XgBoostServingClient() {}

			template <typename T>
			void load_model(const std::string& filename, T& object);

			std::shared_ptr<json::PredictionOutput> callPredict(const std::string& model_name,
																const std::string& model_signature_name,
																const std::shared_ptr<std::vector<float>> input);

		private:

			template <typename T>
			void load_cpb(const std::string& filename, T& object);

			template <typename T>
			void load_cpb(std::istream& is, T& object);
	};

	template <typename T>
	void XgBoostServingClient::load_cpb(const std::string& filename, T& object) {
		std::ifstream ifs(filename, std::ios::binary);
		assert(ifs);
		load_cpb(ifs, object);
	}

	template <typename T>
	void XgBoostServingClient::load_cpb(std::istream& is, T& object) {
		cereal::PortableBinaryInputArchive ia(is);
		ia >> object;
	}

	template <typename T>
	void XgBoostServingClient::load_model(const std::string& filename, T& object) {
		load_cpb(filename, object);
	}
} // XGBOOST_SERVING_CLIENT_H
#endif
