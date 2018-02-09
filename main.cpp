#include "stdafx.h"
#include "microtween.h"

int main(const int /*argc*/, char* /*argv[]*/)
{
	microtween m;
	m.reset(1).to(3, 2, microtween::easing::linear).to(5, 2, microtween::easing::linear);

	for (int i = 0; i < 6; ++i)
		std::cout << m.get(i) << std::endl;

    return 0;
}
