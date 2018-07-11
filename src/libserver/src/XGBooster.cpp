#include <util/to_string.h>
#include <xgboostcxx/XGBooster.hpp>
#include <xgboostcxx/XGBoosterImpl.hpp>

XGBOOSTCPP_NAMESPACE_BEGIN

XGBooster::XGBooster() {
	m_impl = std::make_shared<XGBooster::Impl>();
}

XGBooster::XGBooster(const Recipe& recipe) {
	m_impl = std::make_shared<XGBooster::Impl>(recipe);
}

float XGBooster::operator ()(const std::vector<float>& features) {
	return (*m_impl)(features);
}

void XGBooster::read(const std::string& filename) {
	m_impl->read(filename);
}

void XGBooster::write(const std::string& filename) const {
	m_impl->write(filename);
}

XGBOOSTCPP_NAMESPACE_END
