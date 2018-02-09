#pragma once
#include <vector>

class microtween
{
public:

	enum class easing {
		linear,
		circularIn
	};

	microtween& reset(int v)
	{
		sequence.clear();
		from_value = v;
		cursor = 0;
		return *this;
	}
	
	microtween& to(int v, int d, easing e = microtween::easing::linear)
	{
		sequence.emplace_back(v, d, e);
		return *this;
	}

	void step(int s = 1)
	{
		cursor += s;
	}

	int get() const
	{
		get(cursor);
	}

	int get(int c) const
	{
		int s = from_value;
		for (const auto& i : sequence)
		{
			if (c < i.duration)
				return interpolate(static_cast<float>(c) / i.duration, s, i.to, i.easing);
			c -= i.duration;
			s = i.to;
		}

		return sequence.back().to;
	}

private:
	struct tween_point
	{
		tween_point(int to, int duration, easing easing) : to(to), duration(duration), easing(easing) {}
		int to;
		int duration;
		easing easing;
	};
	int from_value;
	int cursor;
	std::vector<tween_point> sequence;
	int interpolate(float position, int start, int end, easing e) const
	{
		int d = end - start;
		switch (e)
		{
		case easing::circularIn:
			return static_cast<int>(-d * (sqrtf(1 - position * position) - 1) + start);
		}
		return static_cast<int>(d * position + start);
	}
};
