#pragma once

#include <cstdint>

struct 結構_視訊
{
public:
	uint16_t 作者標識 = 0;
	uint16_t 時長 = 0;
	uint16_t 配文詞數 = 0;
	uint16_t 圖片識別詞數 = 0;
	uint16_t 語音識別詞數 = 0;
	uint16_t 配文字數 = 0;
	uint16_t 圖片識別字數 = 0;
	uint16_t 語音識別字數 = 0;
	uint16_t 首人工標類 = 0;
	uint16_t 首機器標類 = 0;
	uint16_t 類目陣列[類目種數] = { 0 };

	結構_視訊() = default;

	結構_視訊(
		uint16_t 作者標識, uint16_t 時長
		, uint16_t 配文詞數, uint16_t 圖片識別詞數, uint16_t 語音識別詞數
		, uint16_t 配文字數, uint16_t 圖片識別字數, uint16_t 語音識別字數
		, uint16_t 首人工標類, uint16_t 首機器標類
	) :
		作者標識(作者標識), 時長(時長)
		, 配文詞數(配文詞數), 圖片識別詞數(圖片識別詞數), 語音識別詞數(語音識別詞數)
		, 配文字數(配文字數), 圖片識別字數(圖片識別字數), 語音識別字數(語音識別字數)
		, 首人工標類(首人工標類), 首機器標類(首機器標類)
	{
	}

	auto 輸出(std::vector<uint16_t>& 資料向量) const
	{
		資料向量.emplace_back(轉半精度(float(時長)));
		資料向量.emplace_back(轉半精度(float(配文詞數)));
		資料向量.emplace_back(轉半精度(float(圖片識別詞數)));
		資料向量.emplace_back(轉半精度(float(語音識別詞數)));
		for (auto 甲 = 0U; 甲 < 類目種數; 甲++)
			資料向量.emplace_back(轉半精度(float(類目陣列[甲])));
	}
};
