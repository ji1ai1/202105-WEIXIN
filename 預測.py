import datetime
import lightgbm
import numpy
import pandas
import pickle
import random
import sklearn

print(str(datetime.datetime.now()) + "\t開始")

標籤型別 = ["査評", "評論", "讚", "點撃頭像", "轉發", "關注", "收藏"]
標籤 = ["査評標籤", "評論標籤", "讚標籤", "點撃頭像標籤", "轉發標籤", "關注標籤", "收藏標籤"]
標準特征 = ["樣本數"] + ["%s標籤率" % 子 for 子 in 標籤型別] + ["設備平均値", "視訊時長平均値", "播放時長平均値", "停留時長平均値", "時長特征三", "時長特征四", "時長特征五"]
欄名 = [
	"用戸標識", "視訊標識", "日期", "査評標籤", "評論標籤", "讚標籤", "點撃頭像標籤", "轉發標籤", "關注標籤", "收藏標籤", "設備"
	, "視訊時長", "配文詞數", "圖片識別詞數", "語音識別詞數"] + ["類目%s" % 子 for 子 in range(5)] \
	+ ["作者視訊數", "首人工標類視訊數", "首機器標類視訊數"] + ["類目%s視訊數" % 子 for 子 in range(5)] \
	+ ["視訊%s" % 子 for 子 in 標準特征 if 子 != "視訊時長平均値"] \
	+ ["視訊加權%s標籤率" % 子 for 子 in 標籤型別] + ["視訊用戸播放時長平均値", "視訊用戸停留時長平均値", "視訊用戸視訊時長平均値"] \
	+ ["作者%s" % 子 for 子 in 標準特征] \
	+ ["首人工標類%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["首機器標類%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["類目%s%s標籤率" % (子, 丑) for 子 in range(5) for 丑 in 標籤型別] \
	+ ["用戸%s" % 子 for 子 in 標準特征] \
	+ ["用戸第二視訊%s標籤率" % 子 for 子 in 標籤型別] + ["用戸第二視訊播放時長平均値", "用戸第二視訊停留時長平均値", "用戸第二視訊視訊時長平均値"] \
	+ ["用戸第二作者%s標籤率" % 子 for 子 in 標籤型別] + ["用戸第二作者播放時長平均値", "用戸第二作者停留時長平均値", "用戸第二作者視訊時長平均値"] \
	+ ["用戸第二首人工標類%s標籤率" % 子 for 子 in 標籤型別] + ["用戸第二首人工標類播放時長平均値", "用戸第二首人工標類停留時長平均値", "用戸第二首人工標類視訊時長平均値"] \
	+ ["用戸第二首機器標類%s標籤率" % 子 for 子 in 標籤型別] + ["用戸第二首機器標類播放時長平均値", "用戸第二首機器標類停留時長平均値", "用戸第二首機器標類視訊時長平均値"] \
	+ [寅 for 丑 in range(5) for 寅 in ["用戸第二類目種類目%s%s標籤率" % (丑, 子) for 子 in 標籤型別] + ["用戸第二類目種類目%s播放時長平均値" % 丑, "用戸第二類目種類目%s停留時長平均値" % 丑, "用戸第二類目種類目%s視訊時長平均値" % 丑]] \
	+ ["用戸設備%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["用戸作者%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["用戸首人工標類%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["用戸首機器標類%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["首人機標類%s標籤率" % 子 for 子 in 標籤型別] \
	+ ["用戸類目%s%s標籤率" % (子, 丑) for 子 in range(5) for 丑 in 標籤型別] \
	+ ["用戸作者類目%s%s標籤率" % (子, 丑) for 子 in [5] for 丑 in 標籤型別] \
	+ ["視頻當日樣本數比", "作者當日樣本數比", "首人工標類當日樣本數比", "首機器標類當日樣本數比"] + ["類目%s當日樣本數比" % 子 for 子 in range(5)] \
	+ ["用戸設備當日樣本數", "用戸作者當日樣本數", "用戸首人工標類當日樣本數", "用戸首機器標類當日樣本數"] + ["用戸類目%s當日樣本數" % 子 for 子 in range(5)] \


訓練資料表 = pandas.concat([
    pandas.DataFrame(numpy.concatenate([numpy.fromfile("資料/資料_標識%s" % 子, dtype=numpy.uint32) for 子 in range(1, 15)]).reshape([-1, 2]), columns=欄名[:2])
    , pandas.DataFrame(numpy.concatenate([numpy.fromfile("資料/資料_%s" % 子, dtype=numpy.float16) for 子 in range(1, 15)]).reshape([-1, len(欄名) - 2]), columns=欄名[2:])
], axis=1)
測試資料表 = pandas.concat([
    pandas.DataFrame(numpy.fromfile("資料/資料_標識15", dtype=numpy.uint32).reshape([-1, 2]), columns=欄名[:2])
    , pandas.DataFrame(numpy.fromfile("資料/資料_15", dtype=numpy.float16).reshape([-1, len(欄名) - 2]), columns=欄名[2:])
], axis=1)

預測表 = 測試資料表.loc[:, ["用戸標識", "視訊標識", "設備"]]
輕模型 = [None for 子 in range(7)]
選擇標籤特征字典 = {
	"査評": ["査評", "評論", "讚", "點撃頭像"]
	, "評論": 標籤型別
	, "讚": ["査評", "讚", "轉發"]
	, "點撃頭像": ["査評", "點撃頭像", "關注"]
	, "轉發": 標籤型別
	, "關注": 標籤型別
	, "收藏": 標籤型別
}
for 甲, 甲標籤型別 in enumerate(標籤型別):
	選取特征 = [子 for 子, 子欄 in enumerate(測試資料表.columns) if 子 >= 3 + len(標籤型別) and not numpy.any([子欄.find(丑 + "標籤") > -1 for 丑 in 標籤型別 if 丑 not in 選擇標籤特征字典[甲標籤型別]])]
	甲訓練資料表 = 訓練資料表.loc[訓練資料表.用戸標識.isin(訓練資料表[訓練資料表.iloc[:, 3 + 甲] == 1].用戸標識)]
	print(str(datetime.datetime.now()) + "\t開始訓練第%s箇模型，選取%s箇特征, %s箇樣本" % (甲, len(選取特征), len(甲訓練資料表)))
	
	輕模型[甲] = lightgbm.train(
		train_set=lightgbm.Dataset(甲訓練資料表.iloc[:, 選取特征], label=甲訓練資料表.iloc[:, 3 + 甲])
		, num_boost_round=[4096, 1024, 4096, 2048, 2048, 1024, 2048][甲], params={"objective": "binary", "learning_rate": [0.05, 0.02, 0.05, 0.05, 0.02, 0.02, 0.02][甲], "lambda_l2": 10, "min_child_weight": 1.5, "max_depth": 6, "num_leaves": 32, "bagging_fraction": 0.7, "feature_fraction": 0.7, "num_threads": 64, "verbose": -1}
	)
	
	with open("資料/輕模型%s" % 甲, "wb") as 档案:
		pickle.dump(輕模型[甲], 档案)
	
	預測表[甲標籤型別 + "打分"] = 輕模型[甲].predict(測試資料表.iloc[:, 選取特征])


提交表 = 預測表.loc[:, ["用戸標識", "視訊標識", "査評打分", "讚打分", "點撃頭像打分", "轉發打分", "收藏打分", "評論打分", "關注打分"]]
提交表.columns = ["userid", "feedid", "read_comment", "like", "click_avatar", "forward", "favorite", "comment", "follow"]
提交表.to_csv("result.csv", index=False)

print(str(datetime.datetime.now()) + "\t結束")
