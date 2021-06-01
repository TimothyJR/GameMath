#pragma once
#include<memory>
#include <vector>

class Helpers
{
public:
	template <class T>
	static std::unique_ptr<T[]> copyVectorToArrayShallow(const std::vector<T>& input);
};

template<class T>
inline std::unique_ptr<T[]> Helpers::copyVectorToArrayShallow(const std::vector<T>& input)
{
	std::unique_ptr<T[]> copy = std::unique_ptr<T[]>(new T[input.size()]);

	memcpy(copy.get(), input.data(), sizeof(T) * input.size());

	return copy;
}
