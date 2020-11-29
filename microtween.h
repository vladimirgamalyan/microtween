#pragma once
#include <vector>
#include <cmath>
#include <functional>

class microtween
{
public:

	enum class easing {
		linear,
		sine_in,
		sine_out,
		sine_in_out,
		quadratic_in,
		quadratic_out,
		quadratic_in_out,
		cubic_in,
		cubic_out,
		cubic_in_out,
		quartic_in,
		quartic_out,
		quartic_in_out,
		quintic_in,
		quintic_out,
		quintic_in_out,
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

	typedef std::function<void(void)> cb_t;

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

	microtween& wait(int d)
	{
		float end = from_value;
		if (!sequence.empty())
			end = sequence.back().end;
		return to(end, d);
	}

	microtween& call(const cb_t& cb)
	{
		sequence.back().cb = cb;
		return *this;
	}

	void step(int s = 1)
	{
		int c = cursor;
		for (const auto& i : sequence)
		{
			if (c < i.duration)
			{
				if (i.cb && c + s >= i.duration)
					i.cb();
				break;
			}
			c -= i.duration;
		}

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

	int duration() const
	{
		int result = 0;
		for (const auto& i : sequence)
			result += i.duration;
		return result;
	}

	bool finished() const
	{
		return cursor >= duration();
	}

private:
	struct tween_point
	{
		tween_point(float end, int duration, easing easing) : end(end),
			duration(duration), easing(easing) {}
		float end;
		int duration;
		easing easing;
		cb_t cb;
	};

	float from_value = 0;
	int cursor = 0;
	std::vector<tween_point> sequence;
	float interpolate(float t, easing e) const
	{
		const float pi = 3.141592654f;
		const float half_pi = 1.570796327f;

		switch (e)
		{
		case easing::sine_in:
			return 1.f + sinf(half_pi * (t - 1.0f));

		case easing::sine_out:
			return sinf(half_pi * t);

		case easing::sine_in_out:
			return .5f * (1.f - cosf(t * pi));

		case easing::quadratic_in:
			return t * t;

		case easing::quadratic_out:
			return -1.f * t * (t - 2);

		case easing::quadratic_in_out:
			t *= 2;
			if (t < 1)
				return .5f * t * t;
			--t;
			return -.5f * (t * (t - 2) - 1);

		case easing::cubic_in:
			return t * t * t;

		case easing::cubic_out:
			--t;
			return t * t * t + 1;

		case easing::cubic_in_out:
			t *= 2;
			if (t < 1)
				return .5f * t * t * t;
			t -= 2;
			return .5f * (t * t * t + 2);

		case easing::quartic_in:
			return t * t * t * t;

		case easing::quartic_out:
			--t;
			return -(t * t * t * t - 1);

		case easing::quartic_in_out:
			t *= 2;
			if (t < 1)
				return .5f * (t * t * t * t);
			t -= 2;
			return -.5f * (t * t * t * t - 2);

		case easing::quintic_in:
		{
			return powf(t, 5);
		}

		case easing::quintic_out:
		{
			return 1.f + powf(t - 1.f, 5);
		}

		case easing::quintic_in_out:
		{
			if (t < .5f)
				return 16.f * powf(t, 5);

			return 1.f - 16.f * powf(t - 1.f, 5);
		}

		case easing::exponential_in:
			return powf(2, 10 * (t - 1));

		case easing::exponential_out:
			return -powf(2, -10 * t) + 1;

		case easing::exponential_in_out:
			t *= 2;
			if (t < 1)
				return .5f * powf(2, 10 * (t - 1));
			--t;
			return .5f * (-powf(2, -10 * t) + 2);

		case easing::circular_in:
			return -(sqrtf(1 - t * t) - 1);

		case easing::circular_out:
			--t;
			return sqrtf(1 - t * t);

		case easing::circular_in_out:
			t *= 2;
			if (t < 1)
				return -.5f * (sqrtf(1 - t * t) - 1);
			t -= 2;
			return .5f * (sqrtf(1 - t * t) + 1);

		case easing::elastic_in:
		{
			if (t <= 0.00001f)
				return 0.f;
			if (t >= 0.999f)
				return 1.f;
			float p = .3f;
			float s = p / 4;
			float postFix = powf(2, 10 * (t -= 1)); // this is a fix, again, with post-increment operators
			return -(postFix * sinf((t - s) * (2 * static_cast<float>(pi)) / p));
		}

		case easing::elastic_out:
		{
			if (t <= 0.00001f)
				return 0.f;
			if (t >= 0.999f)
				return 1.f;
			float p = .3f;
			float s = p / 4;
			return powf(2, -10 * t) * sinf((t - s) * (2 * static_cast<float>(pi)) / p) + 1.f;
		}

		case easing::elastic_in_out:
		{
			if (t <= 0.00001f)
				return 0.f;
			if (t >= 0.999f)
				return 1.f;
			t *= 2;
			float p = (.3f * 1.5f);
			float s = p / 4;
			float postFix;
			if (t < 1)
			{
				postFix = powf(2, 10 * (t -= 1));
				return -0.5f * (postFix * sinf((t - s) * (2 * static_cast<float>(pi)) / p));
			}
			postFix = powf(2, -10 * (t -= 1));
			return postFix * sinf((t - s) * (2 * static_cast<float>(pi)) / p) * .5f + 1.f;
		}

		case easing::back_in:
		{
			float s = 1.70158f;
			return t * t * ((s + 1) * t - s);
		}

		case easing::back_out:
		{
			float s = 1.70158f;
			t -= 1;
			return t * t * ((s + 1) * t + s) + 1;
		}

		case easing::back_in_out:
		{
			float s = 1.70158f * 1.525f;
			if ((t /= .5f) < 1)
				return .5f * (t * t * ((s + 1) * t - s));
			float p = t -= 2;
			return .5f * (p * t * ((s + 1) * t + s) + 2);
		}

		}
		return t;
	}
};
