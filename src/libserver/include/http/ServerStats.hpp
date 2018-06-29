#ifndef TF_SERVING_REST_STATS_H
#define TF_SERVING_REST_STATS_H

#pragma once

namespace ServerService {

	class ServerStats {
		public:

			virtual ~ServerStats() {}

			virtual void recordRequest() {
				++reqCount_;
			}

			virtual uint64_t getRequestCount() {
				return reqCount_;
			}

		private:
			uint64_t reqCount_{0};
	};
}
