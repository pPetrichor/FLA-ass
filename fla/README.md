1.进入`turing-project`目录下，键入`make`指令即可生成可执行文件`turing`；在命令行中输入`./turing ……`即可执行；执行后`make clean`即可删除`turing`;

2.执行采用相对路径，`case1.tm`与`case2.tm`在`programs`目录下；因此要执行`case1.tm`与`case2.tm`，输入命令应该类似于`./turing -v ../programs/case2.tm 11x1=11`；

3.对于输出到`stdout`的内容可以利用`> xxx`进行重定向；对于输出到`stderr`的内容可以利用`&> xxx`进行重定向；