# ringbuf
ringbuffer 环形缓冲、非线程安全、高效环形缓冲

一:提供以下接口：
   1.静态定义ringbuf
	DECLARE_RINGBUFFER(name, bufsize)
   2.动态申请ringbuf
	p_rbuf rbuf_alloc(unsigned int bufsize);
   3.释放动态申请的ringbuffer
	void rbuf_free(p_rbuf *pp_in);
   4.从指定的ringbuf读出数据
	unsigned int rbuf_read(p_rbuf p_in,void *copy_to_buf,unsigned int bufsize);
   5.从指定的ringbuf查看数据，不会更新ringbuf的数据区
	unsigned int rbuf_cat(p_rbuf p_in,unsigned int offset,void *copy_to_buf,unsigned int bufsize);
   6.若查看数据后，确认将已经查看的数据读出(先调用rbuf_cat函数后，再调用该函数)
	void rbuf_out_cat(p_rbuf p_in);
   7.在指定的ringbuf写入数据
	unsigned int rbuf_write(p_rbuf p_in,void *from_buf,unsigned int bufsize);
   8.在指定的ringbuf获取剩余空间大小(byte)
	unsigned int rbuf_get_free_space(p_rbuf p_in);
   9.在指定的ringbuf获取已经使用空间大小(byte)
	unsigned int rbuf_get_used_space(p_rbuf p_in);
   10.在指定的ringbuf获取获取所分配的空间(byte)
	unsigned int rbuf_get_all_space(p_rbuf p_in);
   11.丢弃所有数据，置空指定的ringbuf
	void rbuf_reset(p_rbuf p_in);

二：例子
	参考test.c

三：编译：
	gcc *.c
