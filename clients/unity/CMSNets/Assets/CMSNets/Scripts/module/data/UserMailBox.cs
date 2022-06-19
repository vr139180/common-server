using System;
using System.Collections.Generic;
using System.Text;

using com.cms.client.network.gpro;

namespace com.cms.module.data
{
    public class UserMailBox
    {
        private MailNormalItems mails = new MailNormalItems();
        private int totleNum = 0;
        private int unreadNum = 0;
        //最后一次获取的id
        private Int64 lastMailGet = 0;
        private Boolean firstActive = true;

        public UserMailBox()
        {
        }

        public Mail_UserOnline_active build_mailboxactive_msg()
        {
            Mail_UserOnline_active act = new Mail_UserOnline_active();
            act.Firstactive = firstActive;
            act.LastMailiid = lastMailGet;

            firstActive = false;

            return act;
        }
    }
}
