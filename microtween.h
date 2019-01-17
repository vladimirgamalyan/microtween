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
		back_out,
		back_in_out
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
		if (sequence.empty())
			return from_value;
		
		float start = from_value;
		for (const auto& i : sequence)
		{
			if (c < i.duration)
				return start + (i.end - start) * interpolate(static_cast<float>(c) / i.duration, i.easing);
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
	float interpolate(float position, easing e) const
	{
		const float PI_F = 3.14159265358979f;
		switch (e)
		{
		case easing::quadratic_in:
			return position * position;

		case easing::quadratic_out:
			return -1.f * position * (position - 2);

		case easing::quadratic_in_out:
			position *= 2;
			if (position < 1)
				return .5f * position * position;
			--position;
			return -.5f * (position * (position - 2) - 1);

		case easing::cubic_in:
			return position * position * position;

		case easing::cubic_out:
			--position;
			return position * position * position + 1;

		case easing::cubic_in_out:
			position *= 2;
			if (position < 1)
				return .5f * position * position * position;
			position -= 2;
			return .5f * (position * position * position + 2);

		case easing::quartic_in:
			return position * position * position * position;

		case easing::quartic_out:
			--position;
			return -(position * position * position * position - 1);

		case easing::quartic_in_out:
			position *= 2;
			if (position < 1)
				return .5f * (position * position * position * position);
			position -= 2;
			return -.5f * (position * position * position * position - 2);

		case easing::exponential_in:
			return powf(2, 10 * (position - 1));

		case easing::exponential_out:
			return -powf(2, -10 * position) + 1;

		case easing::exponential_in_out:
			position *= 2;
			if (position < 1)
				return .5f * powf(2, 10 * (position - 1));
			--position;
			return .5f * (-powf(2, -10 * position) + 2);

		case easing::circular_in:
			return -(sqrtf(1 - position * position) - 1);

		case easing::circular_out:
			--position;
			return sqrtf(1 - position * position);

		case easing::circular_in_out:
			position *= 2;
			if (position < 1)
				return -.5f * (sqrtf(1 - position * position) - 1);
			position -= 2;
			return .5f * (sqrtf(1 - position * position) + 1);

		case easing::elastic_in:
		{
			if (position <= 0.00001f)
				return 0.f;
			if (position >= 0.999f)
				return 1.f;
			float p = .3f;
			float s = p / 4;
			float postFix = powf(2, 10 * (position -= 1)); // this is a fix, again, with post-increment operators
			return -(postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p));
		}

		case easing::elastic_out:
		{
			if (position <= 0.00001f)
				return 0.f;
			if (position >= 0.999f)
				return 1.f;
			float p = .3f;
			float s = p / 4;
			return powf(2, -10 * position) * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p) + 1.f;
		}

		case easing::elastic_in_out:
		{
			if (position <= 0.00001f)
				return 0.f;
			if (position >= 0.999f)
				return 1.f;
			position *= 2;
			float p = (.3f * 1.5f);
			float s = p / 4;
			float postFix;
			if (position < 1)
			{
				postFix = powf(2, 10 * (position -= 1));
				return -0.5f * (postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p));
			}
			postFix = powf(2, -10 * (position -= 1));
			return postFix * sinf((position - s) * (2 * static_cast<float>(PI_F)) / p) * .5f + 1.f;
		}

		case easing::back_in:
		{
			float s = 1.70158f;
			return position * position * ((s + 1) * position - s);
		}

		case easing::back_out:
		{
			float s = 1.70158f;
			position -= 1;
			return (position)* position * ((s + 1) * position + s) + 1;
		}

		case easing::back_in_out:
		{
			float s = 1.70158f * 1.525f;
			float t = position;
			if ((t /= .5f) < 1)
				return .5f * (t * t * ((s + 1) * t - s));
			float postFix = t -= 2;
			return .5f * ((postFix)* t * ((s + 1) * t + s) + 2);
		}

		}
		return position;
	}
};
