#include "ringbuffer.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

unsigned int rbuf_read(p_rbuf p_in,void *copy_to_buf,unsigned int bufsize);
unsigned int rbuf_cat(p_rbuf p_in,unsigned int offset,void *copy_to_buf,unsigned int bufsize);
void rbuf_out_cat(p_rbuf p_in);
unsigned int rbuf_write(p_rbuf p_in,void *from_buf,unsigned int bufsize);
unsigned int rbuf_get_free_space(p_rbuf p_in);
unsigned int rbuf_get_used_space(p_rbuf p_in);
unsigned int rbuf_get_all_space(p_rbuf p_in);
void rbuf_reset(p_rbuf p_in);
int main()
{
    //DECLARE_RINGBUFFER(my_test_rbuf,2048);//也可以使用静态定义
    p_rbuf my_test_rbuf=rbuf_alloc(2048);
    if(!my_test_rbuf)
    {   
        return 0; 
    }
    printf("write buf=%u\n",rbuf_write(my_test_rbuf,"123456",strlen("123456")));
    printf("my_test_rbuf all space=%u\n",rbuf_get_all_space(my_test_rbuf));
    printf("my_test_rbuf free space =%u\n",rbuf_get_free_space(my_test_rbuf));
    printf("my_test_rbuf used space=%u\n",rbuf_get_used_space(my_test_rbuf));
    char tmp_str[64]={0};
    printf("read rbuf=%u,",rbuf_read(my_test_rbuf,tmp_str,sizeof(tmp_str)));
    printf("data:[%s]\n",tmp_str);
    memset(tmp_str,0,sizeof(tmp_str));
    printf("===========================================================================\n");


    printf("write buf=%u\n",rbuf_write(my_test_rbuf,"abcdefghijk",strlen("abcdefghijk")));
    unsigned int test_cat=0;
    test_cat=rbuf_cat(my_test_rbuf,0,tmp_str,10);
    printf("cat rbuf=%u,",test_cat);
    printf("data:[%s]\n",tmp_str);
    memset(tmp_str,0,sizeof(tmp_str));
    printf("cat rbuf=%u,",rbuf_cat(my_test_rbuf,test_cat,tmp_str,10));
    printf("data:[%s]\n",tmp_str);
    memset(tmp_str,0,sizeof(tmp_str));
    printf("read rbuf=%u,",rbuf_read(my_test_rbuf,tmp_str,sizeof(tmp_str)));
    printf("data:[%s]\n",tmp_str);
    printf("===========================================================================\n");



    printf("write buf=%u\n",rbuf_write(my_test_rbuf,"abcdefghijk",strlen("abcdefghijk")));
     test_cat=0;
    test_cat=rbuf_cat(my_test_rbuf,0,tmp_str,10);
    printf("cat rbuf=%u,",test_cat);
     printf("data:[%s]\n",tmp_str);
    memset(tmp_str,0,sizeof(tmp_str));
    printf("cat rbuf=%u,",rbuf_cat(my_test_rbuf,test_cat,tmp_str,10));
    printf("data:[%s]\n",tmp_str);
    printf("my_test_rbuf all space=%u\n",rbuf_get_all_space(my_test_rbuf));
    printf("my_test_rbuf free space =%u\n",rbuf_get_free_space(my_test_rbuf));
    printf("my_test_rbuf used space=%u\n",rbuf_get_used_space(my_test_rbuf));
    rbuf_out_cat(my_test_rbuf);
    printf("my_test_rbuf all space=%u\n",rbuf_get_all_space(my_test_rbuf));
    printf("my_test_rbuf free space =%u\n",rbuf_get_free_space(my_test_rbuf));
    printf("my_test_rbuf used space=%u\n",rbuf_get_used_space(my_test_rbuf));
    memset(tmp_str,0,sizeof(tmp_str));
    printf("read rbuf=%u,",rbuf_read(my_test_rbuf,tmp_str,sizeof(tmp_str)));
    printf("data:[%s]\n",tmp_str);

    printf("===========================================================================\n");
    printf("write buf=%u\n",rbuf_write(my_test_rbuf,"ABCDEFG",strlen("ABCDEFG")));
    rbuf_reset(my_test_rbuf);
    printf("read rbuf=%u,",rbuf_read(my_test_rbuf,tmp_str,sizeof(tmp_str)));
    printf("data:[%s]\n",tmp_str);
    printf("my_test_rbuf all space=%u\n",rbuf_get_all_space(my_test_rbuf));
    printf("my_test_rbuf free space =%u\n",rbuf_get_free_space(my_test_rbuf));
    printf("my_test_rbuf used space=%u\n",rbuf_get_used_space(my_test_rbuf));
    //测试代码
    #if 0
    printf("\n===========================================================================\n");
    printf("===========================================================================\n");
    //测试静态定义ringbuf,测试随机读写
    DECLARE_RINGBUFFER(static_rbuf,2048);
    printf("static_rbuf all space=%u\n",rbuf_get_all_space(static_rbuf));
    printf("static_rbuf free space =%u\n",rbuf_get_free_space(static_rbuf));
    printf("static_rbuf used space=%u\n",rbuf_get_used_space(static_rbuf));
    char test_buf[4567]={0};
    int a_z_slen=strlen("abcdefghijklmnopqrstuvwxyz\n");
    for(int i=0;i<sizeof(test_buf)-1;)
    {
        if(a_z_slen+i<sizeof(test_buf)-1)
        {
            strcpy(test_buf+i,"abcdefghijklmnopqrstuvwxyz\n");
            i+=a_z_slen;
        }
        else
        {
            break;
        }
    }
    int len_per_write=0;
    int total_write_len=0;
    int read_len=19;
    char read_buf[20]={0};
    int total_read_len=0;
    while(1)
    {
        len_per_write=(++len_per_write)%sizeof(test_buf);
        unsigned int ret=rbuf_write(static_rbuf,test_buf+(total_write_len%sizeof(test_buf)),len_per_write);
        total_write_len+=ret;
        if(0==rbuf_get_free_space(static_rbuf))
        {
            //满了测试继续写1024
            rbuf_write(static_rbuf,test_buf,1024);
            break;
        }
        ret=rbuf_read(static_rbuf,read_buf,sizeof(read_buf)-1);
        total_read_len+=ret;
        printf("%s",read_buf);
    }
    printf("\ntotal write =%u,total read=%d\n",total_write_len,total_read_len);
    while(rbuf_get_used_space(static_rbuf))
    {
         memset(read_buf,0,sizeof(read_buf));
        unsigned int ret=rbuf_read(static_rbuf,read_buf,sizeof(read_buf)-1);
        total_read_len+=ret;
        printf("%s",read_buf);
    }
    printf("\ntotal write =%u,total read=%d\n",total_write_len,total_read_len);

    //测试动态分配ringbuf
    p_rbuf rbuf_new=rbuf_alloc(2048);
    if(!rbuf_new)
    {   
        return 0; 
    }
    printf("rbuf_new all space=%u\n",rbuf_get_all_space(rbuf_new));
    printf("rbuf_new free space =%u\n",rbuf_get_free_space(rbuf_new));
    printf("rbuf_new used space=%u\n",rbuf_get_used_space(rbuf_new));

    total_write_len=0;
    len_per_write=128;
    read_len=64;
    char read_buf2[65]={0};
    total_read_len=0;
    while(total_write_len<4096)
    {
        unsigned int ret=rbuf_write(rbuf_new,test_buf+total_write_len,len_per_write);
        total_write_len+=ret;
        total_read_len+=rbuf_read(rbuf_new,read_buf2,read_len);
        printf("%s",read_buf2);
        memset(read_buf2,0,sizeof(read_buf2));
        int cat_len=rbuf_cat(rbuf_new,0,read_buf2,read_len/2);
        total_read_len+=cat_len;
        total_read_len+=rbuf_cat(rbuf_new,cat_len,read_buf2+cat_len,read_len-cat_len);
        rbuf_out_cat(rbuf_new);
        printf("%s",read_buf2);
        memset(read_buf2,0,sizeof(read_buf2));
    }
    printf("\ntotal write =%u,total read=%d\n",total_write_len,total_read_len);

    #endif
    return 0;
}
