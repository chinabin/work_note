# 0x00. 导读

# 0x01. 简介

rsync 的最大特点是会检查发送方和接收方已有的文件，仅传输有变动的部分（默认规则是文件大小或修改时间有变动）。

注意，传输的双方都必须安装 rsync。

# 0x02. 参数

```
-r: 递归到目录中去。
-a: 表示存档模式，保存所有的元数据，比如修改时间（modification time）、权限、所有者等，并且软链接也会同步过去。

-n 模拟执行结果
--delete 删除只存在于目标目录、不存在于源目录的文件。

--exclude
--include
```

## 2.1 本地

```bash
# 本机使用 rsync 命令时，可以作为cp和mv命令的替代方法，将源目录同步到目标目录。
# -r表示递归，即包含子目录。并且这个选项是必须的，否则命令会失败。
$ rsync -r source destination
# source1、source2都会被同步到destination目录。
$ rsync -r source1 source2 destination
```

```bash
# -a参数可以替代-r，除了可以递归同步以外，还可以同步元信息（比如修改时间、权限等）。
$ rsync -a source destination

# 如果只想同步源目录source里面的内容到目标目录destination，则需要在源目录后面加上斜杠。
$ $ rsync -a source/ destination
```

```bash
# -n参数模拟命令执行的结果，并不真的执行命令。-v参数则是将结果输出到终端，这样就可以看到哪些内容会被同步。
$ rsync -anv source/ destination
```

```bash
# 默认情况下，rsync 只确保源目录的所有内容（明确排除的文件除外）都复制到目标目录。它不会使两个目录保持相同，并且不会删除文件。如果要使得目标目录成为源目录的镜像副本，则必须使用--delete参数，这将删除只存在于目标目录、不存在于源目录的文件。
$ rsync -av --delete source/ destination
```

```bash
# 排除了所有 TXT 文件。
$ rsync -av --exclude='*.txt' source/ destination

# 注意，rsync 会同步以 "点" 开头的隐藏文件，如果要排除隐藏文件，可以这样写--exclude=".*"

# 排除某个目录里面的所有文件，但不希望排除目录本身，可以写成下面这样。
$ rsync -av --exclude 'dir1/*' source/ destination
```

```bash
# --include参数用来指定必须同步的文件模式，往往与--exclude结合使用。

$ rsync -av --include="*.txt" --exclude='*' source/ destination

# 上面命令指定同步时，排除所有文件，但是会包括 TXT 文件。
```

## 2.2 远程

```bash
$ rsync -av source/ username@remote_host:destination
$ rsync -av username@remote_host:source/ destination
$ rsync -av -e 'ssh -p 2234' source/ user@remote_host:/destination

```

# 0x03 增量备份
rsync 的最大特点就是它可以完成增量备份，也就是默认只复制有变动的文件。

除了源目录与目标目录直接比较，rsync 还支持使用基准目录，即将源目录与基准目录之间变动的部分，同步到目标目录。

具体做法是，第一次同步是全量备份，所有文件在基准目录里面同步一份。以后每一次同步都是增量备份，只同步源目录与基准目录之间有变动的部分，将这部分保存在一个新的目标目录。这个新的目标目录之中，也是包含所有文件，但实际上，只有那些变动过的文件是存在于该目录，其他没有变动的文件都是指向基准目录文件的硬链接。

--link-dest 参数用来指定同步时的基准目录。

```bash
$ rsync -a --delete --link-dest /compare/path /source/path /target/path

# --link-dest 参数指定基准目录/compare/path，然后源目录/source/path跟基准目录进行比较，找出变动的文件，将它们拷贝到目标目录/target/path。那些没变动的文件则会生成硬链接。这个命令的第一次备份时是全量备份，后面就都是增量备份了。
```