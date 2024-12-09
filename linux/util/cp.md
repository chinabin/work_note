# 0x00. 导读

# 0x01. 简介

# 0x02. 

```
-a：此选项通常在复制目录时使用，它保留链接、文件属性，并复制目录下的所有内容。
```

```bash
# 跳过目录
$ ls da_stk_radar/ | grep -v "CoreLog\|log" | xargs -i cp -r da_stk_radar/{} /opt/share/tanb/bigbang/alfred/da_stk_radar/

# 注意通配符
$ ls skyroad/ | grep -v "skyroad_td.2023*\|sr_log_raw*"
```

for file in SRMUYdDceFut SkyRoadTool SkyRoadClient ThunderClient; do cp "$file" "$file.20241205"; done