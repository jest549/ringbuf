#ifndef  _RINGBUFFER_H_
#define  _RINGBUFFER_H_

/****************************************************************
描述：ringbuf的一种实现，属于非线程安全的ringbuf，可用于MCU任务或单线程的读写中
作者：jest549
邮箱:jest549@gmail.com
源码路径：https://github.com/jest549/ringbuf
*****************************************************************/
struct ringbuffer_t 
{
	unsigned int write_in;
	unsigned int read_out;
    unsigned int read_cat;
	unsigned int	buf_size;
	void		*data;
    unsigned char is_static;
};
typedef struct ringbuffer_t* p_rbuf;  

//
/****************************************************************
* @brief    静态定义ringbuf
* @param    [in] name: 静态定义RingBuffer指针名
            [in] bufsize: 静态定义buffer的大小
* @return 
*****************************************************************/
#define DECLARE_RINGBUFFER(name, bufsize) \
static char name##_ringbuf_data[bufsize+1]={0};\
static struct ringbuffer_t name##_ringbuf_t={ \
        .write_in=0,\
        .read_out=0,\
        .buf_size=bufsize,\
        .data=name##_ringbuf_data, \
        .is_static=1 \
    }; p_rbuf name=& name##_ringbuf_t 
//

/****************************************************************
* @brief    动态申请ringbuf
* @param    [in] bufsize: 申请buffer大小.
* @return   RingBuffer 指针.
*****************************************************************/
p_rbuf rbuf_alloc(unsigned int bufsize);

/****************************************************************
* @brief    释放动态申请的ringbuffer
* @param    [in] pp_in: 指向 p_rbuf的指针
* @return   
*****************************************************************/
void rbuf_free(p_rbuf *pp_in);


/****************************************************************
* @brief    从指定的ringbuf读出数据
* @param    [in] p_in: 待操作的p_rbuf 
*           [out] copy_to_buf:存放读出数据的缓冲区
*           [in] bufsize:请求读出数据的长度
* @return   返回实际读取的数据长度，0为没有读出数据
*****************************************************************/
unsigned int rbuf_read(p_rbuf p_in,void *copy_to_buf,unsigned int bufsize);

/****************************************************************
* @brief    从指定的ringbuf查看数据，不会更新ringbuf的数据区
* @param    [in] p_in: 待操作的p_rbuf 
*           [int] offset:查看数据的起始偏移量
*           [out] copy_to_buf:存放查看数据的缓冲区
*           [in] bufsize:请求查看数据的长度
* @return   返回实际查看的数据长度，0为没有查看到数据
*****************************************************************/
unsigned int rbuf_cat(p_rbuf p_in,unsigned int offset,void *copy_to_buf,unsigned int bufsize);

/****************************************************************
* @brief    若查看数据后，确认将已经查看的数据读出(先调用rbuf_cat函数后，再调用该函数)
* @param    [in] p_in: 待操作的p_rbuf 
* @return   
*****************************************************************/
void rbuf_out_cat(p_rbuf p_in);
//

/****************************************************************
* @brief    在指定的ringbuf写入数据
* @param    [in] p_in: 待操作的p_rbuf 
*           [in] from_buf:存放写入数据的缓冲区
*           [in] bufsize:请求写入数据的长度
* @return   返回实际写入的数据长度，0为没有写入数据
*****************************************************************/
unsigned int rbuf_write(p_rbuf p_in,void *from_buf,unsigned int bufsize);

/****************************************************************
* @brief    在指定的ringbuf获取剩余空间大小(byte)
* @param    [in] p_in: 待操作的p_rbuf 
* @return   返回实际剩余的空间的长度(byte)
*****************************************************************/
unsigned int rbuf_get_free_space(p_rbuf p_in);

/****************************************************************
* @brief    在指定的ringbuf获取已经使用空间大小(byte)
* @param    [in] p_in: 待操作的p_rbuf 
* @return   返回实际剩余的空间的长度(byte)
*****************************************************************/
unsigned int rbuf_get_used_space(p_rbuf p_in);

/****************************************************************
* @brief    在指定的ringbuf获取获取所分配的空间(byte)
* @param    [in] p_in: 待操作的p_rbuf 
* @return   返回实际分配的空间长度(byte)
*****************************************************************/
unsigned int rbuf_get_all_space(p_rbuf p_in);

/****************************************************************
* @brief    丢弃所有数据，置空指定的ringbuf
* @param    [in] p_in: 待操作的p_rbuf 
* @return   
*****************************************************************/
void rbuf_reset(p_rbuf p_in);

#endif 
