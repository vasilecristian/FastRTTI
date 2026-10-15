// Configuration of the header under test, so it has to come before it is included.
// A project assert written as a statement, the usual shape of one. RTTI_ASSERT is
// documented as replaceable, so the header has to accept this form.
#include <cstdlib>
#define RTTI_ASSERT(condition) do { if (!(condition)) { std::abort(); } } while (0)

#include "FastRTTI/RTTI.hpp"

namespace fastrttitests
{
	class Probe : public fastrtti::IRTTI<Probe>
	{
	};
}

int main()
{
	// Arrange
	fastrttitests::Probe probe;

	// Act
	fastrttitests::Probe* castResult = fastrtti::dyna_cast<fastrttitests::Probe>(&probe);

	// Assert: compiling was the real check.
	return castResult == &probe ? 0 : 1;
}
