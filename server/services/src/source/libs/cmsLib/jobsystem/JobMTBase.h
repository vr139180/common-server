#ifndef __JOBMTBASE_H__
#define __JOBMTBASE_H__

class ThreadContext;

class JobMTBase
{
public:
    virtual ~JobMTBase(){};
	virtual void run_thread( ThreadContext* p) =0;
	virtual void run_main() =0;
	virtual int get_threadcontext_type() =0;
};

#endif	//__JOBMULTIBASE_H__
