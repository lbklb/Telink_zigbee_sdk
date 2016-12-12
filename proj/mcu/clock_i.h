
#pragma once

// we use clock insteady of timer, to differentiate OS timers utility
static inline void clock_enable_clock(int tmr, u32 en){
	if(0 == tmr){
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR0_EN, en);
	}else if(1 == tmr){
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR1_EN, en);
	}else{
		SET_FLD_V(reg_tmr_ctrl, FLD_TMR2_EN, en);
	}
}

#ifdef WIN32
static inline unsigned __int64   clock_get_cycle()
{ 
  __asm   _emit   0x0F 
  __asm   _emit   0x31 
}
#endif

static inline u32 clock_time(){
#ifdef WIN32
#if (__LOG_RT_ENABLE__)
	unsigned __int64 tt = (unsigned __int64)clock_get_cycle();
	u32 tick = (u32)(tt * 1000000 / (2*1000*1000*1000));	// assuming the cpu clock of PC is 2 giga HZ
	return tick;
#else
	static u32 tick = 0;
	// �������ݣ�����̫׼���ȽϺõ������ǣ�����һ��10��Ķ�ʱ����Ȼ��ʵ��ʱ������
	// �����е���������������ϵͳʱ�ӣ���Ϊ��Ҫ�ڶϵ�󱣳�timer ׼ȷ
	tick += 30;//(CLOCK_SYS_CLOCK_1US * 20);	
	return tick;
#endif
#else
	return reg_tmr0_tick;
#endif
}

static inline u32 clock_time2(int tmr){
	return reg_tmr_tick(tmr);
}

// check if the current time is exceed span_us from ref time
static inline u32 clock_time_exceed(u32 ref, u32 span_us){
	return ((u32)(clock_time() - ref) > span_us * CLOCK_SYS_CLOCK_1US);
}

// more efficient than clock_set_interval
static inline void clock_set_tmr_interval(int tmr, u32 intv){
	reg_tmr_capt(tmr) = intv;
}

static inline void clock_set_tmr_mode(int tmr, u32 m){
	if(0 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR0_MODE, m);
	}else if(1 == tmr){
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR1_MODE, m);
	}else{
		SET_FLD_V(reg_tmr_ctrl16, FLD_TMR2_MODE, m);
	}
}

static inline u32 clock_get_tmr_status(int tmr){
	if(0 == tmr){
		return reg_tmr_ctrl & FLD_TMR0_STA;
	}else if(1 == tmr){
		return reg_tmr_ctrl & FLD_TMR1_STA;
	}else{
		return reg_tmr_ctrl & FLD_TMR2_STA;
	}
}
