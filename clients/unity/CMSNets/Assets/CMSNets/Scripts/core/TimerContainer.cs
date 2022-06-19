using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.utils;

namespace com.cms.client.core.timer
{
    //timer function declare
    public delegate void timer_function_cb( long tnow, int interval, long iid, ref bool finish);

    #region timer struct

    public class TimerKey
    {
        public static TimerKey None = new TimerKey(-1, 0);

        public int step;
        public long tid;

        public TimerKey()
        {
            step = -1;
            tid = 0;
        }

        public TimerKey(int s, long id)
        {
            this.step = s;
            this.tid = id;
        }

        public bool is_equal(TimerKey t)
        {
            return this.step == t.step && this.tid == t.tid;
        }

        public bool is_none()
        {
            return this.step == -1 && this.tid == 0;
        }

        public bool is_validate()
        {
            return !is_none();
        }
    }

    public class timer_object
    {
        public timer_object before_node_;
        public timer_object next_node_;

        public long iid_;
        public timer_function_cb fun_;
        public long last_time_;

        public timer_object( long id, timer_function_cb f)
		{
            this.iid_ =id;
            this.fun_ =f;
            this.before_node_ =null;
            this.next_node_ =null;

            last_time_ = TimeUtil.get_timestamp();
		}

		public bool is_timeout( long tnow, int interval)
        {
			if( last_time_ + interval > tnow)
				return false;

			last_time_ =tnow;
			return true;
		}

    }

    public class TimerSequence
	{
        //header and tail
        private timer_object header_;
        private timer_object tail_;

        private Dictionary<long, timer_object> timers_ = new Dictionary<long, timer_object>();

        private int time_inverval_;
        private long timer_id_generator_;

		public TimerSequence( int t)
        {
            this.timer_id_generator_ = 1;
            this.time_inverval_ = t;
            this.header_ = null;
            this.tail_ = null;
        }

		public long add_timer( timer_function_cb f)
        {
            long tid = generate_timerid();
            timer_object tobj = new timer_object(tid, f);

            timers_[tobj.iid_] = tobj;

            if (header_ == null)
                tail_ = header_ = tobj;
            else
            {
                tail_.next_node_ = tobj;
                tobj.before_node_ = tail_;
                tail_ = tobj;
            }

            return tid;
        }

		public void del_timer( long iid)
        {
            if( !timers_.ContainsKey( iid))
                return;

            timer_object tobj =timers_[iid];
            //remove from map
            timers_.Remove(iid);

            //remove from list
	        timer_object bobj =tobj.before_node_;
	        timer_object nobj =tobj.next_node_;

	        if( bobj != null)
		        bobj.next_node_ =nobj;

	        if( nobj != null)
		        nobj.before_node_ =bobj;

	        if( header_ == tobj)
		        header_ =nobj;
	        if( tail_ == tobj)
		        tail_ =bobj;

            tobj.before_node_ = null;
            tobj.next_node_ = null;
        }

		public void do_timer( long tnow)
        {
            int cnt = timers_.Count;
            int icount = 0;
            while (header_ != null && icount < cnt)
            {
                ++icount;

                if (!header_.is_timeout(tnow, time_inverval_))
                    break;

                bool bfinish = false;
                long oldid = header_.iid_;
                header_.fun_( tnow, time_inverval_, header_.iid_, ref bfinish);

                //remove timer
                if (header_ == null)
                    break;

                //check the header is the old header
                if (bfinish && header_.iid_ == oldid)
                {
                    del_timer(header_.iid_);
                    continue;
                }

                if (header_ != tail_)
                {
                    timer_object nobj = header_.next_node_;
                    nobj.before_node_ = null;
                    header_.next_node_ = null;

                    header_.before_node_ = tail_;
                    tail_.next_node_ = header_;

                    tail_ = header_;
                    header_ = nobj;
                }

            }
        }

		protected long generate_timerid(){ return timer_id_generator_++;}

        public void release()
        {
            while (this.header_ != null)
            {
                timer_object c = this.header_.next_node_;
                c.before_node_ = null;
                this.header_.next_node_ = null;

                this.header_ = c;
            }

            this.header_ = null;
            this.tail_ = null;
            this.timers_.Clear();
        }

	};

    #endregion

    public class TimerContainer
    {
        private Dictionary<int, TimerSequence> timers_ = new Dictionary<int, TimerSequence>();

        public TimerContainer()
        {
        }

        public void reset()
        {
            foreach (var item in timers_.Values)
            {
                item.release();
            }

            timers_.Clear();
        }

        public TimerKey add_timer(int step, timer_function_cb fun)
        {
            if( step <= 0)
		        return TimerKey.None;

            TimerSequence ts = null;

            if (!timers_.ContainsKey(step))
            {
                ts = new TimerSequence(step);
                timers_[step] = ts;
            }
            else
                ts = timers_[step];

            long iid = ts.add_timer(fun);

	        return new TimerKey( step, iid);
        }

        public void del_timer(TimerKey tid)
        {
            if (!timers_.ContainsKey(tid.step))
                return;

            TimerSequence ts = timers_[tid.step];
            ts.del_timer(tid.tid);
        }

        public void delall_timer()
        {
            this.reset();
        }

        public void timer_tick()
        {
            long now_t = TimeUtil.get_timestamp();

	        timer_tick( now_t);
        }

        public void timer_tick(long tnow)
        {
            foreach (var item in timers_.Values)
            {
                item.do_timer(tnow);
            }
        }
    }
}
