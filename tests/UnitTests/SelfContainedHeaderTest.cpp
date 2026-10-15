// The library header is deliberately the first and only include: this test checks
// that it compiles on its own, with every standard header it uses included itself.
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
	const std::vector<std::string> names = probe.GetInheritanceChainNames();
	fastrttitests::Probe* castResult = fastrtti::dyna_cast<fastrttitests::Probe>(&probe);

	// Assert: compiling was the real check. The casts are here so every part of the
	// header is instantiated.
	const bool hasPassed = castResult == &probe && names.size() <= 1;
	return hasPassed ? 0 : 1;
}
