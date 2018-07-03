#include <tensorflow_serving/TensorflowServingClient.hpp>

namespace tensorflow_serving {

	std::shared_ptr<json::PredictionOutput> TensorflowServingClient::callPredict(const tensorflow::string& model_name,
																				 const tensorflow::string& model_signature_name,
																				 const tensorflow::Tensor& input,
																				 const tensorflow::string& input_name) {
		PredictRequest predict_request;
		PredictResponse response;
		ClientContext context;

		predict_request.mutable_model_spec()->set_name(model_name);
		predict_request.mutable_model_spec()->set_signature_name(model_signature_name);

		google::protobuf::Map<tensorflow::string, tensorflow::TensorProto>& inputs =
				*predict_request.mutable_inputs();

		tensorflow::TensorProto proto;

		input.AsProtoField(&proto);

		inputs[input_name] = proto;

		Status status = stub_->Predict(&context, predict_request, &response);

		if (status.ok()) {
			OutMap& map_outputs = *response.mutable_outputs();
			OutMap::iterator iter;
			int output_index = 0;

			std::string results;
			for (iter = map_outputs.begin(); iter != map_outputs.end(); iter++) {
				tensorflow::TensorProto& result_tensor_proto = iter->second;
				tensorflow::Tensor tensor;

				bool converted = tensor.FromProto(result_tensor_proto);
				if (converted) {
					results = results + tensor.SummarizeValue(10) + ", ";
				}
				++output_index;
			}

			json::PredictionOutput output(results);
			return std::make_shared<json::PredictionOutput>(output);
		} else {
			json::PredictionOutput output("gRPC failed.");
			return std::make_shared<json::PredictionOutput>(output);
		}
	}
}
