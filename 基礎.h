#pragma once

#include <cmath>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <sstream>

#define NAN16 32256

auto 分割字串(const std::string& 字串, char 分割字元)
{
	auto 當前迭代器 = 字串.begin();
	std::vector<std::string> 分割向量;
	for (auto 迭代器 = 字串.begin(); 迭代器 != 字串.end(); 迭代器++)
	{
		if (*迭代器 == 分割字元)
		{
			分割向量.push_back(std::string(當前迭代器, 迭代器));
			當前迭代器 = 1 + 迭代器;
		}
	}

	分割向量.push_back(std::string(當前迭代器, 字串.end()));

	return 分割向量;
}

auto 取得時間()
{
	auto 時間 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	return std::put_time(std::localtime(&時間), "%H:%M:%S");
}


template <typename 型別甲, typename 型別乙>
auto 取得標識(型別甲 甲, 型別乙 乙)
{
	std::stringstream 字串流;
	字串流 << 甲 << "_" << 乙;

	return 字串流.str();
}

template <typename 型別甲, typename 型別乙, typename 型別丙>
auto 取得標識(型別甲 甲, 型別乙 乙, 型別丙 丙)
{
	std::stringstream 字串流;
	字串流 << 甲 << "_" << 乙 << "_" << 丙;

	return 字串流.str();
}

template <typename 型別甲, typename 型別乙, typename 型別丙, typename 型別丁>
auto 取得標識(型別甲 甲, 型別乙 乙, 型別丙 丙, 型別丁 丁)
{
	std::stringstream 字串流;
    字串流 << 甲 << "_" << 乙 << "_" << 丙 << "_" << 丁;

	return 字串流.str();
}

uint16_t 轉半精度(float 值)
{
	if (std::isnan(值))
		return NAN16;

	auto 整值 = *reinterpret_cast<uint32_t*>(&值);
	auto 臨時值 = uint16_t((整值 >> 23) & 255);
	臨時值 = (臨時值 - 0x70) & (uint32_t(int32_t(0x70 - 臨時值) >> 4) >> 27);

	auto 結果 = uint16_t(整值 >> 31) << 5;
	結果 = (結果 | 臨時值) << 10;
	結果 |= (整值 >> 13) & 1023;

	return 結果;
}
