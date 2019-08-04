// environment.hpp
//
// Defines the ALisp Environment.
//
// Design considerations:
//  - Insertion speed is highly relevant; items will constantly be inserted or
//    updated into environments.
//  - Lookups will be very random; no point ordering keys.
//  ** Use unordered_map due to above
#pragma once

#include <memory>
#include <unordered_map>

namespace ALisp {
	class Environment {
	public:
		typedef AtomType key_type;
		typedef Cell val_type;
		typedef std::unordered_map<AtomType, Cell> map_type;
		typedef EnvironmentType outer_type;

	private:
		map_type _map;
		outer_type _outer;
	protected:
	public:
		Environment(outer_type parent = nullptr) : _map(), _outer(parent) {}

		bool has_parent() const { return _outer != nullptr; }

		void create(key_type key, val_type val) {
			// Note function signature: val is copied not referenced
			_map[key] = val;
		}
		bool has_key(key_type key) const {
			return _map.find(key) != _map.end();
		}
		bool has_key_any(key_type key) const {
			if (has_key(key)) return true;
			if (has_parent()) return _outer->has_key_any(key);
			return false;
		}

		void set(key_type key, val_type val) EXCEPT {
			// Note function signature: val is copied not referenced
			if (has_key(key)) create(key, val);
			else if (has_parent()) _outer->set(key, val);
			throw new Exception("key not found");
		}
	};
}
