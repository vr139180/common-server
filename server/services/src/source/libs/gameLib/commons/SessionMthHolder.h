#ifndef __SESSIONMTHHOLDER_H__
#define __SESSIONMTHHOLDER_H__

#include <set>
#include <boost/scoped_array.hpp>
#include <cmsLib/net/NetAcceptorEvent.h>
#include <gameLib/LogExt.h>

template<typename T>
class SessionMthHolder
{
public:
	SessionMthHolder();
	virtual ~SessionMthHolder();

	void init_sessions(int maxsvr);
	void unint_sessions();

	void sessions_maintnce(u64 tnow);

public:
	T* get_sessionlink_by_session(boost::shared_ptr<NetSession> session);

	NetAcceptorEvent::NetSessionPtr ask_refuse_netsession_mth();
	NetAcceptorEvent::NetSessionPtr ask_free_netsession_mth();
	void ask_free_netsession_mth_confirm(T* pointer);

	void free_from_wait_mth(T* pointer);
	void remove_waitsession_mth(T* psession);
	void return_freesession_mth(T* psession);

	int get_max_servernum() { return max_server_num_; }

protected:
	void release();

protected:
	boost::scoped_array<T>	sessions_;
	int						max_server_num_;
	std::set<T*>			free_sessions_;
	std::set<T*>			wait_sessions_;
};

template<typename T>
SessionMthHolder<T>::SessionMthHolder()
{
}

template<typename T>
SessionMthHolder<T>::~SessionMthHolder()
{
}

template<typename T>
void SessionMthHolder<T>::init_sessions(int maxsvr)
{
	this->max_server_num_ = maxsvr;
	sessions_.reset(new T[max_server_num_]);
	for (int ii = 0; ii < max_server_num_; ++ii)
		free_sessions_.insert(&(sessions_[ii]));
	wait_sessions_.clear();
}

template<typename T>
void SessionMthHolder<T>::unint_sessions()
{
	release();
}

template<typename T>
void SessionMthHolder<T>::release()
{
	wait_sessions_.clear();
	free_sessions_.clear();
	sessions_.reset(0);
}

template<typename T>
void SessionMthHolder<T>::sessions_maintnce(u64 tnow)
{
	std::set<T*>::iterator iter = wait_sessions_.begin(), eiter = wait_sessions_.end();
	for (; iter != eiter; ++iter)
	{
		T* p = (*iter);
		p->update(tnow);
	}
}

template<typename T>
T* SessionMthHolder<T>::get_sessionlink_by_session(boost::shared_ptr<NetSession> session)
{
	for (int ii = 0; ii < max_server_num_; ++ii)
	{
		if (sessions_[ii].get_session() == session)
			return &(sessions_[ii]);
	}

	return 0;
}

template<typename T>
NetAcceptorEvent::NetSessionPtr SessionMthHolder<T>::ask_refuse_netsession_mth()
{
	NetAcceptorEvent::NetSessionPtr new_session;
	return new_session;
}

template<typename T>
NetAcceptorEvent::NetSessionPtr SessionMthHolder<T>::ask_free_netsession_mth()
{
	if (free_sessions_.size() == 0)
	{
		logError(out_net, "+++++++++++++++ask free session[0] failed+++++++++++++++");
		NetAcceptorEvent::NetSessionPtr new_session;
		return new_session;
	}
	else
	{
		std::set<T*>::iterator iter = free_sessions_.begin();
		T* pointer = (*iter);
		free_sessions_.erase(iter);
		//wait_sessions_.insert(pointer);

		pointer->prepare();

		logError(out_net, "+++++++++++++++ask free session free:%d wait:%d+++++++++++++++", free_sessions_.size(), wait_sessions_.size());

		return pointer->get_session();
	}
}

template<typename T>
void SessionMthHolder<T>::ask_free_netsession_mth_confirm(T* pointer)
{
	wait_sessions_.insert(pointer);
	logError(out_net, "+++++++++++++++ask free session confirm free:%d wait:%d+++++++++++++++", free_sessions_.size(), wait_sessions_.size());
}

template<typename T>
void SessionMthHolder<T>::free_from_wait_mth(T* pointer)
{
	wait_sessions_.erase(pointer);
	free_sessions_.insert(pointer);

	logError(out_net, "+++++++++++++++free_from_wait_mth free:%d wait:%d+++++++++++++++", free_sessions_.size(), wait_sessions_.size());
}

template<typename T>
void SessionMthHolder<T>::remove_waitsession_mth(T* psession)
{
	wait_sessions_.erase(psession);

	logError(out_net, "+++++++++++++++remove_waitsession_mth free:%d wait:%d+++++++++++++++", free_sessions_.size(), wait_sessions_.size());
}

template<typename T>
void SessionMthHolder<T>::return_freesession_mth(T* psession)
{
	free_sessions_.insert(psession);

	logError(out_net, "+++++++++++++++return_freesession_mth free:%d wait:%d+++++++++++++++", free_sessions_.size(), wait_sessions_.size());
}


#endif //__SESSIONMTHHOLDER_H__