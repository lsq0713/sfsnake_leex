# 修改日志

## 版本 1

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

## 版本 2

### fix

运行后发现存在bug，水果将一直生成在同一行。在GameScreen.cpp中修改相应代码

```c++
// GameScreen.cpp
- static std::default_random_engine engine;
- engine.seed(time(NULL));
+ static std::default_random_engine engine(time(NULL));
```

修复后不再生成在同一行！

### fix 2

注意到每次调用`setColor`时均存在warning，查阅得目前setColor已被建议修改为`setFillColor`，故用其进行替换

## 鼠标控制逻辑

在各类中进行了一番修改以增加鼠标控制逻辑

将assets中的pickup.tiff转码为pickup.wav以正确播放

编译成功，然而开始游戏后立刻会跳入死亡界面，经过debug发现一旦开始游戏就会触发`hitself`

经过调试，发现`move`逻辑中头节点移动太慢，会被判定与下一节点重叠，将头节点的移动速度在一定程度上加快，成功运行

目前问题：

- 水果的种类还未完成，吃水果增长太多
- 转弯角度限制太大，鼠标操纵时容易自杀

通过修改自碰撞逻辑改善了转弯角度：只检测碰撞到第五个节点及之后的节点才算自杀

```c++
void Snake::checkSelfCollisions()
{
   SnakeNode &headNode = nodes_[0];
   if (nodes_.size() < 5)
   {
      dieSound_.play();
      sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
      hitSelf_ = true;
   } // 此处逻辑是为了后续加入减少长度机制设计的，同时增强健壮性，保证总有死亡可能
   for (decltype(nodes_.size()) i = 5; i < nodes_.size(); ++i)
   {
      if (headNode.getBounds().intersects(nodes_[i].getBounds()))
      {
         dieSound_.play();
         sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
         hitSelf_ = true;
      }
   }
}
```

## 版本 3

> 完成了对水果种类的增加，分数及出现概率的差异化
> 注释掉部分不需要的调试语句
> 由于背景还未改变，黑色暂时用紫色代替 TODO

对`Fruit`类增加了一个初始化方法

```c++
Fruit.h
+Fruit(sf::Vector2f position = sf::Vector2f(0, 0), int FruitType = 0);
```

```c++
Fruit.cpp
Fruit::Fruit(sf::Vector2f position, int FruitType)
{
   shape_.setPosition(position);
   shape_.setRadius(Fruit::Radius);
   if (FruitType < 0)
   {
      FruitType = -FruitType;
   }
   FruitType = FruitType % 8;
   if (FruitType < 3)
   {
      shape_.setFillColor(sf::Color::Green); // Green
      score_ = 1;
   }
   else if (FruitType < 5)
   {
      shape_.setFillColor(sf::Color::Blue);  // Blue
      score_ = 2;
   }
   else if (FruitType < 6)
   {
      shape_.setFillColor(sf::Color::Red);   // Red
      score_ = 1;
   }
   else if (FruitType < 7)
   {
      shape_.setFillColor(sf::Color(0x935116ee));// Brown
      score_ = 0;
   }
   else
   {
      shape_.setFillColor(sf::Color(0xe00aeaee));// Black // TODO
      score_ = 0;
   }
   // printf("x: %f, y: %f, color: %d\n", position.x, position.y, FruitType);
}
```

同时在`GameScreen.cpp`中传入一个整数作为第二个参数

不同水果的出现概率：

- 黑色（目前为紫色）12.5%
- 棕色 12.5%
- 红色 12.5%
- 蓝色 25%
- 绿色 37.5%

## 版本 4

> 对贪吃蛇进行重绘，用精灵进行渲染，使其更美观
> 目前贪吃蛇移动速度过快（界面太小）
> 对吃水果的范围判定不够，导致很难吃到水果

在Snake类的render方法中，更改了原来调用`SnakeNode::render`对每个节点进行渲染的模式，采用直接在`Snake::render`中直接对头部节点和关节节点进行渲染

```c++
void Snake::render(sf::RenderWindow &window)
{
   headSprite.setPosition(nodes_[0].getPosition());
   window.draw(headSprite);
   static float angle;
   static sf::Vector2f recDirection;
   recDirection = direction_;
   angle =
      std::acos(recDirection.y / length(recDirection)) /
      3.14159265358979323846 * 180.0;
   if (direction_.x > 0)
      angle = -angle;
   headSprite.setRotation(angle);

   for (int i = 1; i < nodes_.size(); i++)
   {
      shape_.setPosition(nodes_[i].getPosition());
      Direction idirection = nodes_[i].getDirection();
      recDirection = idirection;
      angle =
         std::acos(recDirection.y / length(recDirection)) /
         3.14159265358979323846 * 180.0;
      if (idirection.x > 0)
         angle = -angle;
      shape_.setRotation(angle);
      window.draw(shape_);
   }
}
```

对于`SnakeNode`类也进行了优化：

由于类中的`render`在上述修改后已弃用，目前类中重要的是对节点数据的储存：

- position_：位置
- direction_：方向

> 或许可以在此对节点的bound定义来修改吃水果范围太小的问题

### fix 水果pickup

在`SnakeNode`中的shape_以`Circleshape`类型存在，去除了原先的渲染功能，只用来作为实体判定，以正确地pickup水果

### fix Game::run

将`render()`函数移到内循环，以减少资源浪费

同时删除了一些无用的注释

### feat 背景颜色以及网格

在Game.*中增加了控制网格的相关参数

```c++
const sf::Color GridColors[] =
      {
         sf::Color(0xfbfbfbaa), // White
         sf::Color(0x31313116), // Black
         sf::Color(0x5a351d16)  // Brown
   };
   const sf::Color BgColors[] =
    {
        sf::Color(0xeaeaeaee), // White
        sf::Color(0x747474ff), // Black
        sf::Color(0x9f6133aa)  // Brown
};
static bool GridVisibility; // 网格可见性
static int GridColor;// 网格颜色
static int BgColor;// 背景颜色
```

## version 5 颜色改变

通过对`MenuScreen`中处理输入的函数进行修改，与前面完成的改变接口相接，完成了对背景与网格控制的逻辑

但是对于首页的显示处理的还不完善，无法让用户直观的控制。

相关配色也不够和谐

### fix 菜单页面处理

- 优化了屏幕大小
- 优化了菜单页面处理，加入了鼠标锁以及键盘锁
- 将水果生成数量修改为6个

但是还无法正确显示分数

### fix 分数显示

可以正确显示分数
