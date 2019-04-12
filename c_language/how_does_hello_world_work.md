# Hello World是怎么运行的

![hello world workflow](resource/helloworld_workflow.png)

## 预处理

现在写一个简单的hello world的程序，借此来分析以下c语言预处理的过程。
```c
// hello.c

#include<stdio.h>

#define GREET "Hello, World!\n"

// entry point of main method
int main(){
    printf(GREET);
    return 0;
}
```

接着我们用gcc预处理参数来对这个文件进行预处理，看看结果：

```bash
$ gcc -E hello.c
# 1 "hello.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 361 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "hello.c" 2

# 1 "/usr/include/stdio.h" 1 3 4
...
...
...
# 412 "/usr/include/stdio.h" 2 3 4
# 4 "hello.c" 2

int main(){
    printf("Hello, World!\n");
    printf("%d\n", 1 + 2);
    return 0;
}
```

可以从上面的实验结果可以看出，c语言的预编译器干了三件事情：
1. 删除注释注释删掉了。
2. 将#include<stdio.h>迭代替换成stdio.h文件的内容。
3. 把代码中的宏按照他原本的字符串替换在了代码中。

注释这里没什么好说的，我们先来聊聊#include预处理指令

### include
上面的#include<stdio.h>，因为stdio.h文件及其#include的文件太长，所以有一堆不知所云的代码被添加了进来，现在我们加一个自己定义的头文件`head_1.h`，这个头文件中定义了一个全局变量head_1。并在#include<stdio.h>之后加一行`#include "head_1.h"`。

在进行实验之前，这里有一点需要注意，#include语句后面跟的头文件有两种写法，一个用尖括号，一个用双引号。
这两种写法主要是为了区分找头文件的顺序。
尖括号是说让预处理器现在系统默认的库路径下找头文件，用尖括号是找不到用户自定义的头文件的。
而双引号的写法则是告诉编译器，这个文件现在当前的路径找和环境变量定义的路径还有-I指定的路径中去找头文件，然后才去系统路径下去找头文件。

```c
// head_1.h
int head_1;
```

预处理看看结果
```bash
$ gcc -E hello.c
...
# 412 "/usr/include/stdio.h" 2 3 4
# 4 "hello.c" 2
# 1 "./head_1.h" 1
int head_1;
# 5 "hello.c" 2

int main(){
    printf("Hello, World!\n");
    printf("%d\n", (1 + 2));
    printf("2 * 3 == %d\n", 2 * (1 + 2));
    return 0;
}
```

这里可以看到，预处理器把head_1.h的内容添加到了预处理后的文件中来。可以在hello.c里面直接对这个全局变量head_1进行操作。
```c
//hello.c
...
int main(){
	head_1 = 123;
    printf("Hello, World!\n");
    printf("%d\n", (1 + 2));
    printf("2 * 3 == %d\n", 2 * (1 + 2));
    printf("%d\n", head_1);
    return 0;
}
```

编译运行
```bash
$ gcc -o hello hello.c
$ ./hello
Hello, World!
3
2 * 3 == 6
123
```
我们可以看到，虽然没有在头文件里面定义head_1变量，但是hello.c仍然可以使用这个变量。
这里需要注意的是，在头文件中定义变量或者函数不是一个好的practice，在后面会遇到很麻烦的问题的，具体的问题等我们学到全局变量的时候再详细的讲。现在继续研究include预处理指令。

上面说到，#include了一个头文件，预处理器会迭代的把头文件里面的内容复制到源文件里面来。那么问题来了，如果我有两个头文件，head_1.h include了head_2.h，head_2.h又include了head_1.h，这个时候预处理器会怎么处理这种情况呢？

现在我们添加一个文件head_2.h,为了看到结果，我们在
```c
// head_2.h
#include "head_1.h"

int head2;
```

然后让head_1.h也include head_2.h
看看会发生什么事情
//TODO：#ifndef
//TODO：#if DEBUG




### define
这里需要注意的是，在main函数第二行可以发现，宏定义的替换是直接替换字符串，而不是结果，所以这里有两个经典的坑：

1. 运算符优先级
现在把代码改成下面这样
```c
// hello.c

#include<stdio.h>

#define GREET "Hello, World!\n"
#define NUM 1 + 2

// entry point of main method
int main(){
    printf(GREET);
    printf("%d\n", NUM);
    printf("2 * 3 == %d\n", 2 * NUM);
    return 0;
}
```

编译运行：
```bash
$ gcc -o hello hello.c
$ ./hello
Hello, World!
3
2 * 3 == 4
```

这里可以看到，我们想打印出来的2 * （1 + 2），也就是2 * 3，在c语言中的计算结果竟然编程了4，现在把预处理结果拿出来看一下，
```c
...
int main(){
    printf("Hello, World!\n");
    printf("%d\n", 1 + 2);
    printf("2 * 3 == %d\n", 2 * 1 + 2);
    return 0;
}
```
这个数字的预处理的结果变成了 2 * 1 + 2.
这里想说的是，宏定义如果包含了运算等操作，最好用圆括号把它括住，上面的宏定义应该变成`#define NUM (1 + 2)`

2. 宏定义中不能带分号

同样，我们把NUM的宏定义改成`#define NUM (1 + 2);`
再编译hello.c
```bash
gcc -o hello hello.c

hello.c:11:20: error: unexpected ';' before ')'
    printf("%d\n", NUM);

hello.c:12:33: error: unexpected ';' before ')'
    printf("2 * 3 == %d\n", 2 * NUM);
```
编译器会给报两条这样的错，不过现在的编译器都比较智能了，报错之后还能给这个错误加一条note提示你，宏定义不要加分号：
```
hello.c:6:20: note: expanded from macro 'NUM'
#define NUM (1 + 2);
                   ^
```

### 源文件和头文件
//TODO
