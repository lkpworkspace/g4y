# FAQs

Q: 不能在GObj构造函数中添加默认组件?
A: 由于组件在初始化的时候需要获得GObj的指针，但是GObj构造正在构造，智能指针没有有效值，造成的鸡和蛋的问题。。。

Q: 注意weak_ptr在指向的引用销毁时，调用lock会返回新创建一个对象返回，并不会返回空指针，这会在后续程序调试中造成问题
A: 需要判断weak_ptr是否有引用再进行下一步操作

# 调试遇到的BUG

Q: Gobj如何获得Scene引用？
A: 设置一个全局的scene对象。

Q: 在程序退出时，GObj访问GScene成员错误？
A: 程序退出先析构GScene，然后GObj拥有的GScene的弱指针已经无效，再调用lock()，则会返回一个空的GScene，调用前先判断是否是可用指针，再调用其成员函数。

Q: 在Update中添加的Obj需要在下一帧再加入世界吗?
A: 需要，如果直接添加到世界中，会在未调用awake的情况下直接调用update函数导致不可预期的错误。（暂时未实现，目前还未涉及到动态删除增加游戏对象和组件）

Q: TCP发送快，接收慢的问题?
A: 经调试发现，是程序有问题，每次程序只处理一条消息，有多条消息就会没帧产生多个延迟，造成程序接收消息慢的假象。

Q: 先加入服务端的客户端收不到后加入客户端的消息？
A: 在GMsgNetIO并没有通知GSrvMsgMgr新客户端加入，导致的。

Q: 每帧都调用SetScale会出现异常？
A: 两个物体使用同一个脚本，父物体更新缩放会递归的更新子物体缩放因子，然而子物体脚本每次都会在每帧设置脚本中的缩放因子导致子物体并没有变化。

Q: 正确删除游戏物体或组件？
A： 组件： 存储删除队列中，在一帧结束后调用删除回调函数并删除； 游戏物体： 存储到删除队列中，在一帧结束后递归调用删除回调函数， 并递归删除游戏组件游戏物体。
      添加 将要删除标记(删除后不再调用其它回调函数) 已经删除标志(避免递归重复删除时用到)

# OpenGL笔记

Q: opengl绘制图元的类型由那些?
A: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_QUAD_STRIP, GL_QUADS, and GL_POLYGON

Q: 绘制三角形线框和恢复
A: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)， glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

Q: 都有那些光源
A: 环境(Ambient)、漫反射(Diffuse)和镜面(Specular)光照

Q: 光照参数设置
A: 环境光:   最终对象颜色 = (光照强度 * 光的颜色) * 对象颜色
   漫反射:   
   镜面反射: 

# Msg笔记
