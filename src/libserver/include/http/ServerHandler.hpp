#ifndef TF_SERVING_REST_SERVER_H
#define TF_SERVING_REST_SERVER_H

#pragma once

#include <folly/Memory.h>
#include <proxygen/httpserver/RequestServer.h>

namespace proxygen {
	class ResponseHandler;
}

namespace ServerService {

	class ServerStats;

	class ServerHandler: public proxygen::RequestHandler {
		public:
			explicit ServerHandler(ServerStats* stats);

			void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
				noexcept override;

			void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;

			void onEOM() noexcept override;

			void onUpgrade(proxygen::UpgradeProtocol proto) noexcept override;

			void requestComplete() noexcept override;

			void onError(proxygen::ProxygenError err) noexcept override;

		private:
			ServerStats* const stats_{nullptr};

			std::unique_ptr<folly::IOBuf> body_;
	}
} // TF_SERVING_REST_SERVER_H
#endif
