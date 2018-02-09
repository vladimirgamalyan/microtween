#pragma once
#include <vector>
#include <cmath>

class microtween
{
public:

	enum class easing {
		linear,
		quadratic_in,
		quadratic_out,
		quadratic_in_out,
		cubic_in,
		cubic_out,
		cubic_in_out,
		quartic_in,
		quartic_out,
		quartic_in_out,
		exponential_in,
		exponential_out,
		exponential_in_out,
		circular_in,
		circular_out,
		circular_in_out,
		elastic_in,
		elastic_out,
		elastic_in_out,
		back_in,
		back_out
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
		return get(cursor);
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
		const float PI_F = 3.14159265358979f;
		float d = end - start;
		switch (e)
		{
		case easing::quadratic_in:
			return d * position * position + start;

		case easing::quadratic_out:
			return -d * position * (position - 2) + start;

		case easing::quadratic_in_out:
			position *= 2;
			if (position < 1)
				return (d / 2) * position * position + start;
			--position;
			return (-d / 2) * (position * (position - 2) - 1) + start;

		case easing::cubic_in:
			return d * position * position * position + start;

		case easing::cubic_out:
			--position;
			return d * (position * position * position + 1) + start;

		case easing::cubic_in_out:
			position *= 2;
			if (position < 1)
				return (d / 2) * position * position * position + start;
			position -= 2;
			return (d / 2) * (position * position * position + 2) + start;

		case easing::quartic_in:
			return d * position * position * position * position + start;

		case easing::quartic_out:
			--position;
			return -d * (position * position * position * position - 1) + start;

		case easing::quartic_in_out:
			position *= 2;
			if (position < 1)
				return (d / 2) * (position * position * position * position) + start;
			position -= 2;
			return (-d / 2) * (position * position * position * position - 2) + start;

		case easing::exponential_in:
			return d * powf(2, 10 * (position - 1)) + start;

		case easing::exponential_out:
			return d * (-powf(2, -10 * position) + 1) + start;

		case easing::exponential_in_out:
			position *= 2;
			if (position < 1) {
				return (d / 2) * powf(2, 10 * (position - 1)) + start;
			}
			--position;
			return (d / 2) * (-powf(2, -10 * position) + 2) + start;

		case easing::circular_in:
			return -d * (sqrtf(1 - position * position) - 1) + start;

		case easing::circular_out:
			--position;
			return d * (sqrtf(1 - position * position)) + start;

		case easing::circular_in_out:
			position *= 2;
			if (position < 1)
				return (-d / 2) * (sqrtf(1 - position * position) - 1) + start;
			position -= 2;
			return (d / 2) * (sqrtf(1 - position * position) + 1) + start;

		case easing::elastic_in:
		{
			if (position <= 0.00001f)
				return start;
			if (position >= 0.999f)
				return end;
			float p = .3f;
			float s = p / 4;
			float postFix =
				d * powf(2, 10 * (position -= 1)); // this is a fix, again, with post-increment operators
			return -(postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p)) + start;
		}

		case easing::elastic_out:
		{
			if (position <= 0.00001f)
				return start;
			if (position >= 0.999f)
				return end;
			float p = .3f;
			float s = p / 4;
			return d * powf(2, -10 * position) * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p) + end;
		}

		case easing::elastic_in_out:
		{
			if (position <= 0.00001f)
				return start;
			if (position >= 0.999f)
				return end;
			position *= 2;
			float p = (.3f * 1.5f);
			float s = p / 4;
			float postFix;
			if (position < 1)
			{
				postFix = d * powf(2, 10 * (position -= 1));
				return -0.5f * (postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p)) + start;
			}
			postFix = d * powf(2, -10 * (position -= 1));
			return postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p) * .5f + end;
		}

		case easing::back_in:
		{
			float s = 1.70158f;
			return d * position * position * ((s + 1) * position - s) + start;
		}

		case easing::back_out:
		{
			float s = 1.70158f;
			position -= 1;
			return d * ((position)* position * ((s + 1) * position + s) + 1) + start;
		}

		}
		return d * position + start;
	}
};

