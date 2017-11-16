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
  char fpath[1000];char ganti[1000], nama[1000],nya[1000];char aa[100];char mk[100];char pindah[1000];
	   sprintf(fpath, "%s%s",dirpath,path);
 
	 int len=strlen(fpath);
	int i;
	for(i=0;i<len;i++){
		if(fpath[i]=='.'){
			if(fpath[i+1]=='p' && fpath[i+2]=='d' && fpath[i+3]=='f'){ 
			 aa[0]='.'; aa[1]='p'; aa[2]='d';aa[3]='f';
					}

			else if(fpath[i+1]=='d' && fpath[i+2]=='o' && fpath[i+3]=='c' ){ 
			 aa[0]='.'; aa[1]='d'; aa[2]='o';aa[3]='c';
					}

			else if(fpath[i+1]=='t' && fpath[i+2]=='x' && fpath[i+3]=='t' ){ 
			 aa[0]='.'; aa[1]='t'; aa[2]='x';aa[3]='t';
					}
			}
	}

	if(strcmp(aa,".doc")==0||strcmp(aa,".pdf")==0||strcmp(aa,".txt")==0){
		system("zenity --error --text=\"Terjadi Kesalahan! File berisi konten berbahaya.\n\" --title=\"Warning!\"");
		 
		sprintf(ganti,"%s",fpath);
		sprintf(nama,"%s.ditandai",fpath);
		rename(ganti,nama);

		sprintf(nya,"chmod 000 %s.ditandai",fpath);
		system(nya);
	  
	 
		sprintf(mk,"mkdir /home/stark/Documents/rahasia");
		system(mk);
		sprintf(pindah,"mv /home/stark/Documents/*.ditandai /home/stark/Documents/rahasia/");
		system(pindah);
		}
return -errno;
 
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
