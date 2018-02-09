#include "stdafx.h"
#include "microtween.h"
#include "plotter.h"

void test(const char* filename, microtween::easing easing)
{
	const int w = 100;
	const int h = 100;

	microtween m;
	m.reset(0).to(w, h, easing);

	plotter p(w + 1, h + 1, 0xffffff);

	for (int x = 0; x < w + 1; ++x)
		p.set(x, m.geti(x), 0);

	p.write(filename);
}

int main(const int /*argc*/, char* /*argv[]*/)
{
	microtween m;
	m.reset(1).to(3, 2, microtween::easing::linear).to(5, 2, microtween::easing::linear);

	for (int i = 0; i < 6; ++i)
		std::cout << m.get(i) << std::endl;

	test("linear.png", microtween::easing::linear);
	test("circular_in.png", microtween::easing::circular_in);

    return 0;
}
