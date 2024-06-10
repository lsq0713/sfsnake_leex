对原版的文件进行一定的组织：

- 将代码放入`src/`目录下
- 将声音、图片、字体放入`assets/`目录下(*在CMakeLists.txt文件中将它们加载到程序中*)
- 添加了`CMakeLists.txt`文件用于构建项目

在根目录下执行命令`cmake .`，成功生成make文件

执行`make`，出现报错

``` bash
Consolidate compiler generated dependencies of target main
[ 10%] Building CXX object CMakeFiles/main.dir/src/Game.cpp.o
In file included from /home/lee/workspace/OOP/pj/sfSnake-master/src/Game.cpp:6:
/home/lee/workspace/OOP/pj/sfSnake-master/src/Game.h:28:40: error: declaration of ‘std::shared_ptr<Screen> sfSnake::Game::Screen’ changes meaning of ‘Screen’ [-fpermissive]
   28 |         static std::shared_ptr<Screen> Screen;
      |                                        ^~~~~~
In file included from /home/lee/workspace/OOP/pj/sfSnake-master/src/MenuScreen.h:6,
                 from /home/lee/workspace/OOP/pj/sfSnake-master/src/Game.cpp:5:
/home/lee/workspace/OOP/pj/sfSnake-master/src/Screen.h:6:7: note: ‘Screen’ declared here as ‘class Screen’
    6 | class Screen
      |       ^~~~~~
make[2]: *** [CMakeFiles/main.dir/build.make:90: CMakeFiles/main.dir/src/Game.cpp.o] Error 1
make[1]: *** [CMakeFiles/Makefile2:83: CMakeFiles/main.dir/all] Error 2
make: *** [Makefile:91: all] Error 2
```

通过分析报错可知，代码中存在一个命名冲突。在 Game.h 文件中，有一个名为 Screen 的静态成员变量，但是在 Screen.h 文件中也有一个名为 Screen 的类。这导致了编译器无法区分到底应该使用哪个 Screen。

解决措施：将原Game类中的Screen成员重命名为GameScreen

> 其实这里也可以通过命名空间的方法进行区分，但为了避免混淆选择此方法

修改之后重新构建，出现以下报错：

``` bash
/home/lee/workspace/OOP/pj/sfSnake-master/src/Snake.cpp: In member function ‘void sfSnake::Snake::checkFruitCollisions(std::vector<sfSnake::Fruit>&)’:
/home/lee/workspace/OOP/pj/sfSnake-master/src/Snake.cpp:62:37: error: cannot bind non-const lvalue reference of type ‘__gnu_cxx::__normal_iterator<sfSnake::Fruit*, std::vector<sfSnake::Fruit> >&’ to an rvalue of type ‘std::vector<sfSnake::Fruit>::iterator’
   62 |         for (auto& it = fruits.begin(); it != fruits.end(); ++it)
      |                         ~~~~~~~~~~~~^~
```

源代码部分如下

``` cpp
for (auto& it = fruits.begin(); it != fruits.end(); ++it)
{
        if (it->getBounds().intersects(nodes_[0].getBounds()))
            toRemove = it;
}
```

这个报错是因为在代码中，在 for 循环中尝试将 fruits.begin() 的返回值（std::vector<sfSnake::Fruit>::iterator 类型）绑定到一个非常量左值引用 auto& it 上，这是不允许的。

解决方法：将 auto& it 改为 auto it，这样就不再尝试将迭代器绑定到一个非常量引用上了。经过重新编译，编译成功