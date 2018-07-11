#ifndef __xgboostcpp_XGBooster_h__
#define __xgboostcpp_XGBooster_h__

#include <util/xgboostcpp_def.h>
#include <boost/serialization/export.hpp>
#include <vector>
#include <memory>

template <typename T>
using MatrixType = std::vector<std::vector<T>>;

XGBOOSTCPP_BEGIN_NAMESPACE(xgboostcpp)

class XGBooster {

	public:

		struct Recipe {
			int numberOfTrees = 512;
			int maxDepth = 5;
			double dataSubsample = 0.5;
			double learningRate = 0.1;
			double featureSubsample = 0.1;
			bool regression = true;

			template <class Archive> void serialize(Archive& ar, const unsigned int version);
		};

		class Impl;
		XGBooster();
		XGBooster(const Recipe& recipe);
		float operator()(const std::vector<float> &features);

		void read(const std::string& filename);
		void write(const std::string& filename) const;

		// Boost serialization:
		template<class Archive> void serialize(Archive & ar, const unsigned int version);

	protected:
		std::shared_ptr<Impl> m_impl;
};

XGBOOSTCPP_END_NAMESPACE(xgboostcpp)

BOOST_CLASS_EXPORT_KEY(xgboostcpp::XGBooster);
BOOST_CLASS_EXPORT_KEY(xgboostcpp::XGBooster::Impl);

#endif

