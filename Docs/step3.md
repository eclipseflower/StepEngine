1. 外界需要统一创建application的入口
2. 不同平台application创建，所需参数不同，需要wrap
3. 创建application的接口返回为基类，但实际的创建接口返回应为具体平台类
4. wndproc必须为static方法，为了调用实际的处理逻辑，需要一个global application

https://zhuanlan.zhihu.com/p/29127264
https://forums.unrealengine.com/t/cant-debug-unreal-engine-source-code/325539
https://blog.csdn.net/xuyongqi/article/details/117227528