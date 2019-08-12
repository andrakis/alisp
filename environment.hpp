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

		typedef typename map_type::const_iterator const_iterator;

	private:
		map_type _map;
		outer_type _outer;
	protected:
		void addRange(const ListType &keys, const ListType &values) {
			for (auto it1 = keys.cbegin(), it2 = values.cbegin();
				 it1 != keys.cend() && it2 != values.cend();
				 ++it1, ++it2) {
				create(it1->atomId(), *it2);
			}
		}
	public:
		Environment(outer_type parent = nullptr) : _map(), _outer(parent) {}
		Environment(const Cell &keys, const ListType &values, outer_type parent = nullptr)
			: _map(), _outer(parent) {
			ListType lt_keys, lt_values;
			if (keys.type() == CellType::List) {
				// List of keys and values
				lt_keys.insert(lt_keys.end(), keys.cbegin(), keys.cend());
				lt_values.insert(lt_values.end(), values.cbegin(), values.cend());
			} else {
				// Single key capturing multiple values
				lt_keys.push_back(keys);
				lt_values.insert(lt_values.end(), values.cbegin(), values.cend());
			}
			addRange(lt_keys, lt_values);
		}

		bool has_parent() const { return _outer != nullptr; }

		void create(key_type key, val_type val) {
			// Note function signature: val is copied not referenced
			_map[key] = val;
		}
		bool has_key(key_type key) const {
			return _map.find(key) != _map.end();
		}
		const_iterator find_key(key_type key) const {
			return _map.find(key);
		}
		const_iterator cbegin() const { return _map.cbegin(); }
		const_iterator cend() const { return _map.cend(); }

		bool has_key_any(key_type key) const {
			if (has_key(key)) return true;
			if (has_parent()) return _outer->has_key_any(key);
			return false;
		}

		void set(key_type key, val_type val) EXCEPT {
			// Note function signature: val is copied not referenced
			if (has_key(key)) create(key, val);
			else if (has_parent()) _outer->set(key, val);
			throw Exception("key not found");
		}
		Cell get(key_type key) const EXCEPT {
			auto it = find_key(key);
			if (it != cend())
				return it->second;
			else if (has_parent())
				return _outer->get(key);
			throw Exception("key not found");
		}
	};
}
