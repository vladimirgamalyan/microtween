#pragma once
#include <vector>
#include <cmath>

class microtween
{
public:

	enum class easing {
		linear,
		circular_in
	};

	microtween& reset(float v)
	{
		sequence.clear();
		from_value = v;
		cursor = 0;
		return *this;
	}

	microtween& reset(int v)
	{
		return reset(static_cast<float>(v));
	}
	
	microtween& to(float end, int d, easing e = microtween::easing::linear)
	{
		sequence.emplace_back(end, d, e);
		return *this;
	}

	microtween& to(int end, int d, easing e = microtween::easing::linear)
	{
		return to(static_cast<float>(end), d, e);
	}

	void step(int s = 1)
	{
		cursor += s;
	}

	float get() const
	{
		get(cursor);
	}

	float get(int c) const
	{
		float start = from_value;
		for (const auto& i : sequence)
		{
			if (c < i.duration)
				return interpolate(static_cast<float>(c) / i.duration, start, i.end, i.easing);
			c -= i.duration;
			start = i.end;
		}

		return sequence.back().end;
	}

	int geti(int c) const
	{
		return static_cast<int>(roundf(get(c)));
	}

	int geti() const
	{
		return geti(cursor);
	}

private:
	struct tween_point
	{
		tween_point(float end, int duration, easing easing) : end(end), duration(duration), easing(easing) {}
		float end;
		int duration;
		easing easing;
	};
	float from_value;
	int cursor;
	std::vector<tween_point> sequence;
	float interpolate(float position, float start, float end, easing e) const
	{
		float d = end - start;
		switch (e)
		{
		case easing::circular_in:
			return -d * (sqrtf(1 - position * position) - 1) + start;
		}
		return d * position + start;
	}
};
