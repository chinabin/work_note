# 0x00. 导读

# 0x01. 简介

# 0x02. git推送本地分支到远程分支

开分支有两种方式:

- 一种是在远程开好分支,本地直接拉下来;
    ```bash
    $ git checkout -b feature-branch origin/feature-branch    //检出远程的feature-branch分支到本地
    ```
- 一种是本地开好分支,推送到远程.
    ```bash
    $  git checkout -b feature-branch    //创建并切换到分支feature-branch  
    $  git push origin feature-branch:feature-branch    //推送本地的feature-branch(冒号前面的)分支到远程origin的feature-branch(冒号后面的)分支(没有会自动创建)
    ```