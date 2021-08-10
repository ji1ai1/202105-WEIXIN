import datetime
import lightgbm
import gc
import numpy
import pandas
import pickle
import random
import sklearn
import sklearn.cluster
import sklearn.decomposition

類數 = [9, 27, 81, 243, 729]
視訊嵌入表 = pandas.read_csv("feed_embeddings.csv", skiprows=1, engine="python", sep="[, ]", names=["視訊標識"] + ["視訊嵌入%s" % 子 for 子 in range(512)])
視訊類目表 = 視訊嵌入表.loc[:, ["視訊標識"]]
for 甲 in 類數:
	甲聚類器 = sklearn.cluster.KMeans(n_clusters=甲, random_state=0)
	視訊類目表["視訊類目%d" % 甲] = 甲聚類器.fit_predict(視訊嵌入表.loc[:, ["視訊嵌入%s" % 子 for 子 in range(512)]])
視訊類目表.to_csv("資料/視訊類目表2.csv", index=False)
del 視訊嵌入表

類數 = range(16, 272, 16)
視訊嵌入表 = pandas.read_csv("feed_embeddings.csv", skiprows=1, engine="python", sep="[, ]", names=["視訊標識"] + ["視訊嵌入%s" % 子 for 子 in range(512)])
視訊類目表 = 視訊嵌入表.loc[:, ["視訊標識"]]
for 甲 in 類數:
	甲聚類器 = sklearn.cluster.KMeans(n_clusters=甲, random_state=0)
	視訊類目表["視訊類目%d" % 甲] = 甲聚類器.fit_predict(視訊嵌入表.loc[:, ["視訊嵌入%s" % 子 for 子 in range(512)]])
視訊類目表.to_csv("資料/視訊類目表3.csv", index=False)
del 視訊嵌入表


random.seed(0)
視訊嵌入表 = pandas.read_csv("feed_embeddings.csv", skiprows=1, engine="python", sep="[, ]", names=["視訊標識"] + ["視訊嵌入%s" % 子 for 子 in range(512)])
視訊類目表 = 視訊嵌入表.loc[:, ["視訊標識"]]
for 甲 in range(16):
	print(str(datetime.datetime.now()) + "\t%s" % 甲)
	甲聚類器 = sklearn.cluster.KMeans(n_clusters=32, random_state=0)
	選中欄 = random.sample(range(512), 384)
	視訊類目表["視訊類目%d" % 甲] = 甲聚類器.fit_predict(視訊嵌入表.loc[:, ["視訊嵌入%s" % 子 for 子 in 選中欄]])
視訊類目表.to_csv("資料/視訊類目表4.csv", index=False)
del 視訊嵌入表



主成分數 = 32
主成分分析器 = sklearn.decomposition.PCA(n_components=主成分數, random_state=1)
視訊主成分表 = 視訊嵌入表.loc[:, ["視訊標識"]]
視訊主成分表[["主成分%s" % 子 for 子 in range(主成分數)]] = 主成分分析器.fit_transform(視訊嵌入表.loc[:, ["視訊嵌入%s" % 子 for 子 in range(512)]])
視訊主成分表.to_csv("資料/視訊主成分表.csv", index=False)




import pandas
訓練表 = pandas.read_csv("user_action.csv")
訓練表.date_ += 1

線下訓練表 = 訓練表.loc[(訓練表.userid % 3 == 0) & (訓練表.date_ < 15)].reset_index(drop=True)
線下訓練表.to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/user_action.csv", index=False)

線下測試表 = 訓練表.loc[(訓練表.userid % 3 == 0) & (訓練表.date_ == 15)].reset_index(drop=True)
線下測試表 = 線下測試表.drop_duplicates(["userid", "feedid"], ignore_index=True)
線下測試表 = 線下測試表.merge(線下訓練表.loc[:, ["userid", "feedid"]].assign(a=0), on=["userid", "feedid"], how="left")
線下測試表 = 線下測試表.loc[線下測試表.a.isna()].drop("a", axis=1).reset_index(drop=True)
線下測試表.loc[:, ["userid", "feedid", "read_comment", "comment", "like", "click_avatar", "forward", "follow", "favorite"]].to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/reference_a.csv", index=False)
線下測試表.loc[:, ["userid", "feedid", "device"]].to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/test_a.csv", index=False)









import pandas
訓練表 = pandas.read_csv("user_action.csv")
訓練表.date_ += 1

線下訓練表 = 訓練表.loc[(訓練表.userid % 3 == 0) & (訓練表.date_ < 15)].reset_index(drop=True)
線下訓練表.to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/user_action.csv", index=False)



線下測試表 = 訓練表.loc[(訓練表.userid % 3 == 0) & (訓練表.date_ == 15)].reset_index(drop=True)
線下測試表 = 線下測試表.drop_duplicates(["userid", "feedid"], ignore_index=True)
線下測試表 = 線下測試表.merge(線下訓練表.loc[:, ["userid", "feedid"]].assign(a=0), on=["userid", "feedid"], how="left")
線下測試表 = 線下測試表.loc[線下測試表.a.isna()].drop("a", axis=1).reset_index(drop=True)
線下測試表.loc[:, ["userid", "feedid", "read_comment", "comment", "like", "click_avatar", "forward", "follow", "favorite"]].to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/reference_a.csv", index=False)
線下測試表.loc[:, ["userid", "feedid", "device"]].to_csv("C:/2021.05 微信/wbdc2021/data/wedata/wechat_algo_data2/test_a.csv", index=False)

