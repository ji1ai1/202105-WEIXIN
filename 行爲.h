#pragma once

#include <cstdint>

struct 結構_行爲
{
	uint32_t 用戶標識 = 0;
	uint32_t 視訊標識 = 0;
	uint16_t 設備 = 0;
	uint16_t 日期 = 0;
	uint16_t 折號 = 0;

	結構_行爲(
		uint32_t 用戶標識, uint32_t 視頻標識, uint16_t 設備, uint16_t 日期, uint16_t 折號
	) :
		用戶標識(用戶標識), 視訊標識(視頻標識), 設備(設備), 日期(日期), 折號(折號)
	{
	}

	virtual void 輸出(std::vector<uint16_t>& 資料向量) const = 0;
};

struct 結構_訓練行爲 : 結構_行爲
{
public:
	float 播放時長 = 0;
	float 停留時長 = 0;
	uint16_t 標籤陣列[7]{ 0 };

	結構_訓練行爲() = default;

	結構_訓練行爲(
		uint32_t 用戶標識, uint32_t 視頻標識
		, uint16_t 設備, uint16_t 日期, uint16_t 折號, float 播放時長, float 停留時長, uint16_t 標籤陣列[]
	) : 
		結構_行爲(用戶標識, 視頻標識, 設備, 日期, 折號), 播放時長(播放時長), 停留時長(停留時長)
	{
		for (auto 甲 = 0; 甲 < 7; 甲++)
			this->標籤陣列[甲] = 標籤陣列[甲];
	}

	auto 更新(std::shared_ptr<結構_訓練行爲> 行爲指針)
	{
		for (auto 甲 = 0; 甲 < 7; 甲++)
			標籤陣列[甲] = 標籤陣列[甲] | 行爲指針->標籤陣列[甲];
		if (行爲指針->播放時長 > 播放時長)
			播放時長 = 行爲指針->播放時長;
		if (行爲指針->停留時長 > 停留時長)
			停留時長 = 行爲指針->停留時長;
	}

	virtual void 輸出(std::vector<uint16_t>& 資料向量) const
	{
		for (auto 甲 = 0; 甲 < 7; 甲++)
			資料向量.emplace_back(轉半精度(float(標籤陣列[甲])));
		資料向量.emplace_back(轉半精度(float(設備)));
	}
};

struct 結構_測試行爲 : 結構_行爲
{
public:
	uint16_t 忽略標記 = 0;

	結構_測試行爲() = default;

	結構_測試行爲(
		uint32_t 用戶標識, uint32_t 視頻標識, uint16_t 設備, uint16_t 日期, uint16_t 折號, uint16_t 忽略標記 = 0
	) :
		結構_行爲(用戶標識, 視頻標識, 設備, 日期, 折號), 忽略標記(忽略標記)
	{
	}

	virtual void 輸出(std::vector<uint16_t>& 資料向量) const
	{
		for (auto 甲 = 0; 甲 < 7; 甲++)
			資料向量.emplace_back(轉半精度(float(-1)));
		資料向量.emplace_back(轉半精度(float(設備)));
	}
};
