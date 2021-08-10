#define _CRT_SECURE_NO_WARNINGS

#include <condition_variable>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const auto 日數 = uint32_t(15);
const auto 折數 = uint32_t(8);
const auto 視訊數 = uint32_t(112873);
const auto 類目種數 = uint32_t(5);
const auto 主成分數 = uint32_t(32);
const uint32_t 類目數陣列[] = { 9, 27, 81, 243, 729, };
const auto 作者數 = uint32_t(18789);
const auto 首人工標類數 = uint32_t(354);
const auto 首機器標類數 = uint32_t(348);
const auto 用戶數 = uint32_t(250249);
const auto 最大執行緒數 = 15;

#include "基礎.h"
#include "視訊.h"
#include "行爲.h"
#include "特征.h"

auto 讀視訊資料(結構_視訊* 視訊陣列, const std::string& 路徑)
{
	std::ifstream 輸入流(路徑);
	std::string 字串;
	std::getline(輸入流, 字串);
	auto 列數 = 0;
	while (std::getline(輸入流, 字串))
	{
		列數++;
		auto&& 字串分割向量 = 分割字串(字串, ',');
		auto 視訊標識 = std::stoi(字串分割向量[0]);
		auto 作者標識 = uint16_t(std::stoi(字串分割向量[1]));
		auto 時長 = uint16_t(std::stoi(字串分割向量[2]));
		auto 配文詞數 = uint16_t(分割字串(字串分割向量[3], ' ').size());
		auto 圖片識別詞數 = uint16_t(分割字串(字串分割向量[4], ' ').size());
		auto 語音識別詞數 = uint16_t(分割字串(字串分割向量[5], ' ').size());
		auto 配文字數 = uint16_t(分割字串(字串分割向量[12], ' ').size());
		auto 圖片識別字數 = uint16_t(分割字串(字串分割向量[13], ' ').size());
		auto 語音識別字數 = uint16_t(分割字串(字串分割向量[14], ' ').size());

		auto 首人工標類 = uint16_t(首人工標類數 - 1);
		if (字串分割向量[10].size() > 0)
		{
			auto&& 人工標類字串分割向量 = 分割字串(字串分割向量[10], ';');
			首人工標類 = std::stoi(人工標類字串分割向量[0]);
		}
		auto 首機器標類 = uint16_t(首機器標類數 - 1);
		auto 首機器標類打分 = -1;
		if (字串分割向量[11].size() > 0)
		{
			auto&& 機器標類字串分割向量 = 分割字串(字串分割向量[11], ';');
			for (auto 機器標類字串 : 機器標類字串分割向量)
			{
				auto&& 機器標類字串分割向量 = 分割字串(機器標類字串, ' ');
				if (機器標類字串分割向量.size() < 2)
					continue;
				if (std::stod(機器標類字串分割向量[1]) > 首機器標類打分)
					首機器標類 = std::stoi(機器標類字串分割向量[0]);
			}
		}
		視訊陣列[視訊標識] = 結構_視訊(作者標識, 時長, 配文詞數, 圖片識別詞數, 語音識別詞數, 配文字數, 圖片識別字數, 語音識別字數, 首人工標類, 首機器標類);
	}
	輸入流.close();
	std::cout << 取得時間() << " \u5df2\u8b80\u53d6" << std::to_string(列數) << "\u5217\u8996\u8a0a\u8cc7\u6599......" << std::endl;

	return 0;
}

auto 讀類目資料(結構_視訊* 視訊陣列, const std::string& 路徑)
{
	std::ifstream 輸入流(路徑);
	std::string 字串;
	std::getline(輸入流, 字串);
	auto 列數 = 0;
	while (std::getline(輸入流, 字串))
	{
		列數++;
		auto&& 字串分割向量 = 分割字串(字串, ',');
		auto 視訊標識 = uint32_t(std::stoi(字串分割向量[0]));
		for (auto 甲 = 0U; 甲 < 類目種數; 甲++)
			視訊陣列[視訊標識].類目陣列[甲] = uint16_t(std::stoi(字串分割向量[1 + 甲]));
	}
	輸入流.close();
	std::cout << 取得時間() << " \u5df2\u8b80\u53d6" << std::to_string(列數) << "\u5217\u8996\u8a0a\u985e\u76ee\u8cc7\u6599......" << std::endl;

	return 0;
}

auto 匯入樣本數統計資料(
	uint16_t* 測試日某樣本數比陣列
	, const std::string& 路徑
	, uint32_t 某數
)
{
	auto 測試日某樣本數比陣列檔案 = fopen(路徑.c_str(), "rb");
	fread(測試日某樣本數比陣列, sizeof(uint16_t), 某數, 測試日某樣本數比陣列檔案);
	fclose(測試日某樣本數比陣列檔案);
}

auto 取得測試資料(
	const std::string& 路徑
	, const std::string& 輸入路徑
	, const std::string& 統計資料路徑
	, const 結構_視訊* 視訊陣列
)
{
	auto 標識檔案 = fopen((路徑 + "_\u6a19\u8b58" + std::to_string(日數)).c_str(), "wb");
	auto 檔案 = fopen((路徑 + "_" + std::to_string(日數)).c_str(), "wb");
	std::vector<uint32_t> 排除折向量{ 折數 };

	auto 雜資料檔案 = fopen((統計資料路徑 + "/\u96dc\u8cc7\u6599").c_str(), "rb");
	uint32_t 雜資料[4];
	fread(雜資料, sizeof(uint32_t), 4, 雜資料檔案);
	fclose(雜資料檔案);
	auto 視訊資料長度 = 雜資料[1];
	auto 用戶資料長度 = 雜資料[2];
	auto 交叉特征資料長度 = 雜資料[3];

	auto 視訊資料陣列陣列 = new uint16_t * [視訊數];
	auto 視訊特征檔案 = fopen((統計資料路徑 + "/\u8996\u8a0a\u7279\u5f81").c_str(), "rb");
	for (auto 甲 = 0U; 甲 < 視訊數; 甲++)
	{
		視訊資料陣列陣列[甲] = new uint16_t[視訊資料長度];
		fread(視訊資料陣列陣列[甲], sizeof(uint16_t), 視訊資料長度, 視訊特征檔案);
	}
	fclose(視訊特征檔案);

	auto 用戶資料陣列陣列 = new uint16_t*[用戶數];
	auto 用戶特征檔案 = fopen((統計資料路徑 + "/\u7528\u6236\u7279\u5f81").c_str(), "rb");
	for (auto 甲 = 0U; 甲 < 用戶數; 甲++)
	{
		用戶資料陣列陣列[甲] = new uint16_t[用戶資料長度];
		fread(用戶資料陣列陣列[甲], sizeof(uint16_t), 用戶資料長度, 用戶特征檔案);
	}
	fclose(用戶特征檔案);

	auto 測試日視訊樣本數比陣列 = new uint16_t[視訊數];
	匯入樣本數統計資料(測試日視訊樣本數比陣列, 統計資料路徑 + "/\u6e2c\u8a66\u65e5\u8996\u8a0a\u6a23\u672c\u6578\u6bd4\u9663\u5217", 視訊數);
	auto 測試日作者樣本數比陣列 = new uint16_t[作者數];
	匯入樣本數統計資料(測試日作者樣本數比陣列, 統計資料路徑 + "/\u6e2c\u8a66\u65e5\u4f5c\u8005\u6a23\u672c\u6578\u6bd4\u9663\u5217", 作者數);
	auto 測試日首人工標類樣本數比陣列 = new uint16_t[首人工標類數];
	匯入樣本數統計資料(測試日首人工標類樣本數比陣列, 統計資料路徑 + "/\u6e2c\u8a66\u65e5\u9996\u4eba\u5de5\u6a19\u985e\u6a23\u672c\u6578\u6bd4\u9663\u5217", 首人工標類數);
	auto 測試日首機器標類樣本數比陣列 = new uint16_t[首機器標類數];
	匯入樣本數統計資料(測試日首機器標類樣本數比陣列, 統計資料路徑 + "/\u6e2c\u8a66\u65e5\u9996\u6a5f\u5668\u6a19\u985e\u6a23\u672c\u6578\u6bd4\u9663\u5217", 首機器標類數);
	auto  類目種測試日首機器標類樣本數比陣列陣列 = new uint16_t * [類目種數];
	for (auto 甲 = 0U; 甲 < 類目種數; 甲++)
	{
		類目種測試日首機器標類樣本數比陣列陣列[甲] = new uint16_t[類目數陣列[甲]];
		匯入樣本數統計資料(類目種測試日首機器標類樣本數比陣列陣列[甲], 統計資料路徑 + "/\u6e2c\u8a66\u65e5\u985e\u76ee" + std::to_string(甲) + "\u6a23\u672c\u6578\u6bd4\u9663\u5217", 類目數陣列[甲]);
	}

	char 鍵[64];
	std::unordered_map<std::string, uint16_t> 測試日交叉樣本數映射;
	auto 測試日交叉樣本數映射檔案 = fopen((統計資料路徑 + "/\u6e2c\u8a66\u65e5\u4ea4\u53c9\u6a23\u672c\u6578\u6620\u5c04").c_str(), "rb");
	while (true)
	{
		size_t 鍵長;
		if (fread(&鍵長, sizeof(size_t), 1, 測試日交叉樣本數映射檔案) == 0)
			break;

		uint16_t 值;
		fread(鍵, sizeof(char), 鍵長, 測試日交叉樣本數映射檔案);
		fread(&值, sizeof(uint16_t), 1, 測試日交叉樣本數映射檔案);
		測試日交叉樣本數映射[std::string(鍵, 鍵長)] = 值;
	}
	fclose(測試日交叉樣本數映射檔案);

	std::unordered_map<std::string, uint16_t*> 測試折交叉特征映射;
	auto 測試折交叉特征映射檔案 = fopen((統計資料路徑 + "/\u6e2c\u8a66\u6298\u4ea4\u53c9\u7279\u5f81\u6620\u5c04").c_str(), "rb");
	while (true)
	{
		size_t 鍵長;
		if (fread(&鍵長, sizeof(size_t), 1, 測試折交叉特征映射檔案) == 0)
			break;

		auto 值 = new uint16_t[交叉特征資料長度];
		fread(鍵, sizeof(char), 鍵長, 測試折交叉特征映射檔案);
		fread(值, sizeof(uint16_t), 交叉特征資料長度, 測試折交叉特征映射檔案);
		測試折交叉特征映射[std::string(鍵, 鍵長)] = 值;
	}
	fclose(測試折交叉特征映射檔案);

	std::vector<uint16_t> 資料向量;
	std::vector<uint16_t> 空交叉特征資料向量;
	結構_特征::空輸出(空交叉特征資料向量, false, false, false, false);

	std::ifstream 輸入流(輸入路徑);
	std::string 字串;
	std::getline(輸入流, 字串);
	while (std::getline(輸入流, 字串))
	{
		auto&& 字串分割向量 = 分割字串(字串, ',');
		auto 用戶標識 = uint32_t(std::stoi(字串分割向量[0]));
		auto 視訊標識 = uint32_t(std::stoi(字串分割向量[1]));
		auto 日期 = 日數;
		auto 設備 = uint16_t(std::stoi(字串分割向量[2]));
		if (視訊標識 > 視訊數 - 1)
			視訊標識 = 視訊數 - 1;
		auto& 視訊 = 視訊陣列[視訊標識];

		uint32_t 標識陣列[]{ 用戶標識, 視訊標識 };
		fwrite(標識陣列, sizeof(uint32_t), 2, 標識檔案);

		資料向量.clear();
		資料向量.emplace_back(轉半精度(float(日期)));
		for (auto 甲 = 0U; 甲 < 7; 甲++)
			資料向量.emplace_back(轉半精度(float(-1)));
		資料向量.emplace_back(轉半精度(float(設備)));
		fwrite(資料向量.data(), 資料向量.size(), sizeof(uint16_t), 檔案);
		fwrite(視訊資料陣列陣列[視訊標識], 視訊資料長度, sizeof(uint16_t), 檔案);

		fwrite(用戶資料陣列陣列[用戶標識], 用戶資料長度, sizeof(uint16_t), 檔案);

		auto&& 用戶設備標識 = 取得標識(用戶標識, 設備);
		auto&& 用戶設備條件型值 = 測試折交叉特征映射.find(用戶設備標識);
		if (用戶設備條件型值 != 測試折交叉特征映射.end())
			fwrite(用戶設備條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		auto&& 用戶作者標識 = 取得標識(1, 用戶標識, 視訊.作者標識);
		auto&& 用戶作者條件型值 = 測試折交叉特征映射.find(用戶作者標識);
		if (用戶作者條件型值 != 測試折交叉特征映射.end())
			fwrite(用戶作者條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		auto&& 用戶首人工標類標識 = 取得標識(2, 用戶標識, 視訊.首人工標類);
		auto&& 用戶首人工標類條件型值 = 測試折交叉特征映射.find(用戶首人工標類標識);
		if (用戶首人工標類條件型值 != 測試折交叉特征映射.end())
			fwrite(用戶首人工標類條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		auto&& 用戶首機器標類標識 = 取得標識(3, 用戶標識, 視訊.首機器標類);
		auto&& 用戶首機器標類條件型值 = 測試折交叉特征映射.find(用戶首機器標類標識);
		if (用戶首機器標類條件型值 != 測試折交叉特征映射.end())
			fwrite(用戶首機器標類條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		auto&& 首人機標類標識 = 取得標識(4, 視訊.首人工標類, 視訊.首機器標類);
		auto&& 首人機標類條件型值 = 測試折交叉特征映射.find(首人機標類標識);
		if (首人機標類條件型值 != 測試折交叉特征映射.end())
			fwrite(首人機標類條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		auto&& 用戶作者類目4標識 = 取得標識(5, 用戶標識, 視訊.作者標識, 視訊.類目陣列[4]);
		auto&& 用戶作者類目4條件型值 = 測試折交叉特征映射.find(用戶作者類目4標識);
		if (用戶作者類目4條件型值 != 測試折交叉特征映射.end())
			fwrite(用戶作者類目4條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
		else
			fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);

		for (auto 乙 = 0U; 乙 < 類目種數; 乙++)
		{
			auto&& 用戶乙類目標識 = 取得標識(16 + 乙, 用戶標識, 視訊.類目陣列[乙]);
			auto&& 用戶乙類目條件型值 = 測試折交叉特征映射.find(用戶乙類目標識);
			if (用戶乙類目條件型值 != 測試折交叉特征映射.end())
				fwrite(用戶乙類目條件型值->second, 交叉特征資料長度, sizeof(uint16_t), 檔案);
			else
				fwrite(空交叉特征資料向量.data(), 交叉特征資料長度, sizeof(uint16_t), 檔案);
		}

		資料向量.clear();
		資料向量.emplace_back(測試日視訊樣本數比陣列[視訊標識]);
		資料向量.emplace_back(測試日作者樣本數比陣列[視訊.作者標識]);
		資料向量.emplace_back(測試日首人工標類樣本數比陣列[視訊.首人工標類]);
		資料向量.emplace_back(測試日首機器標類樣本數比陣列[視訊.首機器標類]);
		for (auto 乙 = 0U; 乙 < 類目種數; 乙++)
			資料向量.emplace_back(類目種測試日首機器標類樣本數比陣列陣列[乙][視訊.類目陣列[乙]]);

		if (測試日交叉樣本數映射.find(用戶設備標識) != 測試日交叉樣本數映射.end())
			資料向量.emplace_back(測試日交叉樣本數映射[用戶設備標識]);
		else
			資料向量.emplace_back(NAN16);

		if (測試日交叉樣本數映射.find(用戶作者標識) != 測試日交叉樣本數映射.end())
			資料向量.emplace_back(測試日交叉樣本數映射[用戶作者標識]);
		else
			資料向量.emplace_back(NAN16);

		if (測試日交叉樣本數映射.find(用戶首人工標類標識) != 測試日交叉樣本數映射.end())
			資料向量.emplace_back(測試日交叉樣本數映射[用戶首人工標類標識]);
		else
			資料向量.emplace_back(NAN16);

		if (測試日交叉樣本數映射.find(用戶首機器標類標識) != 測試日交叉樣本數映射.end())
			資料向量.emplace_back(測試日交叉樣本數映射[用戶首機器標類標識]);
		else
			資料向量.emplace_back(NAN16);

		for (auto 乙 = 0U; 乙 < 類目種數; 乙++)
		{
			auto&& 用戶乙類目標識 = 取得標識(16 + 乙, 用戶標識, 視訊.類目陣列[乙]);
			if (測試日交叉樣本數映射.find(用戶乙類目標識) != 測試日交叉樣本數映射.end())
				資料向量.emplace_back(測試日交叉樣本數映射[用戶乙類目標識]);
			else
				資料向量.emplace_back(NAN16);
		}

		fwrite(資料向量.data(), 資料向量.size(), sizeof(uint16_t), 檔案);
	}
	輸入流.close();
	fclose(檔案);
	fclose(標識檔案);

	for (auto& 鍵值對 : 測試折交叉特征映射)
		delete[] 鍵值對.second;
	for (auto 甲 = 0U; 甲 < 類目種數; 甲++)
		delete[] 類目種測試日首機器標類樣本數比陣列陣列[甲];
	delete[] 類目種測試日首機器標類樣本數比陣列陣列;
	delete[] 測試日首機器標類樣本數比陣列;
	delete[] 測試日首人工標類樣本數比陣列;
	delete[] 測試日作者樣本數比陣列;
	delete[] 測試日視訊樣本數比陣列;
	for (auto 甲 = 0U; 甲 < 視訊數; 甲++)
		delete[] 視訊資料陣列陣列[甲];
	delete[] 視訊資料陣列陣列;

	std::stringstream 字串流;
	字串流 << 取得時間() << " \u5df2\u53d6\u5f97\u7b2c" << 日數 << "\u65e5\u8cc7\u6599......" << std::endl;
	std::cout << 字串流.str();
}

int main(int 引數數量, char* 引數陣列[])
{
	std::ios_base::sync_with_stdio(false);

	if (引數數量 == 1)
		return 0;

	std::cout << 取得時間() << " \u958b\u59cb......" << std::endl;

	auto 視訊陣列 = new 結構_視訊[視訊數]();
	讀視訊資料(視訊陣列, "../wbdc2021/data/wedata/wechat_algo_data2/feed_info.csv");
	讀類目資料(視訊陣列, "data/\u8996\u8a0a\u985e\u76ee\u88682.csv");

	取得測試資料(
		"data/\u8cc7\u6599"
		, 引數陣列[1]
		, "data"
		, 視訊陣列
	);


	std::cout << 取得時間() << " \u7d50\u675f......" << std::endl;

	return 0;
}