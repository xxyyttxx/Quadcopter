bug: 同时使用两种关中断方式

OS_E*_CRITICAL 用的是 中断方式3
在 pendsv 里面用的是 中断方式1

如果默认在调用(可能触发上下文切换的)系统调用的时，不在临界区中，就不会有任何问题。
否则在pengsv后到逻辑上退出临界的点，其实是允许中断的。

之前在主任务 suspend 前没有退出临界。
```
{
	OS_ENTER_CRITICAL();
	OSTaskSuspend(START_TASK_PRIO);
    OS_EXIT_CRITICAL();
}
```

但是确没有对系统造成任何影响，原因就在这里。

debug -> pendsv 使用第二种中断方式(也不可能?) or 在外围进出临界区(不可能)

只能保证不在临界区进行系统调用?
