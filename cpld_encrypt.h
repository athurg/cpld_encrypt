#ifndef __CPLD_ENCRYPT_H__
#define __CPLD_ENCRYPT_H__
	
#ifdef __cplusplus
extern "C" {
#endif

/*
 *	CPLD加密校验函数
 * 
 * 参数：
 * 	dat : 传入随机值
 * 返回：
 * 	0   : 校验通过
 * 	-1  : 校验失败
 * 	-2  : 其他失败
 *
 */
int cpld_encrypt_check(char dat);

#ifdef __cplusplus
}
#endif

#endif //__CPLD_ENCRYPT_H__
