

#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include<stdlib.h>

static const char *dirpath = "/home/stark/Documents";
static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;

char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
  int len=strlen(fpath);
	int i;
	for(i=0;i<len;i++){
			if(fpath[i]=='.' &&fpath[i+1]=='c' && fpath[i+2]=='o' && fpath[i+3]=='p'&& fpath[i+3]=='y'){ 
				char nya[1000];
				 sprintf(nya,"chmod 000 %s.copy",fpath);
				 system(nya);
				system("zenity --error --text=\"File yang anda buka adalah file hasil salinan. File tidak bisa diubah maupun disalin kembali!\n\" --title=\"ERROR!\"");

					}
	}

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}
static int xmp_rename(const char *from, const char *to)
{
	int res;
 
	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
		 
	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000];
	sprintf(fpath,"%s%s",dirpath,path);
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.rename		= xmp_rename,

	.mkdir		= xmp_mkdir,.open		= xmp_open,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
