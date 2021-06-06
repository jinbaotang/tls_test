# tls_test 

## 1. 建立自己的CA
在OpenSSL的安装目录下的misc目录下，运行脚本sudo ./CA.sh -newCA。运行完后会生成一个demoCA的目录，里面存放了CA的证书和私钥。
拷贝至自己用户目录下，使用所有者是root，则用``sudo chown [user] [filename]``改成用户，方便操作
## 2. 生成客户端和服务器证书申请
openssl req -newkey rsa:2048 -out req1.pem -keyout sslclientkey.pem
openssl req -newkey rsa:2048 -out req2.pem -keyout sslserverkey.pem
## 3. 签发客户端和服务器证书
openssl ca -in req1.pem -out sslclientcert.pem
openssl ca -in req2.pem -out sslservercert.pem

## 4. 运行

```
git clone git@github.com:jinbaotang/tls_test.git
cd tls_test
mkdir build
cd build 
cmake ..
make
./tls_test ../sslservercert.pem ../sslserverkey.pem
./tls_test_client 127.0.0.1 7838
```