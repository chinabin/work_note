# 0x00. 导读

# 0x01. 简介

子模块允许你将一个 Git 仓库作为另一个 Git 仓库的子目录。 它能让你将另一个仓库克隆到自己的项目中，同时还保持提交的独立。

# 0x02. 命令

按需clone submodule
1. 先 git clone 主项目仓库并进入主项目文件夹，这时候submodule的文件夹都是空的。
2. 执行git submodule init [submodule的文件夹的相对路径]。
3. 执行git submodule update [submodule的文件夹的相对路径]。

第2、3步可以合并。使用以下命令：
```
git submodule update --init [submodule的文件夹的相对路径]
```
注意顺序，--init 跟 [submodule的文件夹的相对路径] 的位置不可以调换。

`git submodule init [<submodule_name> ...]` 命令初始化子仓库，它的作用是将 .gitmodules 文件中的子模块信息拷贝到当前仓库的 ./.git/config 配置文件中，乍一看这一步骤似乎是多余的，因为 .gitmodules 已经包含了子模块的信息，为什么还要多此一举呢？假设当前仓库包含了很多子模块，但是此时你开发的工作并不需要全部的子模块代码，所以你可以指定初始化你需要的子模块，然后再拉取代码。

---

一次性 clone 所有 submodule
1. 先git clone 主项目仓库，这时候 submodule 的文件夹都是空的。
2. 执行git submodule init。
3. 执行git submodule update。

只要不写 submodule ，那么就一次性检查该主项目的所有 submodule ，都拉下来。

合并第2、3步骤
```
git submodule update --init
```

合并第1、2、3步骤
```
git clone --recurse-submodules [主项目Git仓库地址]
```

## 2.1 添加子模块

```bash
$ git submodule add https://github.com/iphysresearch/GWToolkit.git GWToolkit

# 如果你是旧版 Git 的话，你会发现 ./GWToolkit 目录中是空的，你还需要在执行一步「更新子模块」，才可以把远程仓库项目中的内容下载下来。

$ git submodule update --init --recursive
```

添加子模块后，若运行 git status，可以看到主仓库目录中会增加一个文件 `.gitmodules` ，这个文件**保存了项目 URL 和你拉取到的本地子目录**。
```
[submodule "external/uganda"]
path = external/uganda
url = ssh://git@192.168.1.100:7999/util/uganda.git
[submodule "external/thunder"]
path = external/thunder
url = ssh://git@192.168.1.100:7999/aeg/thunder.git
[submodule "external/yaml-cpp"]
path = external/yaml-cpp
url = ssh://git@192.168.1.100:7999/util/yaml-cpp.git
```

另外，在 `.git/config` 中会多出一块关于子模块信息的内容：

```
[submodule "GWToolkit"]
    url = https://github.com/iphysresearch/GWToolkit.git
    active = true
```

该配置文件**保存了项目 URL 与已经拉取的本地目录之间的映射**。如果有多个子模块，该文件中就会有多条记录。 要重点注意的是，该文件也像 .gitignore 文件一样受到（通过）版本控制。 它会和该项目的其他部分一同被拉取推送。 这就是克隆该项目的人知道去哪获得子模块的原因。

新生成的还有相关子模块的文件：``.git/modules/GWToolkit/`


在终端 Git 命令操作下，位于主仓库目录中除了子模块外的任何子目录下进行的 commit 操作，都会记到主仓库下。在子模块目录内的任何 commit 操作，会记到子模块仓库下。

## 2.2 查看子模块

```
$ git submodule
```

## 2.3 更新子模块

主项目保存了对应 submodule 的版本号（commit id），当子模块有新的更新，就要有两步：1. 子模块 git pull --rebase. 2. 主模块更新自己记录的子模块 commit 到最新的。

假设主项目记录子项目的 commit id 为 A ，子项目远程最新的 commit id 为 B 

子项目用 A：
```bash
$ git submodule update [submodule文件夹相对路径]
# 注意，这个方法会使 submodule 的分支处于主项目里指定的 commit id。可能并不是拉 submodule 的 master 最新代码。
# 所以，这种方法仅适用于，当主仓库里记录的 submodule 的 commit id 已经是最新的（可能被其他同事提交过）。或者你期望 submodule 跟主仓库记录的保持一致时，也可以使用该方法。
```

子项目拉取到最新的提交，并且用上用上最新的提交，也就是用 B
```bash
$ git submodule update --remote [submodule文件夹相对路径]
# 这个方法会自动拉取 submodule 的主分支的最新版本。
# 如果不带参数 [submodule文件夹相对路径]，就会更新所有 submodules。

# 注意：更新后需提交主项目变更。
# 当我们更新子项目后，相当于是把主项目记录的 submodule 的 commit id 给更新了，需要提交下主项目的变更。
```

## 2.x 删除子模块

删除子模块比较麻烦，需要手动删除相关的文件，否则在添加子模块时有可能出现错误 同样以删除 GWToolkit 子模块仓库文件夹为例：

1. 删除子模块文件夹
```
$ git rm --cached GWToolkit
$ rm -rf GWToolkit
```

2. 删除 .gitmodules 文件中相关子模块的信息，类似于：
```
[submodule "GWToolkit"]
        path = GWToolkit
        url = https://github.com/iphysresearch/GWToolkit.git
```

3. 删除 .git/config 中相关子模块信息，类似于：
```
[submodule "GWToolkit"]
        url = https://github.com/iphysresearch/GWToolkit.git
        active = true
```

4. 删除 .git 文件夹中的相关子模块文件
```
$ rm -rf .git/modules/GWToolkit
```



# 工作经验

1. 主仓库项目的合作者并不会自动地看到子模块仓库的更新通知的。所以，更新子模块后一定要记得提醒一下主仓库项目的合作者 git submodule update

2. 如果父项目中子模块的仓库地址（submodule.xxx.url）被其他协作者修改了，那么我们再更新子模块时就可能遇到问题，需要执行：

    将新的 URL 复制到本地配置中  
    `git submodule sync --recursive`  
    同步完 url，然后再重新初始化更新：

    `git submodule update --int --recursive`

3. `git submodule foreach 'git stash'`