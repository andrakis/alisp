// timekeeping.hpp
//
// Defines method for measuring time.
//

#pragma once

#include <chrono>
#include <tuple>
#include "acore.hpp"

namespace ALisp {
	namespace Timekeeping {
		typedef typename std::chrono::high_resolution_clock ClockType;
		typedef typename std::chrono::milliseconds ResolutionType;
		typedef long TimeType;
		template<typename T = ResolutionType> StringType time_type() { return "unknown"; }
		template<> StringType time_type<std::chrono::microseconds>() { return "us"; }
		template<> StringType time_type<std::chrono::milliseconds>() { return "ms"; }
		template<> StringType time_type<std::chrono::nanoseconds>() { return "ns"; }
		template<> StringType time_type<std::chrono::seconds>() { return "s"; }

		template<
			typename Result,
			class Callback,
			typename Resolution = ResolutionType,
			typename Time = TimeType>
		std::tuple<Time, Result> timeCallbackResult(Callback callback) {
			auto start = ClockType::now();
			Result result = callback();
			auto finish = ClockType::now();
			auto duration = std::chrono::duration_cast<Resolution>(finish - start);
			return std::make_tuple((Time)duration.count(), result);
		}

		template<
			class Callback,
			typename Resolution = ResolutionType,
			typename Time = TimeType>
		Time timeCall(Callback callback) {
			auto start = ClockType::now();
			callback();
			auto finish = ClockType::now();
			auto duration = std::chrono::duration_cast<Resolution>(finish - start);
			return (Time)duration.count();
		}

	}
}

