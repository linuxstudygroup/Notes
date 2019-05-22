# x86系列芯片架构

## 三种模式
x86架构的芯片可以运行在三种模式上：保护模式、实地址模式、系统管理模式。这些模式决定了可以使用那些指令集和芯片特性。

1. 保护模式（protected mode）
保护模式是芯片最常用的模式。
这种模式可以让应用程序运行在一个看上去是实地址模式的多任务环境。相当于是给每个应用程序分配了一个沙盒。
在应用程序看来，它能访问到地址空间的所有内存地址。
这种特性又被称作虚拟8086模式，但实际上，虚拟8086模式不是一个真正的处理器模式，它只是保护模式给应用程序提供的一种特性。

2. 实地址模式（real-address mode）
实地址模式下，处理器可以访问到真实的内存地址，可以理解为是对传统8086处理器的一种拓展。
这种模式通常是用于切换保护模式和系统管理模式的。
在上电或重启之后，处理器都是运行在实地址模式下的。

3. 系统管理模式（system management mode， SMM）
该模式为操作系统或执行程序提供了透明的机制，用于实现特定于平台的功能，如电源管理和系统安全性。
当外部SMM中断引脚(SMI#)被激活或APIC（advanced programmable interrupt controller， 高级可编程中断控制器）接收SMI（system management interrupt，系统管理中断）时，处理器进入系统管理模式。
在系统管理模式下，处理器切换到一个单独的地址空间，同时保存当前运行的程序或任务的上下文，然后执行系统管理模式专用的代码。
在从返回时，处理器会还原进入系统管理模式之前的上下文。


OPTIONS_GROUP_NAME = "Splunk test orca Options"

def pytest_addoption(parser):
    """
    This is a pytest hook to add options from the command line so that
    we can use it later.
    """
    splk_orca_test_group = parser.getgroup(OPTIONS_GROUP_NAME)
    splk_orca_test_group.addoption("--deployment-file",
                                   action="store",
                                   metavar="NAME",
                                   default=None,
                                   help="The orca deployment file path")

option = pytest.config._parser.getgroup(OPTIONS_GROUP_NAME).options
deployment_file = getattr(option, "deployment_file")
