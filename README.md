# BIT网关自动登录脚本-C++版本（可注册服务）
## 1. 介绍
BIT网关自动登录脚本-C++版本（可注册服务）是一个基于C++的BIT网关（10.0.0.55）自动登录脚本，可以实现自动登录BIT网关，支持注册服务，可以开机自动登录。

本脚本根据沈哥python版本的脚本改写而来，原脚本地址：（哦，他没有开源，所以没有地址）。所有下载脚本的人，默念一句谢谢沈哥吧，我只是脚本的搬运工\doge
## 2. 使用说明
以管理员权限打开cmd命令行，输入以下命令（学号和密码记得改成自己的）：
```
sc.exe create BITAutoLogin binpath="\"C:\xxx\BITAutoLogin.exe\" 学号 密码" start=auto
```
如果提示[SC] CreateService 成功，则说明注册服务成功，可以在服务中看到BITAutoLogin服务，可以设置为自动启动。
## 3. 作者
[vict0rShen](https://github.com/vict0rShen)
