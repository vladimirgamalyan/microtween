#include "stdafx.h"
#include "microtween.h"
#include "plotter.h"

void test(const char* filename, microtween::easing easing)
{
	const int w = 300;
	const int h = 300;
	const int border = w / 2;

	const std::uint32_t border_color = 0xafafaf;
	const std::uint32_t background_color = 0xffffff;
	const std::uint32_t pen_color = 0;

	std::cout << "generate " << filename << std::endl;

	microtween m;
	m.reset(0).to(w, h, easing);

	plotter p(w + 1 + border * 2, h + 1 + border * 2, background_color);

	
	for (int x = 0; x < w; ++x)
	{
		p.set(border + x, border, border_color);
		p.set(border + x, border + h, border_color);
	}

	for (int y = 0; y < h; ++y)
	{
		p.set(border, border + y, border_color);
		p.set(border + w, border + y, border_color);
	}
		
	for (int x = 0; x < w + 1; ++x)
		p.set(border + x, border + m.geti(x), pen_color);

	p.write(filename);
}

int main(const int /*argc*/, char* /*argv[]*/)
{
	microtween m;
	m.reset(1).to(3, 2, microtween::easing::linear).to(5, 2, microtween::easing::linear);

	for (int i = 0; i < 6; ++i)
		std::cout << m.get(i) << std::endl;

	test("output/linear.png", microtween::easing::linear);
	test("output/quadratic_in.png", microtween::easing::quadratic_in);
	test("output/quadratic_out.png", microtween::easing::quadratic_out);
	test("output/quadratic_in_out.png", microtween::easing::quadratic_in_out);
	test("output/cubic_in.png", microtween::easing::cubic_in);
	test("output/cubic_out.png", microtween::easing::cubic_out);
	test("output/cubic_in_out.png", microtween::easing::cubic_in_out);
	test("output/quartic_in.png", microtween::easing::quartic_in);
	test("output/quartic_out.png", microtween::easing::quartic_out);
	test("output/quartic_in_out.png", microtween::easing::quartic_in_out);
	test("output/exponential_in.png", microtween::easing::exponential_in);
	test("output/exponential_out.png", microtween::easing::exponential_out);
	test("output/exponential_in_out.png", microtween::easing::exponential_in_out);
	test("output/circular_in.png", microtween::easing::circular_in);
	test("output/circular_out.png", microtween::easing::circular_out);
	test("output/circular_in_out.png", microtween::easing::circular_in_out);
	test("output/elastic_in.png", microtween::easing::elastic_in);
	test("output/elastic_out.png", microtween::easing::elastic_out);
	test("output/elastic_in_out.png", microtween::easing::elastic_in_out);
	test("output/back_in.png", microtween::easing::back_in);
	test("output/back_out.png", microtween::easing::back_out);
	test("output/back_in_out.png", microtween::easing::back_in_out);

	return 0;
}
