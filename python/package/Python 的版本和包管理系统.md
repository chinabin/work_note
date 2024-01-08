# 0x00. 导读

版本管理
- pyenv: Simple Python version management
- Conda

虚拟环境管理
- pyenv-virtualenv
- Poetry
- Conda

包依赖管理
- Poetry
- Conda

# 0x01. 简介

# 0x02. 

## 2.1 版本管理

版本管理主要是用来管理同一个机器上的不同的 Python 解释器，每一个版本的解释器都可以产生独立的虚拟环境和包依赖。

推荐使用 pyenv 

## 2.2 虚拟环境管理

虚拟环境其实就是某一个 Python 版本 + 定制化的依赖。

比如 env-1 是 Python 3.11 + numpy + pandas，而 env-2 是 Python 3.12 + networkx，而 env-3 是 pypy 3.10 + numpy + pandas。这其中 env-1, env-2, env-3 就是三个完全独立的 Python 执行环境。他们有不同版本的 Python 解释器，安装有不同的包，而且这些包的版本可能也是不一样的。

为什么需要不同的环境呢？其中一个比较重要用途就是确保包的版本不会存在冲突，因为一般每一个 Python 项目的依赖都不尽相同，如果要开发不同的项目，就需要不同的虚拟环境。另外，如果每个项目有独立的虚拟化环境配置，在打包 Docker 运行的时候，也会非常轻松。

推荐使用 pyenv-virtualenv

## 2.3 包依赖管理

Python 有几个主要的包源：

- [PyPI](https://pypi.org/) Python Package Index
- [Conda](https://docs.conda.io/en/latest/) Package, dependency and environment management for any language
- [Conda-Forge](https://conda-forge.org/)

目前 PyPI 还是最主要的 Python 库的源，而 Conda 则更加适合机器学习、数据科学的库，或者一些需要混合编程场景，比如 C++，CUDA 这种。

相应的 pip 和 conda 是两个主要的依赖管理器。需要指出，pip 在依赖管理方面非常弱，可能会出现冲突。

推荐使用 Poetry