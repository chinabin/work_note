-O3 (enables machine-independent optimizations)  
-march (enables optimizations for particular
CPU generation)  
-flto (enables inter-procedural optimizations)

```bash
# 会列出可选项
g++ -march=native -m32 ... -Q --help=target 

# 显示 O3 用到了哪些优化
gcc -Q -O3 --help=optimizers` 
```