# 0x00. 导读

# 0x01. 简介

# 0x02. 

```bash
# 断点续传
wget -c http://www.jsdig.com/testfile.zip

# 后台下载
wget -b http://www.jsdig.com/testfile.zip

# 限速下载
wget --limit-rate=300k http://www.jsdig.com/testfile.zip

wget -O wordpress.zip http://www.jsdig.com/download.aspx?id=1080

# 下载多个文件，filelist.txt 里面是链接
wget -i filelist.txt

# FTP下载
wget ftp-url
wget --ftp-user=USERNAME --ftp-password=PASSWORD url
```