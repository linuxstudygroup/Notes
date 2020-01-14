# CLI

清除中断标志

清除EFLAGS中的IF（interrupt flag），其他标志不受影响。

清除IF标志会导致处理器屏蔽中断请求。这个时候除了NMI中断，处理器不会陷入中断服务程序。

# STI

设置中断标志

处理器开始接受可屏蔽中断请求，这个时候如果有中断产生，处理器是会进入相应的中断服务子程序的。

CLI和STI需要配合使用，它们两个之间的代码段相当于是原子操作。