#ifndef __THREADCONTEXTFACTORY_H__
#define __THREADCONTEXTFACTORY_H__

class ThreadContext;

class ThreadContextFactory
{
public:
	virtual ThreadContext*	get_threadcontext( int type) =0;
	virtual void release_threadcontext( int type, ThreadContext* pContext) =0;
};

#endif	//__THREADCONTEXTFACTORY_H__
