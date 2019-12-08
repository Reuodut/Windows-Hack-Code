记录通过管理员权限获取system权限的方法：
1、通过父进程（becoming-system-via-parent）
CreateProcessA函数允许用户创建新进程，默认情况下，会通过其继承的父进程完成创建。该函数有一个名为“lpStartupInfo”的参数，该参数允许使用者自定义要使用的父进程。该功能最初用于Windows Vista中设置UAC。
lpStartupInfo参数指向一个名为“STARTUPINFOEX”的结构体，该结构包含变量“lpAttributeList”，这个变量在初始化时可以调用“UpdateProcThreadAttribute”回调函数进行属性添加，你可以通过“PROC_THREAD_ATTRIBUTE_PARENT_PROCESS”属性从而对父进程进行设置。
参考：https://blog.xpnsec.com/becoming-system/

2、待续