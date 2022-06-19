using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.network;
using com.cms.client.core.timer;

namespace com.cms.client.core
{
    public class MainThreadBase
    {
        #region base propery

        private bool b_will_quit_ = false;
        protected int loop_num_ = 100;

        #endregion

        #region init and reset

        public MainThreadBase()
        {
            for (int ii = 0; ii < 2; ++ii)
            {
                system_cmds_[ii] = new List<CmdBase>();
                net_cmds_[ii] = new List<CmdBase>();
            }
        }

        public virtual void init(int loopnum)
        {
            this.loop_num_ = loopnum;
        }

        public virtual void uninit()
        {
            this.free_cmds();
        }

        public void update( int loop =1)
        {
            for( int ii =loop; ii > 0; --ii)
            {
                CmdBase pcmd = pop_one_cmd();
                if (pcmd == null)
                    break;

                pcmd.run_in_mainthread();
            }
        }

        public void quit_mainthread()
        {
            this.b_will_quit_ = true;
        }

        public bool is_quit()
        {
            return this.b_will_quit_;
        }

        #endregion

        #region thread command pools

        private object thread_lock_ = new object();

        private List<CmdBase>[] system_cmds_ =new List<CmdBase>[2];
        private List<CmdBase>[] net_cmds_ = new List<CmdBase>[2];

        private int cur_system_cmd_ = 0;
        private int cur_net_cmd_ = 0;

        public void regist_systemcmd(CmdBase cmd)
        {
            if (b_will_quit_)
                return;

            lock (thread_lock_)
            {
                system_cmds_[switch_another_cmd_list(cur_system_cmd_)].Add(cmd);
            }
        }

        public void regist_netcmd(CmdBase cmd)
        {
            if (b_will_quit_)
                return;

            lock (thread_lock_)
            {
                net_cmds_[switch_another_cmd_list(cur_net_cmd_)].Add(cmd);
            }
        }

        public CmdBase pop_one_cmd()
        {
            if (b_will_quit_)
                return null;

            CmdBase ret = null;

            if (system_cmds_[cur_system_cmd_].Count > 0)
            {
                ret =system_cmds_[cur_system_cmd_][0];
                system_cmds_[cur_system_cmd_].RemoveAt(0);
            }
            else if (net_cmds_[cur_net_cmd_].Count > 0)
            {
                ret = net_cmds_[cur_net_cmd_][0];
                net_cmds_[cur_net_cmd_].RemoveAt(0);
            }

            if (system_cmds_[cur_system_cmd_].Count == 0)
            {
                if (system_cmds_[switch_another_cmd_list(cur_system_cmd_)].Count > 0)
                {
                    lock (thread_lock_)
                    {
                        cur_system_cmd_ = switch_another_cmd_list(cur_system_cmd_);
                    }
                }
                else if (net_cmds_[cur_net_cmd_].Count == 0)
                {
                    if (net_cmds_[switch_another_cmd_list(cur_net_cmd_)].Count > 0)
                    {
                        lock (thread_lock_)
                        {
                            cur_net_cmd_ = switch_another_cmd_list(cur_net_cmd_);
                        }
                    }
                }
                
            }

            return ret;
        }

        private int switch_another_cmd_list(int c)
        {
            return c == 1 ? 0 : 1;
        }

        protected void free_cmds()
        {
            lock (thread_lock_)
            {
                for (int ii = 0; ii < 2; ++ii)
                {
                    system_cmds_[ii].Clear();
                    net_cmds_[ii].Clear();
                }
            }
        }

        #endregion

        #region timer related

        protected TimerContainer app_timer_ =new TimerContainer();

        public void apptimer_reset() { app_timer_.reset(); }

        public TimerKey add_apptimer(int step, timer_function_cb f) { return app_timer_.add_timer(step, f); }

        public void del_apptimer(TimerKey tid) { app_timer_.del_timer(tid); }

        public void delall_moduletimer() { app_timer_.delall_timer(); }

        #endregion
    }
}
