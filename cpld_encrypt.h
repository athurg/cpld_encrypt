#ifndef __CPLD_ENCRYPT_H__
#define __CPLD_ENCRYPT_H__
	
#ifdef __cplusplus
extern "C" {
#endif

/*
 *	CPLD����У�麯��
 * 
 * ������
 * 	dat : �������ֵ
 * ���أ�
 * 	0   : У��ͨ��
 * 	-1  : У��ʧ��
 * 	-2  : ����ʧ��
 *
 */
int cpld_encrypt_check(char dat);

#ifdef __cplusplus
}
#endif

#endif //__CPLD_ENCRYPT_H__
