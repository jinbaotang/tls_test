# tls_test 

## 1. 生成服务器证书申请
openssl genrsa -out test.key 2048

## 2. 签发客服务器证书
openssl req -new -x509 -key test.key -out test.dvc -days 1095

## 4. 运行

```
git clone git@github.com:jinbaotang/tls_test.git
cd tls_test
mkdir build
cd build 
cmake ..
make
./tls_test ../test.dvc ../test.key
./tls_test_client 127.0.0.1 7838
```