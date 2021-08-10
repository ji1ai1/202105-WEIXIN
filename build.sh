cat 基礎.h | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > U57faU790e.h
cat 視訊.h | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > U8996U8a0a.h
cat 行爲.h | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > U884cU7232.h
cat 特征.h | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > U7279U5f81.h
cat 微信特征提取器.cpp | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > 微信特征提取器U.cpp
cat 微信測試特征提取器.cpp | perl -pe 'BEGIN { binmode STDIN, ":utf8"; } s/(.)/ord($1) < 128 ? $1 : sprintf("U%x", ord($1))/ge;' > 微信測試特征提取器U.cpp

g++ -Wall -O3 -o 微信特征提取器 微信特征提取器U.cpp -lpthread
g++ -Wall -O3 -o 微信測試特征提取器 微信測試特征提取器U.cpp -lpthread

