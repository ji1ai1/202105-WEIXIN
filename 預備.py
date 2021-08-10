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
