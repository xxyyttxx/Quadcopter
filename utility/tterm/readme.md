# 一个简单的读写串口的命令行程序
# 编译前务必修改makefile的预定义宏
# 编译 make
# 运行 ./tterm

[运行截图](https://github.com/xxyyttxx/Quadcopter/raw/master/utility/tterm/run-shot.png)
上面是mac上比较有名的串口收发程序CoolTerm，下面是用调用POSIX-termios接口自己实现的串口通信程序tterm。
