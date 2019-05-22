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
```c
//head_1.h

#include "head_2.h"

int head_1;
```

```bash
$ gcc -E hello.c
...
./head_1.h:1:10: error: #include nested too deeply
#include "head_2.h"
...
```

我们会发现在许多次迭代的引用之后，gcc编译器给我们报出来一个错误，说#include层次太深，那么我们该怎么避免这种互相include所造成的迭代include的问题呢？

C语言给我们提供了一个简单的方法来避免头文件被重复include
现在我们在head_1.h和head_2.h文件的开始加上一条预处理指令`#pragma once`,
这条指令会告诉编译器，如果这个头文件已经被include，那就不要在去include这个头文件了，这样就避免了，上述由于互相include所造成的问题了。
```c
//head_1.h
#pragma once
#include "head_2.h"

int head_1;
```

```c
//head_2.h
#pragma once
#include "head_1.h"

int head_2;
```

解决互相include的问题还有一种方法，我们的目的是让头文件只被引用一次，那么可以借助`#ifndef`的预处理指令。
`#ifndef`是告诉编译器，如果没有定义这个宏，才处理以下代码块里面的内容，知道遇到了对应的`#endif`。
现在可以把两个头文件改成下面这样：
```c
//head_1.h
#ifndef _HEAD_1_H_
#define _HEAD_1_H_

#include "head_2.h"

int head_1;

#endif
```

```c
//head_2.h
#ifndef _HEAD_2_H_
#define _HEAD_2_H_

#include "head_1.h"

int head2;

#endif
```

用`#ifndef` `#define` `#endif`把头文件的内容包起来就可以了

上面两种方法的预处理结果都是一样的
```bash
$ gcc -E hello.c
...
# 412 "/usr/include/stdio.h" 2 3 4
# 4 "hello.c" 2
# 1 "./head_1.h" 1

# 1 "./head_2.h" 1

# 1 "./head_1.h" 1
# 4 "./head_2.h" 2

int head2;
# 5 "./head_1.h" 2

int head_1;
# 5 "hello.c" 2

int main(){
    head_1 = 123;
    printf("Hello, World!\n");
    printf("%d\n", (1 + 2));
    printf("2 * 3 == %d\n", 2 * (1 + 2));
    printf("%d\n", head_1);
    return 0;
}
```
下面我们来分析以下`#include`预处理指令的工作过程，`#include <stdio.h>`写在最前面，它复制了一堆东西到源文件里面，我们暂时不管他，就只看head_1.h和head_2.h。

在预处理器遇到`#include “head_1.h”`的时候会打开head_1.h,紧接着就会对head_1.h进行预处理，
发现没有定义`_HEAD_1_H_`这个宏，然后就继续处理ifndef中的内容。
下一行的define指令定义了`_HEAD_1_H_`这个宏，接下来又遇到了`#include "head_2.h"`,这时候先把手头的预处理head_1.h的工作放在一边，对head_2.h进行处理，前面的工作和head_1.h的预处理过程都一样，知道处理到`#include "head_1.h"`这条预处理指令的时候，按照常规操作打开并处理head_1.h的时候发现`_HEAD_1_H_`这个宏已经被定义过了，就跳过了`#ifndef`块中的内容，这样就避免了head_1.h被重复`#include`
随后，head_2.h被预处理完成，head_1.h接着被预处理，最后都添加进hello.i中。这时候预处理的过程才被执行完成。

pragma once的功能和ifndef是差不多的，但是pragma once对编译器的实现有依赖，一些老版本的编译器可能不支持这个预处理的特性，导致了程序的可移植性不强，所以很多c语言程序仍然是使用#ifndef这种方式来避免重复include的问题，不过现在大部分编译器都支持pragma once这个特性了。它和ifndef的区别就没那么明显了。

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


从上面的例子中可以看出，预处理器只是在编译之前对c语言源文件和头文件按照用户定义的规则进行了字符串处理，并没有进行其他操作。说的高级一点就叫做宏编程。

那么接下来，我们就来了解一下预处理指令的常见用法。

下面是[菜鸟教程]([http://www.runoob.com/cprogramming/c-preprocessors.html](http://www.runoob.com/cprogramming/c-preprocessors.html))中对c语言预处理器作用的简单描述。
 | 指令 | 描述 |
 |--|--|
|`#define`|定义宏|
|`#include`|包含一个源代码文件|
|`#undef`|取消已定义的宏|
|`#ifdef`|如果宏已经定义，则返回真|
|`#ifndef`|如果宏没有定义，则返回真|
|`#if`|如果给定条件为真，则编译下面代码|
|`#else`|`#if` 的替代方案|
|`#elif`|如果前面的`#if`给定条件不为真，当前条件为真，则编译下面代码|
|`#endif`|结束一个 `#if……#else` 条件编译块|
|`#error`|当遇到标准错误时，输出错误消息|
|`#pragma`|使用标准化方法，向编译器发布特殊的命令到编译器中|

### 再说#define
define指令是宏编程中的最基本的指令，用于定义一个宏，这个宏的内容可以是一个常量，可以是一段代码，甚至可以什么都没有。这个宏可以是静态的字符串，也可以是一个字符串模板（c语言支持有参数的宏）。这样就使得c语言中的宏的用法非常多样化。

* 宏常量

在进行变成实践的时候有些常量被多次使用，而且在以后可能会被修改成其他值，如果我们用hard code的方法写在代码中，那么一旦这个常量需要被改变，就需要修改代码中的很多地方，而且很有可能出现没有全部替换导致的问题，这个时候我们可以把这个常量定义成一个宏，在使用的时候直接用宏名就可以的，在需要修改的时候，也只用对一个地方进行修改即可。比如，我们在程序中需要用到圆周率PI，只需要在代码开头，或者头文件里定义`#define PI 3.14`。在其他地方直接使用PI替代3.14。如果哪天，我们觉得3.14的精度不够那也只需要改这一个地方`#define PI 3.1415`就可以了。

* 带参数的宏

如果宏只能定义成固定的字符串，那他的用法就太单一了，c语言为宏定义提供了一种更灵活的方式：宏模板。也就是带参数的宏。这个时候宏就可以当作类似一个函数来使用了。
写法是这样的
```c
// test_abs.c
#define ABS(x) ((x)<0?-(x):(x))

int main(){
	printf("%d\n", ABS(-12)*2);
}
```

这里有一点需要注意，在定义函数的时候我们可以在函数的形参列表中随便加空格，但是定义带参数的宏的时候宏名一定不能包含空格，否则就定义成无参数的宏了。`#define ABS (x)  ((x)<0?-(x):(x))`,这个样子定义出来的宏是ABS,替换宏的字符串则变成了(x)  ((x)<0?-(x):(x))。有兴趣可以自己用gcc -E试一试。

我们还是看看预处理之后这个源文件变成了什么样子：
```bash
$ gcc -E test_abs.c
...
# 412 "/usr/include/stdio.h" 2 3 4
# 2 "test_abs.c" 2

int main(){
 printf("%d\n", ((-12)<0?-(-12):(-12))*2);
}
```

可以看出，即使我们是把宏当作一个函数来使用的，但是经过预编译之后，宏还是被展开成了，它原先的样子，而且定义宏的时候宏的参数都被替换成了使用的时候的参数。

这种用法和c++中的内联函数的用法差不多，通过宏定义的方法，把小函数直接展开在使用它的地方，用这种方法来省去了函数调用本身的开销（函数调用的开销会在后面章节讲到）。

看上去很简单，宏的本质就是字符串替换，但是这里面还是有一些坑要去踩的。
现在我们把原先的test_abs.c改成下面这个样子：
```c
// test_abs.c
#include <stdio.h>

#define PRINT_ABS(x)  printf("abs(x) == %d\n", ((x)<0?-(x):(x)))
int main(){
    PRINT_ABS(-12);
}
```

我们想要让带参数的宏直接打印出“abs(-12) == 12”这种字符串，来看看预处理的结果是什么：
```bash
$ gcc -E test_abs.c
...
# 412 "/usr/include/stdio.h" 2 3 4
# 2 "test_abs.c" 2


int main(){
    printf("abs(x) == %d\n", ((-12)<0?-(-12):(-12)));
}
```

这里可以看出，字符串中的宏参数并没有被替换，打印出来的结果就变成了“abs(x) == 12”，这明显和我们的期望不一致。下面这种方法可以绕过这种限制。
```c
// test_abs.c
#include <stdio.h>

#define PRINT_ABS(x)  printf("abs("#x") == %d\n", ((x)<0?-(x):(x)))
int main(){
    PRINT_ABS(-12);
}
```

我们来看一下预处理之后的结果是什么：
```bash
$ gcc -E test_abs.c
# 412 "/usr/include/stdio.h" 2 3 4
# 2 "test_abs.c" 2


int main(){
    printf("abs(""-12"") == %d\n", ((-12)<0?-(-12):(-12)));
}
```

可能大家看到`"abs(""-12"") == %d\n"`这种样子的字符串会觉得比较奇怪，这里涉及到一个c语言的知识点：拼接字符串常量，就是三个字符串常量写在一起，编译器会自动把它拼接起来，当作一个字符串来使用。但是这里的拼接仅限于字符串常量，也就是使用双引号括起来的字符串，如果是一个char*指针是不能使用这种方式进行拼接的。

接下来，我们继续分析这段预处理的过程，我们定义的宏明明是一个int类型的常量，为什么替换的时候会给它加上一个双引号呢？

这就要说到宏定义中的`#`号的作用了，#号会默认把宏定义的值加上双引号进行替换，那么如果宏定义的参数本身就是一个字符串常量会怎么呢？
下面把代码再改成这个样子
```c
// test_abs.c
#include <stdio.h>

#define PRINT_ABS(x)  printf("abs(#x)\n")
int main(){
    PRINT_ABS("-12");
}
```

预处理
```bash
$ gcc -E test_abs.c
# 412 "/usr/include/stdio.h" 2 3 4
# 2 "test_abs.c" 2


int main(){
    printf("abs(""\"-12\""")\n");
}
```

可以看出，预处理仍然保留了宏参数的双引号，而且自动把双引号变成了转义之后的双引号，编译运行一下这个程序：
```bash
$ gcc -o test_abs test_abs.c
$ ./test_abs
abs("-12")
```

因为进行了转义，所以输出的字符串中依然保留了双引号。

说了这么多define，这里有一点要特别聊聊，
前面也说过了，宏实际上就是做了字符串的替换，但是这里做的字符串替换，和我们在编辑器里面做的replace操作还是有区别的，预处理器做的字符串替换是以c语言词法分析出来的token为基础的，而我们平常在编辑器中做的replace操作是以字符为基础的。我们举一个最简单的例子来说明这件事情。
```c
// entry_point.c
#define ENTRY_POINT main

int ENTRY_POINT(){
	return 0;
}

int ENTRY_POINT_SOMETHING(){
	return 0;
}
```

```bash
$ gcc -E entry_point.c
# 1 "entry_point.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 361 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "entry_point.c" 2

int main(){
 return 0;
}

int ENTRY_POINT_SOMETHING(){
 return 0;
}
```

可以看到只有第一个函数的ENTRY_POINT宏被替换掉了，而第二个函数没有被替换，这是因为在词法分析的过程中，第一个函数的token就是ENTRY_POINT，而第二个函数的token是ENTRY_POINT_SOMETHING和宏定义不一致，所以不会被替换。

再看带参数的宏，实际上带参数的宏被就是被替换了两次的宏，在定义的时候我不知道自己什么时候会被使用，那我就等你使用我，然后给我一个参数，再用这个参数把自己的内容替换掉，最后在用得到的内容替换掉调用的内容。

这个时候就有个问题了，如果我想要带参数的宏帮我连接一个生成一个新的字符串，用这个字符串替换代码中的宏，我又希望得到的是一个token而不是用空格分隔出来的两个宏，我该怎么办。
`#define VAR_INT(i) something`  我希望`VAR_INT(abc)`得到的是INT_abc，这种情况如果说照上面说的按照token进行替换，肯定是做不到的了。不过预处理器提供了除了空格以外的另外一种分隔token的规则##。

`##`用于分隔宏定义中的token

```c
// connect_macro.c
#define MACRO1(name, type) type name_##type##_type

#define MACRO2(name, type) type name##_##type##_type

MACRO1(Bar, int);
MACRO2(Foo, string);
```

这个源文件不是一个完整的c语言程序，但是我们只看看预处理的结果，不编译运行，不会有什么问题。

```bash
$ gcc -E connect_macro.c
# 1 "connect_macro.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 361 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "connect_macro.c" 2

int name_int_type;
string Foo_string_type;
```

第一个宏中的name没有被替换，因为在宏定义的时候第一个宏的token分别是`[type， name_， type， _type]`， 所以就只有第一个和第三个type被替换掉了，第二个token和第四个token都因为没有找到对应的宏名而没有被替换。

第二个宏的token list是`[type, name, _, type, _type]`,自己分析去吧。

`##`还有一个特性，不对分出来的token进行进一步的宏处理了，举个例子：
```c
#define ABC abc
#define MACRO3(name, type) type name##_##ABC
MACRO3(Tee, float);
```

预处理结果
```c
float Tee_ABC;
```

### 再说条件编译
条件编译可以用于定制我们希望在代码中加入什么什么特性。
是不是打上什么级别的log。
是不是在debug模式，之类的事情。

条件编译的用法比较简单，和平时编译只有一个区别，就是条件编译的条件是针对宏的。

如果是宏名存不存在就用ifdef/ifndef
如果要对宏的值进行判断，就用if/else/elif

比如现在用宏定义写一个最简单的log系统
```c
// debug.c
#define LOGGING_LEVEL 0
#define WARNING_LEVEL 1
#define ERROR_LEVEL 2

#define DEBUG_LEVEL WARNING_LEVEL

#define DEBUG(x) ((x) >= DEBUG_LEVEL)

#if DEBUG(LOGGING_LEVEL)
int logging;
#endif

#if DEBUG(WARNING_LEVEL)
int warning;
#endif

#if DEBUG(ERROR_LEVEL)
int error;
#endif
```

当前的log等级设置的是WARNING_LEVEL，也就是说只有大于这个LEVEL的logging才会被编译。

### pragma
这是一个给编译器发指令的命令，具体的用法在应用场景中说可能印象会比较深一点，所以我们晚点再聊这个。最常用的有
* pragma once
文件只编译一次

* pragma message("some message print when preprocessing")
编译时打印信息

* pragma code_seg( ["section-name"[,"section-class"] ] )
设置函数存放的段

* pragma pack(16)
设置字节对齐
