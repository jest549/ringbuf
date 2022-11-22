#include "ringbuffer.h"
#include "stdlib.h"
#include "string.h"


p_rbuf rbuf_alloc(unsigned int bufsize)
{
    if(bufsize<1)
        return NULL;
    p_rbuf p=(p_rbuf)malloc(sizeof(struct ringbuffer_t));
    if(!p)
        return NULL;
    p->data=malloc(bufsize+1);
    if(!(p->data))
    {
        free (p);
        return NULL;
    }
    p->write_in=0;
    p->read_out=0;
    p->buf_size=bufsize;
    p->is_static=0;
    for(unsigned int i=0;i<bufsize+1;i++)
         ((char *)p->data)[i]=0;
    return p;
}

void rbuf_free(p_rbuf *pp_in)
{
    if(pp_in && (*pp_in) && (! ((*pp_in)->is_static) ) )
    {
        if((*pp_in)->data)
        {
            free((*pp_in)->data);
            (*pp_in)->data=NULL;
        }
        free(*pp_in);
        *pp_in=NULL;
    }
}

unsigned int rbuf_read(p_rbuf p_in,void *copy_to_buf,unsigned int bufsize)
{

    if(!p_in)
        return 0;
    unsigned int used_size=rbuf_get_used_space(p_in);
    if(used_size<1UL)
        return 0;
    unsigned int r_all_size=bufsize>used_size ? used_size : bufsize;
    unsigned int offset=p_in->read_out % p_in->buf_size;
    unsigned int r_len=r_all_size > (p_in->buf_size-offset)? (p_in->buf_size-offset) : r_all_size ;
    memcpy(copy_to_buf,((char*)(p_in->data))+offset,r_len);
    memcpy((char*)copy_to_buf+r_len,((char*)(p_in->data)),r_all_size-r_len);
    p_in->read_out+=r_all_size;
    return r_all_size;
}

unsigned int rbuf_cat(p_rbuf p_in,unsigned int in_offset,void *copy_to_buf,unsigned int bufsize)
{
    if(!p_in)
        return 0;
    if(in_offset>rbuf_get_used_space(p_in))
        return 0;
    p_in->read_cat=(unsigned int)(p_in->read_out+in_offset);
    unsigned int used_size= (unsigned int)(p_in->write_in-p_in->read_cat);
    if(used_size<1UL)
        return 0;
    unsigned int r_all_size=bufsize>used_size ? used_size : bufsize;
    unsigned int offset=p_in->read_cat % p_in->buf_size;
    unsigned int r_len=r_all_size > (p_in->buf_size-offset)? (p_in->buf_size-offset) : r_all_size ;
    memcpy(copy_to_buf,((char*)(p_in->data))+offset,r_len);
    memcpy((char*)copy_to_buf+r_len,((char*)(p_in->data)),r_all_size-r_len);
    p_in->read_cat+=r_all_size;
    return r_all_size;
}

void rbuf_out_cat(p_rbuf p_in)
{
    if(p_in && ((unsigned int)(p_in->write_in-p_in->read_cat) <= rbuf_get_used_space(p_in)) )
        p_in->read_out=p_in->read_cat;

}

unsigned int rbuf_write(p_rbuf p_in,void *from_buf,unsigned int bufsize)
{
    if(!p_in)
       return 0;
    unsigned int freesize=rbuf_get_free_space(p_in);
    if(freesize<1UL)
       return 0;
    unsigned int w_all_size=bufsize>freesize ? freesize : bufsize;
    unsigned int offset=p_in->write_in % p_in->buf_size;
    unsigned int w_len=w_all_size > (p_in->buf_size-offset)? (p_in->buf_size-offset) : w_all_size ;
    memcpy(((char*)(p_in->data))+offset,from_buf,w_len);
    memcpy(((char*)(p_in->data)),((char*)from_buf)+w_len,w_all_size-w_len);
    p_in->write_in=(unsigned int)(p_in->write_in+w_all_size);
    return w_all_size;
}

unsigned int rbuf_get_free_space(p_rbuf p_in)
{
    return p_in ? p_in->buf_size -((unsigned int)(p_in->write_in-p_in->read_out)): 0 ;
}

unsigned int rbuf_get_used_space(p_rbuf p_in)
{

    return p_in ? (unsigned int)(p_in->write_in-p_in->read_out) : 0 ;
}

unsigned int rbuf_get_all_space(p_rbuf p_in)
{
    return p_in ? p_in->buf_size : 0 ;
}

void rbuf_reset(p_rbuf p_in)
{
    if(p_in)
    {
        p_in->write_in=0;
        p_in->read_out=0;
    }
}

