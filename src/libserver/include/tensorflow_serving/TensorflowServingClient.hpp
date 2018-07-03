#ifndef TF_SERVING_CLIENT_H
#define TF_SERVING_CLIENT_H

#include <grpc++/create_channel.h>
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "tensorflow_serving/apis/prediction_service.grpc.pb.h"
#include <jsonparse/JsonParse.hpp>

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using tensorflow::serving::PredictRequest;
using tensorflow::serving::PredictResponse;
using tensorflow::serving::PredictionService;


namespace tensorflow_serving {
	typedef google::protobuf::Map<tensorflow::string, tensorflow::TensorProto> OutMap;

	class TensorflowServingClient {

		public:
			TensorflowServingClient(std::shared_ptr<Channel> channel): stub_(PredictionService::NewStub(channel)) {}

			std::shared_ptr<json::PredictionOutput> callPredict(const tensorflow::string& model_name,
																const tensorflow::string& model_signature_name,
																const tensorflow::Tensor& input,
																const tensorflow::string& input_name);

		private:
			std::unique_ptr<PredictionService::Stub> stub_;
	};
} // TF_SERVING_CLIENT_H
#endif
