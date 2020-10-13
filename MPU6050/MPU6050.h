/*
 * MPU6050.h
 *
 *  Created on: 2020��10��10��
 *      Author: HP
 */

#ifndef MPU6050_H_
#define MPU6050_H_



//��̬��ֵ���ĺ궨��
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SAMPLE_SIZE   100
#define FILTER_CNT  4       /*�˲�����ƽ����*/
#define MOST_ACTIVE_NULL                0        /*δ�ҵ����Ծ��*/
#define MOST_ACTIVE_X                   1        /*���Ծ��X*/
#define MOST_ACTIVE_Y                   2        /*���Ծ��Y*/
#define MOST_ACTIVE_Z                   3        /*���Ծ��Z*/
#define ACTIVE_PRECISION                200      /*��Ծ����С�仯ֵ*/
#define ABS(a) (0 - (a)) > 0 ? (-(a)) : (a)
#define DYNAMIC_PRECISION               450       /*��̬����*/


//������ٶȴ���ṹ��
typedef struct {
  short x;
  short y;
  short z;
}axis_info_t;

//��̬��ֵ���Ľṹ�壨�¾����ֵ����Сֵ��
typedef struct {
   axis_info_t newmax;
   axis_info_t newmin;
   axis_info_t oldmax;
   axis_info_t oldmin;
}peak_value_t;

/*һ��������λ�Ĵ��������ڹ��˸�Ƶ����*/
typedef struct slid_reg{
    axis_info_t new_sample;
    axis_info_t old_sample;
}slid_reg_t;

//�����˲���
typedef struct filter_avg{
axis_info_t info[FILTER_CNT];
    unsigned char count;
}filter_avg_t;

extern int step_cntt;
extern peak_value_t peak_VALUE;
extern slid_reg_t fliter;
static void filter_calculate(filter_avg_t *, axis_info_t *);
static void peak_value_init(peak_value_t *);
static void peak_update(peak_value_t *, axis_info_t *);
static char is_most_active(peak_value_t *);
static void detect_step(peak_value_t *, slid_reg_t *);
void  slid_Init(slid_reg_t *);
void  MPU6050_Init(void);
void check_step(void);
axis_info_t MPU_Slipper_Ave( axis_info_t data );



#endif /* MPU6050_H_ */
