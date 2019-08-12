#include "acore.hpp"
#include "cell.hpp"

#include "atoms.hpp"

namespace ALisp {
	Cell Nil(AtomCell(0));
	Cell True(AtomCell(1));
	Cell False(AtomCell(2));

	Cell Cell::head() const EXCEPT {
		if (pImpl) return pImpl->head();
		return Nil;
	}
	Cell Cell::tail() const EXCEPT {
		if (pImpl) return pImpl->tail();
		return Nil;
	}
	bool Cell::empty() const EXCEPT {
		if (pImpl) return pImpl->empty();
		return true;
	}
	size_t Cell::size() const EXCEPT {
		if (pImpl) return pImpl->size();
		return 0;
	}
	Cell Cell::index(size_t i) const EXCEPT {
		if (pImpl) return pImpl->index(i);
		return Nil;
	}
	
	// Lambda/Macro functions
	Cell Cell::lambda_ptr() const EXCEPT {
		if (pImpl) return pImpl->lambda_ptr();
		return Nil;
	}
	void Cell::lambda_ptr(const Cell &ptr) EXCEPT {
		if (pImpl) pImpl->lambda_ptr(ptr);
	}
	Cell Cell::lambda_args() const EXCEPT {
		if (pImpl) return pImpl->lambda_args();
		return Nil;
	}
	void Cell::lambda_args(const Cell &args) EXCEPT {
		if (pImpl) pImpl->lambda_args(args);
	}
	Cell Cell::lambda_body() const EXCEPT {
		if (pImpl) return pImpl->lambda_body();
		return Nil;
	}
	void Cell::lambda_body(const Cell &body) EXCEPT {
		if (pImpl) pImpl->lambda_body(body);
	}

	std::ostream &operator<<(std::ostream &os, const Cell &cell) {
		os << cell.str();
		return os;
	}
}
