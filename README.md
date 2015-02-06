# cocos2dx-flatbuffers-demo
using flatbuffers demo with the lib from cocos2d-x

###flatbuffers的介绍和使用
由于cocos2d-x 3.0已经集成了flatbuffers，这里直接使用cocos2d-x中库编写了使用的demo

Flatbuffers的介绍自行google，下面的介绍请配合代码阅读。

这里简单介绍下使用方法
首先是编写scheme文件
基本的数据类型就是常见int,float,double,string以及vector（使用[int]表示）
可以使用struct和table自定义数据类型
对于string,struct,table都是特殊的数据类型，在序列化的时候需要使用fbb来创建。
另外struct和table的区别是，struct是简单的数据类型，不需要偏移，但是缺陷是不支持兼容处理，table支持向下兼容。

SaveFile这个方法在util.h中，使用这个方法可以保持序列化内容到本地。

