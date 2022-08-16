# c++ 内存模型

## 1_cpu_reorder

用于证明CPU的乱序发射,指令重排;

## 2_atomic_relaxed

用于证明memory_relaxed不能防止CPU重排

## 3_mat_cal

用于展示多线程使用原子操作比加锁解锁效率高

## 4_atomic_acq_rel

用于展示acq和rel操作实现同步

## 5_double_rel_acq

用于展示release和acquire次序不能唯一操作顺序

## 6_sync_acq_rel

用于展示使用acq_rel次序实现同步传递

## 7_fence

用于展示内存屏障的用法